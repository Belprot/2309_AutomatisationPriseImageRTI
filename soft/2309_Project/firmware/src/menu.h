/* 
 * File:   menu.h
 * Author: ricch
 *
 * Created on August 30, 2023, 10:17 AM
 */

#ifndef MENU_H
#define	MENU_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <stdbool.h>
    #include <stdint.h>
    #include <stepperDriver.h>
    #include "Mc32NVMUtil.h"

    #define RIGHT_ARROW 0x10

    // Enumerations
    
    /* All menus */
    typedef enum
    {
        MAIN_MENU = 0,
        CAPTURE_MODE_MENU,
        SETTINGS_MENU,
        ABOUT_MENU,
        MOTOR_MENU,
        MANUAL_MODE_MENU,
        LIGHT_MENU,
        BACKLIGHT_MENU,
        CAMERA_MENU,
        SAVE_DATA_MENU,
        AUTO_HOME_MENU,
        AUTOMATIC_MODE_MENU,
                
    } MENU_STATE;
    
    
    
    
    typedef enum{
        
        RETURN_SEL = 0,
                
    } COMMON;
    
    typedef enum{
        
        CAPTURE_MODE_SEL = 0,
        SETTINGS_SEL,
        ABOUT_SEL,
                
    } MAIN_MENU_LIST;
    
    typedef enum{
        
        LIGHT_INTENSITY_SEL = 1,
        LIGHT_TIME_SEL,
        TIME_BW_PICTURES,
                
    }LEDS_MENU_LIST;
    
    typedef enum{
        
        AUTO_HOME_START_SEL = 1,
                
    } AUTO_HOME_MENU_LIST;
    
    typedef enum{
        
        MANUAL_MODE_SEL = 1,
        AUTOMATIC_MODE_SEL,
                
    } CHOICE_SEQ_MENU_LIST;
    
    typedef enum{
        
        AUTO_HOME_SEL = 1,
        ANGLE_SEL,
                
    } MANUAL_MODE_MENU_LIST;
    
    typedef enum{
        
        AUTOMATIC_MODE_START_SEL = 1,
                
    } AUTO_MODE_MENU_LIST;
    
    typedef enum{
        
        MOTOR_SEL = 1,
        LEDS_SEL,
        BACKLIGHT_SEL,
        CAMERA_SEL,
        SAVE_DATA_SEL,
                
    } SETTINGS_MENU_LIST;
    
    typedef enum{
        
        SPEED_SEL = 1,
        GEAR_SEL,
        STEP_PER_TURN_SEL,
        POWER_SEL,
                
    } MOTOR_MENU_LIST;
    
    typedef enum{
        
        BACKLIGHT_INTENSITY_SEL = 1,
                
    } BACKLIGHT_MENU_LIST;
    
    typedef enum{
        
        EXPOSURE_TIME_SEL = 1,
        TIME_BW_PICTURES_SEL,
                
    } CAMERA_MENU_LIST;

    
    
    typedef enum{
        
        ANGLE_MODIF = 0,
        SPEED_MODIF,
        GEAR_MODIF,
        STEP_PER_TURN_MODIF,
        POWER_MODIF,
        BL_INTENSITY_MODIF,
        LIGHT_INTENSITY_MODIF,
        LIGHT_TIME_MODIF,
        EXPOSURE_TIME_MODIF,
        TIME_BW_PICTURES_MODIF,
                
        SAVE_DATA_START,
        AUTO_HOME_START,// INTERACT   
        AUTOMATIC_MODE_START,
        
    } MODIF_LIST;
    
    
    
    // Structures
    typedef struct{
        
        uint8_t menuPage;
        uint8_t menuSize;
        MENU_STATE menuState;
        MODIF_LIST modifState;
        
    } MENU;

    
    
    
    
    // Prototypes
    void printLcdInit(void);
    void printMainMenu(void);
    void printParameterMenuPage0(void);
    void printParameterMenuPage1();
    void printMotorMenu0(STEPPER_DATA *pStepperData);
    void printMotorMenu1(STEPPER_DATA *pStepperData);
    void printLedsMenu(void);
    void printChoiceSeqMenu(void);
    void printAboutMenu(void);
    void printManualModeMenu(STEPPER_DATA *pStepperData);
    void printAutoModeMenu(STEPPER_DATA *pStepperData);
    void printAutoHomeMenu(void);
    void printBackLightMenu(void);
    void printCameraMenu(void);
    void printSaveDataMenu(void);
    
    void menuManagementProcess(void);
    void menuActionProcess(int32_t pec12RotationValue);
    void menuDataProcess(int32_t *pec12RotationValue, STEPPER_DATA *pStepperData);
    void menuPrintProcess(STEPPER_DATA *pStepperData);
    
    void clearFirstRow(void);
    void printCursor(int32_t cursor);
    
    bool saveDataInEeprom(STEPPER_DATA *pStepperData);
    bool readDataFromEeprom(STEPPER_DATA *pStepperData);
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* MENU_H */

