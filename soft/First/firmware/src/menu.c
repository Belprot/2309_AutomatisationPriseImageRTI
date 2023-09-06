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

    menu.menuPage = 0;
    menu.menuSize = 2;
    menu.menuState = MAIN_MENU;
}


//____________________________________________________________________________// menuManagementProcess
void menuManagementProcess(void){
    
    static int32_t pec12RotationValue = 0;
    
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

        if(pec12RotationValue > menu.menuSize) pec12RotationValue =  menu.menuSize;
        else if(pec12RotationValue < 0) pec12RotationValue = 0;

        if(pec12RotationValue <= 3){
            
            menu.menuPage = 0;
            menuPrintProcess(getStepperStruct());
            printCursor(pec12RotationValue);
        }
        else{
            
            menu.menuPage = 1;
            menuPrintProcess(getStepperStruct());
            printCursor(pec12RotationValue - 4);
        }
        
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
        
        startImaging(1);
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
                        menu.menuSize = 2;
                        break;

                    case SETTINGS_SEL:
                        menu.menuState = SETTINGS_MENU;
                        menu.menuSize = 5;
                        break;

                    case ABOUT_SEL:
                        menu.menuState = ABOUT_MENU;
                        menu.menuSize = 0;
                        break;
                }
                break;

            //----------------------------------------------------------------// Main menu -> Choice menu
            case CAPTURE_MODE_MENU:

                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = MAIN_MENU;
                        menu.menuSize = 2;
                        break;

                    case MANUAL_MODE_SEL:
                        menu.menuState = MANUAL_MODE_MENU;
                        menu.menuSize = 3;
                        break;

                    case AUTOMATIC_MODE_SEL:
                        //....
                        //....
                        break;
                }
                break;
                
            //----------------------------------------------------------------// Main menu -> Choice menu -> Manual Mode
            case MANUAL_MODE_MENU:
                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = CAPTURE_MODE_MENU;
                        menu.menuSize = 2;
                        break;
                        
                    case AUTO_HOME_SEL:
                        menu.menuState = AUTO_HOME_MENU;
                        menu.menuSize = 1;
                        break;

                    case ANGLE_SEL:
                        menu.modifState = ANGLE_MODIF;
                        isInModifMode = true;
                        isFirstDataProcessPass = true;
                        break;
                }
                break;
                
            //----------------------------------------------------------------//
            case AUTO_HOME_MENU:
                    switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = CAPTURE_MODE_MENU;
                        menu.menuSize = 2;
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
                        menu.menuSize = 2;
                        break;
                        
                    case MOTOR_SEL:
                        menu.menuState = MOTOR_MENU;
                        menu.menuSize = 3;
                        break;
                        
                    case LEDS_SEL:
                        menu.menuState = LIGHT_MENU;
                        menu.menuSize = 1;
                        break;
                    
                    case BACKLIGHT_SEL:
                        menu.menuState = BACKLIGHT_MENU;
                        menu.menuSize = 1;
                        break;
                        
                    case CAMERA_SEL:
                        menu.menuState = CAMERA_MENU;
                        menu.menuSize = 3;
                        break;
                    
                    case SAVE_DATA_SEL:
                        menu.menuState = SAVE_DATA_MENU;
                        menu.menuSize = 1;
                        break;
                        
                }
                break;
            
            //----------------------------------------------------------------// Main menu -> Settings menu -> Motor menu
            case MOTOR_MENU:
                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = SETTINGS_MENU;
                        menu.menuSize = 5;
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
                        menu.menuSize = 5;
                        break;
                        
                    case LIGHT_INTENSITY_SEL:
                        menu.modifState = LIGHT_INTENSITY_MODIF;
                        isInModifMode = true;
                        isFirstDataProcessPass = true;
                        break;
                        
