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
extern APP_DATA appData;
bool isInModifMode = 0;
bool isFirstDataProcessPass = false;


void initMenuParam(){

    menu.isPrinted = false;
    menu.menuState = MAIN_MENU;
}


//____________________________________________________________________________// menuManagementProcess
void menuManagementProcess(void){
    
    static int32_t pec12RotationValue = 0;
    static bool isSimplePage = true;
    
    /* Get PEC12 increments or decrements if there are */
    int incrOrDecr = getPec12IncrOrDecr();
    //------------------------------------------------------------------------// isInModifMode == true
    if(isInModifMode){
        
        pec12RotationValue += incrOrDecr;
        menuDataProcess(&pec12RotationValue, getStepperStruct());
        menuPrintProcess(getStepperStruct());
        
        /* PEC12 switch pressed */
        if(getPec12SwitchEvent()){
            
            /* Leave modification mode */
            isInModifMode = false;
            /* Put the cursor on the first line */ 
            pec12RotationValue = 0;
        }
    }
    //------------------------------------------------------------------------// isInModifMode == false
    if(isInModifMode == false){
        
        pec12RotationValue += incrOrDecr;

        if(isSimplePage){
            if(pec12RotationValue > 3) pec12RotationValue = 3;
            else if(pec12RotationValue < 0) pec12RotationValue = 0;
        } else{
            if(pec12RotationValue > 3){
                
            }
        }

        printCursor(pec12RotationValue);
        
        /* PEC12 switch pressed */
        if(getPec12SwitchEvent()){

            menuActionProcess(pec12RotationValue);
            menuDataProcess(&pec12RotationValue, getStepperStruct());
            menuPrintProcess(getStepperStruct());
            
            /* Put the cursor on the first line */
//            pec12RotationValue = 0;
        }
    }
    if(getSwitchEvent()){
        
//        imagingSeqProcess();
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
                        menu.menuState = CAPTURE_MODE_MENU;
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
            case CAPTURE_MODE_MENU:

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
                        menu.menuState = CAPTURE_MODE_MENU;
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
                        menu.menuState = CAPTURE_MODE_MENU;
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
                        menu.menuState = LIGHT_MENU;
                        break;
                    
                    case BACKLIGHT_SEL:
                        menu.menuState = BACKLIGHT_MENU;
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
                        isFirstDataProcessPass = true;
                        break;
                        
                    case GEAR_SEL:
                        menu.modifState = GEAR_MODIF;
                        isInModifMode = true;
                        isFirstDataProcessPass = true;
                        break;
                        
                    case STEP_PER_TURN_SEL:
                        menu.modifState = STEP_PER_TURN_MODIF;
                        isInModifMode = true;
                        isFirstDataProcessPass = true;
                        break;
                }
                break;
                
            //----------------------------------------------------------------// Main menu -> Settings menu -> Light menu
            case LIGHT_MENU:
                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = SETTINGS_MENU;
                        break;
                        
                    case LIGHT_INTENSITY_SEL:
                        menu.modifState = LIGHT_INTENSITY_MODIF;
                        isInModifMode = true;
                        isFirstDataProcessPass = true;
                        break;
                        
                    case LIGHT_TIME_SEL:
                        menu.modifState = LIGHT_TIME_MODIF;
                        isInModifMode = true;
                        isFirstDataProcessPass = true;
                        break;
                }
                break;
                
            //----------------------------------------------------------------// Main menu -> Settings menu -> Back-light
            case BACKLIGHT_MENU:
                switch(pec12RotationValue){
                    
                    case RETURN_SEL:
                        menu.menuState = SETTINGS_MENU;
                        break;  
                        
                    case BACKLIGHT_INTENSITY_SEL:
                        menu.modifState = BL_INTENSITY_MODIF;
                        isInModifMode = true;
                        break;
                }
                isFirstDataProcessPass = true;
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
void menuDataProcess(int32_t *pec12RotationValue, STEPPER_DATA *pStepperData){
    
    /* Data action switch */
    if(isInModifMode){
        switch(menu.modifState){

            //----------------------------------------------------------------// Manual mode data :
            case ANGLE_MODIF:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getRotationToDo(pStepperData); /////// A TESTER ET VALIDER, PERTE DE PAS POSSIBLE
                }
                setRotationToDo(pStepperData, pec12RotationValue); 
                break;
                
            case AUTO_HOME_MENU:
                
                break;
            
            //----------------------------------------------------------------// Motor settings :
            case SPEED_MODIF:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getSpeed(pStepperData);
                }
                setSpeed(pStepperData, pec12RotationValue);
                break;
            
            //----------------------------------------------------------------// GEAR_MODIF
            case GEAR_MODIF:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getGearReduction(pStepperData);
                }
                setGearReduction(pStepperData, pec12RotationValue);
                break;
                
            //----------------------------------------------------------------// STEP_PER_TURN_MODIF
            case STEP_PER_TURN_MODIF :
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getAnglePerStep(pStepperData);
                }
                setAnglePerStep(pStepperData, pec12RotationValue);
                break;
            
            //----------------------------------------------------------------// BL_INTENSITY_MODIF
            case BL_INTENSITY_MODIF :
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getBlIntensity();
                }
                setBlIntensity(pec12RotationValue);
                break;
                
            //----------------------------------------------------------------//    
            case LIGHT_INTENSITY_MODIF:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getLightIntensity();
                }
                setLightIntensity(pec12RotationValue);
                break;
                
            //----------------------------------------------------------------//
            case LIGHT_TIME_MODIF:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getLightTime();
                }
                setLightTime(pec12RotationValue);
                break;
                
            //----------------------------------------------------------------// AUTO_HOME_START
            case AUTO_HOME_START:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
