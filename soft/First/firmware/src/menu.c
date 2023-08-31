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



void initMenuParam(){
    
    menu.isPrinted = false;
    menu.menuState = MAIN_MENU;
}



void processSelection(void){
    
    static int16_t pec12RotationValue = 0;
    
    if(isInModifMode){
        
        int temp = getPec12IncrOrDecr();
        
        if(temp > 0) stepperData.isCW = true;
        else if(temp < 0) stepperData.isCW = false;
        
        pec12RotationValue += temp;
        processMenu(pec12RotationValue);
        
        if(getPec12SwitchEvent()){
            
            isInModifMode = false;
//            processMenu(pec12RotationValue);
            /* Put the cursor on the first line */ 
            pec12RotationValue = 0;
        }
    }
    if(isInModifMode == false){
        pec12RotationValue += getPec12IncrOrDecr();

        if(pec12RotationValue > 3) pec12RotationValue = 3;
        else if(pec12RotationValue < 0) pec12RotationValue = 0;

        clearFirstRow();
        char str[2];
        SetPostion(pec12RotationValue * 0x20);

        sprintf(str, "%c", RIGHT_ARROW);
        WriteString(str);

        if(getPec12SwitchEvent()){

            processMenu(pec12RotationValue);
            /* Put the cursor on the first line */ 
            pec12RotationValue = 0;
        }
    }
}
   
    
    
void processMenu(int16_t pec12RotationValue){
    
    /* Menu action switch */
    if(isInModifMode == false){
        switch(menu.menuState){

            case MAIN_MENU:

                switch(pec12RotationValue){

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
                
            //----------------------------------------------------------------//
            case MANUAL_MODE_MENU:
                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = CHOICE_SEQ_MENU;
                        break;

                    case ROTATION_SEL:
                        menu.modifState = ANGLE_MODIF;
                        isInModifMode = true;
                        break;
                }
                break;

            //----------------------------------------------------------------//
            case SETTINGS_MENU:

                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = MAIN_MENU;
                        break;
                        
                    case MOTOR_SEL:
                        menu.menuState = MOTOR_MENU;
                        break;
                }
                break;
                
            case MOTOR_MENU:
                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = MAIN_MENU;
                        break;
                        
                    case SPEED_SEL:
                        menu.modifState = SPEED_MODIF;
                }
                break;

            //----------------------------------------------------------------//
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
    
    /* Data action switch */
    if(isInModifMode){
        switch(menu.modifState){ 

            case ANGLE_MODIF:
                stepperData.stepToDo = pec12RotationValue * stepperData.stepPerTurn; //* stepperData.degreePerStep;  // gearValue

                printManualModeMenu();
                break;
        }
    }
    
    
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
        
        case CHOICE_SEQ_MENU:
            printChoiceSeqMenu();
            
            break;
        case MANUAL_MODE_MENU:
            printManualModeMenu();
            break;
        
        case ABOUT_MENU:
            printAboutMenu();
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
    WriteString("  Choix mode");
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
    WriteString("  Retro-eclairage");
}

void printMotorMenu(void){
    
    char str[21];
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Retour");
    SetPostion(LINE2);
    sprintf(str, "  Vitesse : %3d", stepperData.stepPerSec);
    WriteString(str);
    SetPostion(LINE3);
    sprintf(str, "  Reducteur : %3d", stepperData.gearValue);
    WriteString(str);
    SetPostion(LINE4);
    WriteString("  ");
}

void printChoiceSeqMenu(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Retour");
    SetPostion(LINE2);
    WriteString("  Mode manuel");
    SetPostion(LINE3);
    WriteString("  Mode automatique");
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

void printManualModeMenu(void){
    
    char str[21];
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Retour");
    SetPostion(LINE2);
    stepperData.realAngle = stepperData.motorStepNumber * stepperData.degreePerStep;
    sprintf(str, "  Angle : %05.1f", ((float)stepperData.stepToDo * 1.8));
    WriteString(str);
    SetPostion(LINE3);
    WriteString("  ");
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