//                    case LIGHT_TIME_SEL: // <--- in camera param
//                        menu.modifState = LIGHT_TIME_MODIF;
//                        isInModifMode = true;
//                        isFirstDataProcessPass = true;
//                        break;
                }
                break;
                
            //----------------------------------------------------------------// Main menu -> Settings menu -> Back-light menu
            case BACKLIGHT_MENU:
                switch(pec12RotationValue){

                    case RETURN_SEL:
                        menu.menuState = SETTINGS_MENU;
                        menu.menuSize = 5;
                        break;
                        
                    case LIGHT_INTENSITY_SEL:
                        menu.modifState = BL_INTENSITY_MODIF;
                        isInModifMode = true;
                        isFirstDataProcessPass = true;
                        break;
                }
                break;
                
            //----------------------------------------------------------------// Main menu -> Settings menu -> Camera
            case CAMERA_MENU:
                switch(pec12RotationValue){
                    
                    case RETURN_SEL:
                        menu.menuState = SETTINGS_MENU;
                        menu.menuSize = 5;
                        break;  
                        
                    case EXPOSURE_TIME_SEL:
                        menu.modifState = EXPOSURE_TIME_MODIF;
                        isInModifMode = true;
                        break;
                        
                    case TIME_BW_PICTURES_SEL:
                        menu.modifState = TIME_BW_PICTURES_MODIF;
                        isInModifMode = true;
                        break;
                }
                isFirstDataProcessPass = true;
                break; 
                
            //----------------------------------------------------------------// Main menu -> Settings menu -> Save data
            case SAVE_DATA_MENU:
                switch(pec12RotationValue){
                    
                    case RETURN_SEL:
                        menu.menuState = SETTINGS_MENU;
                        menu.menuSize = 5;
                        break;  
                        
                    case SAVE_DATA_SEL - 4:
                        menu.modifState = SAVE_DATA_START;
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
                        menu.menuSize = 2;
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

            //----------------------------------------------------------------// ANGLE_MODIF
            case ANGLE_MODIF:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    /* A TESTER ET VALIDER, PERTE DE PAS POSSIBLE */
                    *pec12RotationValue = getRotationToDo(pStepperData);
                }
                setRotationToDo(pStepperData, pec12RotationValue); 
                break;
            
            //----------------------------------------------------------------// SPEED_MODIF
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
                
            //----------------------------------------------------------------// LIGHT_INTENSITY_MODIF
            case LIGHT_INTENSITY_MODIF:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getLightIntensity();
                }
                setLightIntensity(pec12RotationValue);
                break;
                
            //----------------------------------------------------------------// LIGHT_TIME_MODIF
//            case LIGHT_TIME_MODIF:
//                if(isFirstDataProcessPass){
//                    
//                    isFirstDataProcessPass = false;
//                    *pec12RotationValue = getLightTime();
//                }
//                setLightTime(pec12RotationValue);
//                break;
                
            //----------------------------------------------------------------// EXPOSURE_TIME_MODIF
            case EXPOSURE_TIME_MODIF:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getExposureTime();
                }
                setExposureTime(pec12RotationValue);
                break;
                
            //----------------------------------------------------------------// TIME_BW_PICTURES_MODIF
            case TIME_BW_PICTURES_MODIF:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
                    *pec12RotationValue = getTimeBwPictures();
                }
                setTimeBwPictures(pec12RotationValue);
                break;
                
                
                
            //----------------------------------------------------------------// SAVE_DATA_START
            case SAVE_DATA_START:
                if(isFirstDataProcessPass){
                    
                    isFirstDataProcessPass = false;
//                    isInModifMode = false; // AFFICHER .. ECRAN
                    saveDataInEeprom(pStepperData);
                    /* Once the data are saved, back to previous menu */
                    isInModifMode = false;
                    menu.menuState = SETTINGS_MENU;
                    menu.menuSize = 5;
                    
                }
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
            switch (menu.menuPage){
                case 0: printParameterMenuPage0();
                    break;
                case 1: printParameterMenuPage1();
                    break;
            }
            
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
                    
        case CAMERA_MENU:
            printCameraMenu();
            break;
            
        case SAVE_DATA_MENU:
            printSaveDataMenu();
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
        
        APP_Delay_ms(75);
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