//                    isInModifMode = false; // AFFICHER .. ECRAN
                    autoHome(pStepperData);
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
void menuPrintProcess(STEPPER_DATA *pStepperData){
    /* Print switch */
    switch(menu.menuState){
        
        case MAIN_MENU:
            printMainMenu();
            break;
        
        case SETTINGS_MENU:
            printParameterMenu();
            break;
            
        case MOTOR_MENU:
            printMotorMenu(pStepperData);
            break;
           
        case LIGHT_MENU:
            printLedsMenu();
            break;
        
        case BACKLIGHT_MENU:
            printBackLightMenu();
            break;
                    
        case CAPTURE_MODE_MENU:
            printChoiceSeqMenu();
            
            break;
        case MANUAL_MODE_MENU:
            printManualModeMenu(pStepperData);
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
    
    char str[2];
    RESET_LCD_CMDOff();
    APP_Delay_ms(1);
    RESET_LCD_CMDOn();
    APP_Delay_ms(10);
    initDispl();
    ClrDisplay();
    DisplayOnOff(DISPLAY_ON); //Disable cursor
    SetPostion(LINE1);
    WriteString("Auto RTI Capt System");
    SetPostion(LINE2);
    WriteString("08-09 2023");
    SetPostion(LINE3);
    WriteString("Meven Ricchieri");
    SetPostion(LINE4);
    
    int i;
    for (i = 0; i < 20; i++){
        
        APP_Delay_ms(100);
        SetPostion(LINE4 + i);
        sprintf(str, "%c", 0xD0);
        WriteString(str);
    }
    APP_Delay_ms(150);
}

void printMainMenu(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Capture mode");
    SetPostion(LINE2);
    WriteString("  Settings");
    SetPostion(LINE3);
    WriteString("  About");
    SetPostion(LINE4);
    WriteString("  ");
}

void printParameterMenu(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    WriteString("  Motor");
    SetPostion(LINE3);
    WriteString("  Power light");
    SetPostion(LINE4);
    WriteString("  Back-light");
}

void printMotorMenu(STEPPER_DATA *pStepperData){
    
    char str[21];
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    sprintf(str, "  Speed : %03d", pStepperData->stepPerSec);
    WriteString(str);
    SetPostion(LINE3);
    sprintf(str, "  Gear : %03d", pStepperData->gearValue);
    WriteString(str);
    SetPostion(LINE4);
    sprintf(str, "  Step angle : %01.2f", pStepperData->anglePerStep);
    WriteString(str);
}

void printLedsMenu(void){
    
    char str[21];
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    /* 0.04 = 100 / 2500 */
    sprintf(str, "  Intensity : %03.0f%%", ((float)appData.lightIntensity * 0.04));
    WriteString(str);
    SetPostion(LINE3);
    sprintf(str, "  Light time: %03dms", appData.lightTime);
    WriteString(str);
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

void printManualModeMenu(STEPPER_DATA *pStepperData){
    
    char str[21];
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    if(pStepperData->isIndexed == true){
        sprintf(str, "  Auto home : %s", "DONE");
    } else {
        sprintf(str, "  Auto home : %s", "NOK");
    }
    WriteString(str);
    SetPostion(LINE3);
    sprintf(str, "  Angle : %05.1f%c", (((float)pStepperData->stepToDoReach * 1.8) 
            / pStepperData->gearValue), 0x01);
//    sprintf(str, "  Steps      : %05d", stepperData.stepToDoReach);
    WriteString(str);
    SetPostion(LINE4);
    sprintf(str, "  Steps      : %05d", pStepperData->performedStep);
    WriteString(str);
}

void printAutoHomeMenu(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    WriteString("  Press to index");
    SetPostion(LINE3);
    WriteString("  ");
    SetPostion(LINE4);
    WriteString("  ");
}

void printBackLightMenu(void){
    
    char str[21];
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    /* 0.04 = 100 / 2500 */
    sprintf(str, "  Intensity : %03.0f%%", ((float)appData.backLightIntensitiy * 0.04));
    WriteString(str);
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
    
    char str[2];
    clearFirstRow();
    SetPostion(cursor * 0x20);
    sprintf(str, "%c", RIGHT_ARROW);
    WriteString(str);
}