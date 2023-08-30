/* 
 * File:   pec12.c
 * Author: ricch
 *
 * Created on August 30, 2023, 9:15 AM
 */

#include "app.h"
#include "pec12.h"
#include "lcd_spi.h"


PEC12 pec12;

void scanPec12(void){
    
    static int counter = 0;
//    char a_toPrint[21];
    
    // Save old states for debounce
    pec12.chA.state[3] = pec12.chA.state[2];
    pec12.chA.state[2] = pec12.chA.state[1];
    pec12.chA.state[1] = pec12.chA.state[0];
    pec12.chA.state[0] = CHANNEL_AStateGet();
    
    pec12.chB.state[1] = pec12.chB.state[0];
    pec12.chB.state[0] = CHANNEL_BStateGet();
    
    pec12.chC.state[3] = pec12.chC.state[2];
    pec12.chC.state[2] = pec12.chC.state[1];
    pec12.chC.state[1] = pec12.chC.state[0];
    pec12.chC.state[0] = PEC12R_SWStateGet();
    
    // Check if PEC12 is in rotation
    if(pec12.chA.state[0] == 0 && pec12.chA.state[1] == 0
            && pec12.chA.state[2] == 1 && pec12.chA.state[3] == 1){
        
        // Check direction of rotation
        if(pec12.chB.state[0] == 1 && pec12.chB.state[1]){
            
            // CW
            pec12.incrOrDecr++;
//            SetPostion(LINE3);
//            sprintf(a_toPrint, "counter = %d",counter);
//            WriteString(a_toPrint);
        }
        else{
            
            //CCW
            pec12.incrOrDecr--;
//            SetPostion(LINE3);
//            sprintf(a_toPrint, "counter = %d",counter);
//            WriteString(a_toPrint);
        }
    }
    // Check if PEC12 switch is pressed
    if(pec12.chC.state[0] == 0 && pec12.chC.state[1] == 0
            && pec12.chC.state[2] == 1 && pec12.chC.state[3] == 1){
        
        pec12.isPressed = true;
    }
}

int8_t getPec12IncrOrDecr(void){
    
    int8_t incrOrDecr = pec12.incrOrDecr;
    pec12.incrOrDecr = 0;
    
    return incrOrDecr;
}

int8_t getPec12SwitchEvent(void){
    
    int8_t isPressed = pec12.isPressed;
    pec12.isPressed = 0;
    
    return isPressed;
}