void printParameterMenuPage0(void){
    
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

void printParameterMenuPage1(void){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Camera");
    SetPostion(LINE2);
    WriteString("  Save data");
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
//    SetPostion(LINE3);
//    sprintf(str, "  Light time: %03dms", appData.lightTime);
//    WriteString(str);
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

void printCameraMenu(void){
    
    char str[21];
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    sprintf(str, "  Expos time: %04dms", appData.exposureDuration);
    WriteString(str);
    SetPostion(LINE3);
    sprintf(str, "  Time bw pic:%04dms", appData.timeBetweenPictures);
    WriteString(str);
    SetPostion(LINE4);
    WriteString("  Trigger : cable"); // <-- or IR but not ready
}

void printSaveDataMenu(){
    
    ClrDisplay();
    SetPostion(LINE1);
    WriteString("  Return");
    SetPostion(LINE2);
    WriteString("  Confirm to save");
    SetPostion(LINE3);
    WriteString("  ! Old values will ");
    SetPostion(LINE4);
    WriteString("  be overwritten ! ");
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


bool saveDataInEeprom(STEPPER_DATA *pStepperData){ // dataToSaveInEeprom
    
    DATA_IN_EEPROM dataToSaveInEeprom;
    
    /* Set the structure value for saving in EEPROM */
    dataToSaveInEeprom.stepPerSec   = pStepperData->stepPerSec;
    dataToSaveInEeprom.stepPerTurn  = pStepperData->stepPerTurn;
    dataToSaveInEeprom.gearValue    = pStepperData->gearValue;
    dataToSaveInEeprom.anglePerStep = pStepperData->anglePerStep;
    
    dataToSaveInEeprom.lightIntensity       = appData.lightIntensity;
    dataToSaveInEeprom.timeBetweenPictures  = appData.timeBetweenPictures;
    dataToSaveInEeprom.exposureDuration     = appData.exposureDuration;
    
    dataToSaveInEeprom.backLightIntensitiy  = appData.backLightIntensitiy;
    
    dataToSaveInEeprom.controlValue = CONTROL_VALUE;
    
    Init_DataBuff();
    /* Write in the EEPROM */
    NVM_WriteBlock((uint32_t*)&dataToSaveInEeprom, sizeof(dataToSaveInEeprom));
    
    return 0;
}

/* Read the parameters from the EEPROM */
bool readDataFromEeprom(STEPPER_DATA *pStepperData){
    
    DATA_IN_EEPROM dataReadFromEeprom;
    
    Init_DataBuff();
    /* Read in the EEPROM */
    NVM_ReadBlock((uint32_t*)&dataReadFromEeprom, sizeof(dataReadFromEeprom));
    
    /* Check if the control value is already inside the EEPROM */
    if(dataReadFromEeprom.controlValue == CONTROL_VALUE){
        
        /* Save data from EEPROM */
        pStepperData->stepPerSec    = dataReadFromEeprom.stepPerSec;
        pStepperData->stepPerTurn   = dataReadFromEeprom.stepPerTurn;
        pStepperData->gearValue     = dataReadFromEeprom.gearValue;
        pStepperData->anglePerStep  = dataReadFromEeprom.anglePerStep;
        
        appData.lightIntensity      = dataReadFromEeprom.lightIntensity;
        appData.timeBetweenPictures = dataReadFromEeprom.timeBetweenPictures;
        appData.exposureDuration    = dataReadFromEeprom.exposureDuration;
        
        appData.backLightIntensitiy = dataReadFromEeprom.backLightIntensitiy;
    
    } else {
        
        // SAVE INIT VAL
        saveDataInEeprom(pStepperData);
    }
}