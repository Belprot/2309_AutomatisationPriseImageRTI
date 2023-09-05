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

    #define RIGHT_ARROW 0x10

    // Enumerations
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
        AUTO_HOME_MENU,
                
    } MENU_STATE;
    
    typedef enum{
        
        RETURN_SEL = 0,
                
    } COMMON;
    
    
    
    typedef enum{
        
        CHOICE_SEQ_SEL = 0,
        SETTINGS_SEL,
        ABOUT_SEL,
                
    } MAIN_MENU_LIST;
    
    typedef enum{
        
        LIGHT_INTENSITY_SEL = 1,
        LIGHT_TIME_SEL,
        TIME_BW_PICTURES, //<---- mettre dans r�glage appareil photo
                
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
                
    } MANUAL_MODE_MENU_SEL;
    
    typedef enum{
        
        MOTOR_SEL = 1,
        LEDS_SEL,
        BACKLIGHT_SEL,
                
    } SETTINGS_MENU_LIST;
    
    typedef enum{
        
        SPEED_SEL = 1,
        GEAR_SEL,
        STEP_PER_TURN_SEL,
                
    } MOTOR_MENU_LIST;
    
    typedef enum{
        
        BACKLIGHT_INTENSITY_SEL = 1,
                
    } BACKLIGHT_MENU_LIST;
    
    /* X is when the value is auto modified */
    // PEUT ETRE MODIFIER VAL MOD ET INTERACT
    typedef enum{
        
        ANGLE_MODIF = 0,
        SPEED_MODIF,
        GEAR_MODIF,
        STEP_PER_TURN_MODIF,
        BL_INTENSITY_MODIF,
        LIGHT_INTENSITY_MODIF,
        LIGHT_TIME_MODIF,
                
        AUTO_HOME_START,// INTERACT   
        
    } MODIF_LIST;
    
    
    
    // Structures
    typedef struct{
        
        MENU_STATE menuState;
        MODIF_LIST modifState;
        bool isPrinted;
        
    } MENU;

    
    
    // Prototypes
    void printInit(void);
    void printMainMenu(void);
    void printParameterMenu(void);
    void printMotorMenu(STEPPER_DATA *pStepperData);
    void printLedsMenu(void);
    void printChoiceSeqMenu(void);
    void printAboutMenu(void);
    void printManualModeMenu(STEPPER_DATA *pStepperData);
    void printAutoHomeMenu(void);
    void printBackLightMenu(void);
    
    void menuManagementProcess(void);
    void menuActionProcess(int32_t pec12RotationValue);
    void menuDataProcess(int32_t *pec12RotationValue, STEPPER_DATA *pStepperData);
    void menuPrintProcess(STEPPER_DATA *pStepperData);
    
    void clearFirstRow(void);
    void printCursor(int32_t cursor);
    
#ifdef	__cplusplus
}
#endif

#endif	/* MENU_H */

