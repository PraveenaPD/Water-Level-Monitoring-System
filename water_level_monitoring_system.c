/*
 * File:   water_level_monitoring_system.c
 * Author: hp
 *
 * Created on 25 November, 2024, 10:56 AM
 */


#include <xc.h>
void delay();
void data(char A);
void cmd(char A);
void display();
void delay1();
void clearAfterCursor();
void waterLevel();


void main(void) {
    ADCON1 = 0x0F;
    TRISA  = 0xFF;
    TRISB  = 0x00;
    TRISC  = 0x00;
    TRISD  = 0x00;
    
    cmd(0x38); //Function Set: 8-bit, 2 Line, 5×7 Dots
    cmd(0x80); //Force cursor to the beginning ( 1st line)
    cmd(0x01); //Clear Display 
    cmd(0x06); //Increment cursor (shift cursor to right)
    cmd(0x0E); //Display on Cursor on
    
    int prevMode = -1;
    while(1){
//        for 
        if(RA4 == 1){
            if (prevMode != 1) {
                prevMode = 1; // Update mode
                cmd(0x01); 
                cmd(0x0E);
                display("Manual Mode");
                cmd(0x01);
                
            }
            
            waterLevel();
            
            
            if(RA5 ==1){
                RB5 = 1;
                cmd(0xC0);
                display("Motor is ON");
                clearAfterCursor();
                delay();
                
                
            }else if(RA5 == 0){
                RB5 = 0;
                cmd(0xC0);
                display("Motor is OFF");
                delay1();
            }
        
//for automatic mode
        }else if(RA4 ==0){
            
            if (prevMode != 0) {
                
                prevMode = 0; // Update mode
                cmd(0x01);
                cmd(0x0E);
                display("Automatic Mode");
                cmd(0x01);
                
            }
            
            waterLevel();
            
            if(RA0 && RA1 && RA2 && RA3 == 1){
                
                RB5 = 0;
                cmd(0xC0);
                display("Motor is OFF");
                delay();
                   
            }else if(RA0 == 0 && RA1 == 0 && RA2 == 0 && RA3 == 1){
                 
                RB5 = 1;
                cmd(0xC0);
                display("Motor is ON");
                clearAfterCursor();
                delay();
                
            }
        }
    
    }
    
    
    
    return;
}

void delay(){
    
    for(int i=0;i<150;i++){
        for(int j=0;j<150;j++);
    }
}

void delay1(){
    
    for(int i=0;i<1200;i++){
        for(int j=0;j<450;j++);
    }
}
void data(char A){
    PORTD = A;
    RC0 = 1; // RS = High
    RC1 = 1; // E = High
    delay();
    RC1 = 0; // E = LOW
    
}
void cmd(char A){
    PORTD = A;
    RC0 = 0; // RS = LOW
    RC1 = 1; // E = High
    delay();
    RC1 = 0; // E = LOW
    
}

void display(const char *P){  // const--> address of the pointer fixed only value is changing
    while(*P){
        data(*P++);
    }
}
//Function for clear 
void clearAfterCursor() {
    for (int i = 0; i < 2; i++) {
        data(' '); 
    }
    cmd(0x10);
}

void waterLevel(){
    cmd(0x80);
    cmd(0x0C);
    display("Water Level ");
    if(RA0 && RA1 && RA2 && RA3 ==1){
        display("100%");
        
    }else if(RA0 == 0 && RA1 && RA2 && RA3 ==1){
        display("50%");
        
    }else if(RA0 || RA1 == 0 && RA2 && RA3 == 1){
        display("25%");
        
    }else if(RA0 || RA1 == 0 || RA2 == 0 && RA3 == 1){
        display("0%");
        
    }
    clearAfterCursor();
    
}