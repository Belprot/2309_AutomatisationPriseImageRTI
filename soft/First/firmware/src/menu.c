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
#include "stepperDriver.h"


MENU menu;
extern STEPPER_DATA stepperData;
bool isInModifMode = 0;
bool isFirstDataProcessPass = false;


void initMenuParam(){
    
    menu.isPrinted = false;
    menu.menuState = MAIN_MENU;
}



void processSelection(void){
    
    static int32_t pec12RotationValue = 0;
    static bool isSimplePage = true;
    
    if(isInModifMode){
        
        int temp = getPec12IncrOrDecr();
        
        pec12RotationValue += temp;
        menuDataProcess(&pec12RotationValue);
        menuPrintProcess();
        
        // Pec12 switch pressed
        if(getPec12SwitchEvent()){
            
            isInModifMode = false;
            /* Put the cursor on the first line */ 
            pec12RotationValue = 0;
        }
    }
    if(isInModifMode == false){
        pec12RotationValue += getPec12IncrOrDecr();

        if(isSimplePage){
            if(pec12RotationValue > 3) pec12RotationValue = 3;
            else if(pec12RotationValue < 0) pec12RotationValue = 0;
        }

        printCursor(pec12RotationValue);
        

        if(getPec12SwitchEvent()){

            
            menuActionProcess(pec12RotationValue);
            menuDataProcess(&pec12RotationValue);
            menuPrintProcess();
            
            /* Put the cursor on the first line */
//            pec12RotationValue = 0;
        }
    }
}
   
    
//____________________________________________________________________________//
void menuActionProcess(int32_t pec12RotationValue){
    
    /* Menu action switch */
    if(isInModifMode == false){
        switch(menu.menuState){

            //----------------------------------------------------------------// Main menu
            case MAIN_MENU:

                switch(pec12RotationValue){

                    case CHOICE_SEQ_SEL:
                        menu.menuState = MODE_CHOICE_MENU;
                        break;

                    case SETTINGS_SEL:
                        menu.menuState = SETTINGS_MENU;
                        break;

                    case ABOUT_SEL:
                        menu.menuState = ABOUT_MENU;
                        break;
                }
                break;

            //----------------------------------------------------------------// Main menu -> Choice menu
            case MODE_CHOICE_MENU:

                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = MAIN_MENU;
                        break;

                    case MANUAL_MODE_SEL:
                        menu.menuState = MANUAL_MODE_MENU;
                        break;

                    case AUTOMATIC_MODE_SEL:
                        break;
                }
                break;
                
            //----------------------------------------------------------------// Main menu -> Choice menu -> Manual Mode
            case MANUAL_MODE_MENU:
                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = MODE_CHOICE_MENU;
                        break;
                        
                    case AUTO_HOME_SEL:
                        menu.menuState = AUTO_HOME_MENU;
                        break;

                    case ANGLE_SEL:
                        menu.modifState = ANGLE_MODIF;
                        isInModifMode = true;
                        isFirstDataProcessPass = true;
                        break;
                }
                break;
                
            case AUTO_HOME_MENU:
                    switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = MODE_CHOICE_MENU;
                        break;
                        
                    case AUTO_HOME_START_SEL:
                        menu.modifState = AUTO_HOME_START;
                        isInModifMode = true;
                        isFirstDataProcessPass = true;
                        break;
                    }
                break;
                
                //returnToHome(); PEUT ETRE METTRE AILLEUR
                break;

            //----------------------------------------------------------------// Main menu -> Settings menu
            case SETTINGS_MENU:

                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = MAIN_MENU;
                        break;
                        
                    case MOTOR_SEL:
                        menu.menuState = MOTOR_MENU;
                        break;
                        
                    case LEDS_SEL:
                        menu.menuState = LEDS_MENU;
                        break;
                }
                break;
            
            //----------------------------------------------------------------// Main menu -> Settings menu -> Motor menu
            case MOTOR_MENU:
                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = SETTINGS_MENU;
                        break;
                        
                    case SPEED_SEL:
                        menu.modifState = SPEED_MODIF;
                        isInModifMode = true;
                        break;
                        
                    case GEAR_SEL:
                        menu.modifState = GEAR_MODIF;
                        isInModifMode = true;
                        break;
                        
                    case STEP_PER_TURN_SEL:
                        menu.modifState = STEP_PER_TURN_MODIF;
                        isInModifMode = true;
                        break;
                }
                isFirstDataProcessPass = true;
                break;
                
            //----------------------------------------------------------------// Main menu -> Settings menu -> LEDs menu
            case LEDS_MENU:
                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = SETTINGS_MENU;
                        break;
                        
                    case LIGHT_TIME_SEL:
                        break;    
                }
                break;

            //----------------------------------------------------------------// Main menu -> About menu
            case ABOUT_MENU:

                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = MAIN_MENU;
                        break;
                }
                break;

            default:
                break;
        }
    }
}

