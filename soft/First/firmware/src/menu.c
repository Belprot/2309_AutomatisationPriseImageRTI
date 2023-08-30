/* 
 * File:   menu.c
 * Author: ricch
 *
 * Created on August 30, 2023, 10:15 AM
 */

#include "menu.h"
#include "lcd_spi.h"
#include "stdio.h"
#include "pec12.h"

//const char RIGHT_ARROW = 0x10;


MENU menu;

void printInit(void){
    
    
    delay_ms(10);
    initDispl();
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("Systeme RTI");
    SetPostion(LINE2);
    WriteString("2023 ");
    SetPostion(LINE3);
    WriteString(" ");
    SetPostion(LINE4);
    WriteString("Meven Ricchieri");
    DisplayOnOff(DISPLAY_ON); //Disable cursor
}

void printMainMenu(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Choix sequ.");
    SetPostion(LINE2);
    WriteString("  Parametres");
    SetPostion(LINE3);
    WriteString("  A propos");
    SetPostion(LINE4);
    WriteString("  OK..");
}

void printParameterMenu(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Retour");
    SetPostion(LINE2);
    WriteString("  Moteur");
    SetPostion(LINE3);
    WriteString("  LEDs");
    SetPostion(LINE4);
    WriteString("  Retro-eclairag");
}

void printChoiceSeqMenu(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Retour");
    SetPostion(LINE2);
    WriteString("  ???");
    SetPostion(LINE3);
    WriteString("  ???");
    SetPostion(LINE4);
    WriteString("  ???");
}

void printAboutMenu(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Retour");
    SetPostion(LINE2);
    WriteString("  Version 1.0.0");
    SetPostion(LINE3);
    WriteString("  Meven Ricchieri");
    SetPostion(LINE4);
    WriteString("  08-09 2023");
}

/* Clear the first row all 4 lines */
void clearFirstRow(void){
    
    SetPostion(LINE1);
    WriteString(" ");
    SetPostion(LINE2);
    WriteString(" ");
    SetPostion(LINE3);
    WriteString(" ");
    SetPostion(LINE4);
    WriteString(" ");
}

void initMenuParam(){
    
    menu.isPrinted = false;
    menu.menuState = MAIN_MENU;
}



void processSelection(void){
    
    static int8_t cursor = 0; 
    
    
    cursor += getPec12IncrOrDecr();

    if(cursor > 3) cursor = 3;
    else if(cursor < 0) cursor = 0;

    clearFirstRow();
    char str[2];
    SetPostion(cursor * 0x20);

    sprintf(str, "%c", RIGHT_ARROW);
    WriteString(str);
    
    
    if(getPec12SwitchEvent()){
        
        processMenu(cursor);
        /* Put the cursor on the first line */ 
        cursor = 0;
    }
}
   
    
    
void processMenu(int8_t cursor){
    
    /* Action switch */
    switch(menu.menuState){
        
        case MAIN_MENU:
            
            switch(cursor){
            
                case CHOICE_SEQ_SEL:
                    menu.menuState = CHOICE_SEQ_MENU;
                    break;
                
                case SETTINGS_SEL:
                    menu.menuState = SETTINGS_MENU;
                    break;
                
                case ABOUT_SEL:
                    menu.menuState = ABOUT_MENU;
                    break;
            }
            break;
        
        case CHOICE_SEQ_MENU:
            
            switch(cursor){
            
                case RETURN_SEL:
                    menu.menuState = MAIN_MENU;
                    break;
            }
            break;
            
        case SETTINGS_MENU:
            
            switch(cursor){
            
                case RETURN_SEL:
                    menu.menuState = MAIN_MENU;
                    break;
            }
            break;
            
        case ABOUT_MENU:
            
            switch(cursor){
            
                case RETURN_SEL:
                    menu.menuState = MAIN_MENU;
                    break;
            }
            break;
        
        default:
            break;
    }
    
    
    
    /* Print switch */
    switch(menu.menuState){
        
        case MAIN_MENU:
            printMainMenu();
            
            break;
        
        case SETTINGS_MENU:
            printParameterMenu();
        
            break;
        
        case CHOICE_SEQ_MENU:
            printChoiceSeqMenu();
            
            break;
        
        case ABOUT_MENU:
            printAboutMenu();
            
            break;
        
        default:
            break;
    }
    
    
}