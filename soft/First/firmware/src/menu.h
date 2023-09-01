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

    #define RIGHT_ARROW 0x10

    // Enumerations
    typedef enum
    {
        MAIN_MENU = 0,
        CHOICE_SEQ_MENU,
        SETTINGS_MENU,
        ABOUT_MENU,
        MOTOR_MENU,
        MANUAL_MODE_MENU,
        LEDS_MENU,
        BACKLIGHT_MENU,
        RETURN_HOME_MENU,
                
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
        
        LIGHT_TIME_SEL = 1,
                
    }LEDS_MENU_LIST;
    
    typedef enum{
        
        MANUAL_MODE_SEL = 1,
        AUTOMATIC_MODE_SEL,
                
    } CHOICE_SEQ_MENU_LIST;
    
    
    
    typedef enum{
        
        RETURN_HOME_SEL = 1,
        ANGLE_SEL,
                
    } MANUAL_MODE_MENU_SEL;
    
    typedef enum{
        
        MOTOR_SEL = 1,
        LEDS_SEL,
                
    } SETTINGS_MENU_LIST;
    
    typedef enum{
        
        SPEED_SEL = 1,
        GEAR_SEL,
        STEP_PER_TURN_SEL,
                
    } MOTOR_MENU_LIST;
    
    
    
    /* X is when the value is auto modified */
    typedef enum{
        
        ANGLE_MODIF = 0,
        SPEED_MODIF,
        GEAR_MODIF,
        STEP_PER_TURN_MODIF,
        
                
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
    void printMotorMenu(void);
    void printLedsMenu(void);
    void printChoiceSeqMenu(void);
    void printAboutMenu(void);
    void printManualModeMenu(void);
    
    void processSelection(void);
    void menuActionProcess(int16_t pec12RotationValue);
    void menuDataProcess(int16_t *pec12RotationValue);
    void menuPrintProcess(void);
    
    void clearFirstRow(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* MENU_H */