//____________________________________________________________________________//
void menuDataProcess(int32_t *pec12RotationValue){
    
    /* Data action switch */
    if(isInModifMode){
        switch(menu.modifState){

            //----------------------------------------------------------------// Manual mode data :
            case ANGLE_MODIF:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getRotationToDo(&stepperData); /////// A TESTER ET VALIDER, PERTE DE PAS POSSIBLE
                }
                setRotationToDo(&stepperData, pec12RotationValue); 
                break;
                
            case AUTO_HOME_MENU:
                
                break;
            
            //----------------------------------------------------------------// Motor settings :
            case SPEED_MODIF:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getSpeed(&stepperData);
                }
                setSpeed(&stepperData, pec12RotationValue);
                break;
            
            case GEAR_MODIF:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getGearReduction(&stepperData);
                }
                setGearReduction(&stepperData, pec12RotationValue);
                break;
                
            case STEP_PER_TURN_MODIF :
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getAnglePerStep(&stepperData);
                }
                setAnglePerStep(&stepperData, pec12RotationValue);
                break;
                
            case AUTO_HOME_START:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
//                    isInModifMode = false; // AFFICHER .. ECRAN
                    autoHome(&stepperData);
                }
                else{
                    
//                    if(INDEXStateGet()){ // PAS ASSEZ RAPIDE  ICI
//                        
//                        // APELER FONCTION
//                        stepperData.stepToDo = 0;
//                    }
                    // attendre
                }
                break;
        }
    }
}




//____________________________________________________________________________//
void menuPrintProcess(void){
    /* Print switch */
    switch(menu.menuState){
        
        case MAIN_MENU:
            printMainMenu();
            break;
        
        case SETTINGS_MENU:
            printParameterMenu();
            break;
            
        case MOTOR_MENU:
            printMotorMenu();
            break;
           
        case LEDS_MENU:
            printLedsMenu();
            break;
        
        case MODE_CHOICE_MENU:
            printChoiceSeqMenu();
            
            break;
        case MANUAL_MODE_MENU:
            printManualModeMenu();
            break;
        
        case ABOUT_MENU:
            printAboutMenu();
            break;
        
        case AUTO_HOME_MENU:
            printAutoHomeMenu();
            break;
        
        default:
            break;
    }
}


void printInit(void){
    
    delay_ms(10);
    initDispl();
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("RTI System ");
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
    WriteString("  Mode choice");
    SetPostion(LINE2);
    WriteString("  Settings");
    SetPostion(LINE3);
    WriteString("  About");
    SetPostion(LINE4);
    WriteString("  OK..");
}

void printParameterMenu(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    WriteString("  Motor");
    SetPostion(LINE3);
    WriteString("  LEDs");
    SetPostion(LINE4);
    WriteString("  Back-light");
}

void printMotorMenu(void){
    
    char str[21];
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    sprintf(str, "  Speed : %03d", stepperData.stepPerSec);
    WriteString(str);
    SetPostion(LINE3);
    sprintf(str, "  Gear : %03d", stepperData.gearValue);
    WriteString(str);
    SetPostion(LINE4);
    sprintf(str, "  Step angle : %01.2f", stepperData.anglePerStep);
    WriteString(str);
}

void printLedsMenu(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    WriteString("  Light time");
    SetPostion(LINE3);
    WriteString("  Time bw lights");
    SetPostion(LINE4);
    WriteString("  ");
}

void printChoiceSeqMenu(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    WriteString("  Manual mode");
    SetPostion(LINE3);
    WriteString("  Auto mode");
    SetPostion(LINE4);
    WriteString("  ");
}

void printAboutMenu(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    WriteString("  Version 1.0.0");
    SetPostion(LINE3);
    WriteString("  Meven Ricchieri");
    SetPostion(LINE4);
    WriteString("  08-09 2023");
}

void printManualModeMenu(void){
    
    char str[21];
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    if(stepperData.isIndexed == true){
        sprintf(str, "  Auto home : %s", "DONE");
    } else {
        sprintf(str, "  Auto home : %s", "NOK");
    }
    WriteString(str);
    SetPostion(LINE3);
//    sprintf(str, "  Angle : %05.1f", (((float)stepperData.stepToDo * 1.8) 
//              / stepperData.gearValue));
    sprintf(str, "  Steps : %05d", stepperData.stepToDoReach);
    WriteString(str);
    SetPostion(LINE4);
    sprintf(str, "  Steps ef: %05d", stepperData.performedStep);
    WriteString(str);
}

void printAutoHomeMenu(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    WriteString("  Press to index");
    SetPostion(LINE3);
    WriteString("  ...");
    SetPostion(LINE4);
    WriteString("  ");
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

/* Print cursor */
void printCursor(int32_t cursor){
    
        clearFirstRow();
        char str[2];
        SetPostion(cursor * 0x20);
        sprintf(str, "%c", RIGHT_ARROW);
        WriteString(str);
}