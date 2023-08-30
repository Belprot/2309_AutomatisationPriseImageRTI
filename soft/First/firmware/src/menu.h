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

    typedef enum
    {
        MAIN_MENU = 0,
        CHOICE_SEQ_MENU,
        SETTINGS_MENU,
        ABOUT_MENU,
        MOTOR_MENU,
                
    } MENU_STATE;
    
    typedef enum{
        
        CHOICE_SEQ_SEL = 0,
        SETTINGS_SEL,
        ABOUT_SEL,
                
    } MAIN_MENU_STATE;
    
    typedef enum{
        
        RETURN_SEL = 0,
                
    } CHOICE_SEQ_MENU_STATE;
    
    typedef struct{
        
        MENU_STATE menuState;
        MAIN_MENU_STATE mainMenuState;
        bool isPrinted;
        
    } MENU;

    void printInit(void);
    void printMainMenu(void);
    void processSelection(void);
    void processMenu(int8_t cursor);
    void clearFirstRow(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* MENU_H */

