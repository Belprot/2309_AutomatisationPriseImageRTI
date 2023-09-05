/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "Mc32SpiUtil.h"
#include "lcd_spi.h"
#include "system/devcon/src/sys_devcon_local.h"
#include "pec12.h"



// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;
SW S1;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************


//----------------------------------------------------------------------------// APP_UpdateAppState
void APP_UpdateAppState(APP_STATES newState){
    
    appData.appState = newState;
}

//----------------------------------------------------------------------------// APP_Initialize
void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.appState = APP_STATE_INIT;
    appData.msCounter = 0;
    appData.backLightIntensitiy = 2500;
    appData.lightTime = 100;
    appData.focusTime = 200;
    
    initMenuParam();
    initStepperData();

}



//----------------------------------------------------------------------------// APP_Tasks
void APP_Tasks ( void )
{
    unsigned char degreeSymbol[] = {
        0b00011100,
        0b00010100,
        0b00011100,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
    };
    
    /* Main state machine */
    switch(appData.appState){
        //--------------------------------------------------------------------// APP_STATE_INIT
        case APP_STATE_INIT:
            
            PLIB_MCPWM_Enable(MCPWM_ID_0);
            turnOffStepperPwms();
            
            /* Turn ON required PWMs */
            PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,PWM_BL_CH);
            PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,PWM_BUZZER_CH);
            PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,PWM_DIM_CH);
            /* Change PWMs DutyCycle */
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0, PWM_BL_CH, appData.backLightIntensitiy);
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0, PWM_BUZZER_CH, 500);
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0, PWM_DIM_CH, 2500);
            
            /* Disable RESET on both H bridge */
            RESET_AB_CMDOn();
            RESET_CD_CMDOn();
            
            APP_Delay_ms(1000);
            /* Print screen on LCD */
            printInit();
            
            DRV_TMR1_Start();
            DRV_TMR2_Start();
            
            
            /* Create a degree symbol at 0x01 address of the CG RAM */
            DefineCharacter(0x01, &degreeSymbol[0]);
            
            printMainMenu();
            
            
            
            takePicture(1);
            /* Trigger and focus camera */
            //TRIGGER_CMDOn();
            //FOCUS_CMDOn();
            
            /* States machines update */
            APP_UpdateAppState(APP_STATE_WAIT);
            break;
            
        //--------------------------------------------------------------------// APP_STATE_SERVICE_TASKS
        case APP_STATE_SERVICE_TASKS:
            
            /* 20Hz */
            menuManagementProcess();
            
            /* States machines update */
            APP_UpdateAppState(APP_STATE_WAIT);
            break;
        //--------------------------------------------------------------------// APP_STATE_WAIT
        case APP_STATE_WAIT:
            /* Nothing is supposed to happen here */
            break;

        //--------------------------------------------------------------------// default
        default:
            break;
    }
}

//----------------------------------------------------------------------------// APP_Delay_ms
void APP_Delay_ms(uint32_t ms){
    
    DRV_TMR3_Start();
//    SIGN_LED_CMDToggle();
    while(appData.msCounter < ms){
        
    }
//    SIGN_LED_CMDToggle();
    
    DRV_TMR3_Stop();
    appData.msCounter = 0;
}

//----------------------------------------------------------------------------// setBlIntensity
void setBlIntensity(int32_t *backLightIntensitiy){
    
    // Limit values to avoid problems
    if(*backLightIntensitiy < BACKLIGHT_INTENSITY_MIN) *backLightIntensitiy 
            = BACKLIGHT_INTENSITY_MIN;
    if(*backLightIntensitiy > BACKLIGHT_INTENSITY_MAX) *backLightIntensitiy 
            = BACKLIGHT_INTENSITY_MAX;
    
    /* 25 = 2500 / 100 */
    appData.backLightIntensitiy = *backLightIntensitiy * 25;
    PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0, PWM_BL_CH, appData.backLightIntensitiy);
}
int32_t getBlIntensity(void){
    
    return appData.backLightIntensitiy / 25;
}

//----------------------------------------------------------------------------// setLighIntensity
void setLightIntensity(int32_t *lightIntensity){
    
    // Limit values to avoid problems
    if(*lightIntensity < LIGHT_INTENSITY_MIN) *lightIntensity 
            = LIGHT_INTENSITY_MIN;
    if(*lightIntensity > LIGHT_INTENSITY_MAX) *lightIntensity 
            = LIGHT_INTENSITY_MAX;
    
    /* 25 = 2500 / 100 */
    appData.lightIntensity = *lightIntensity * 25;
    PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0, PWM_DIM_CH, appData.lightIntensity);
}
int32_t getLightIntensity(void){
    
    return appData.lightIntensity / 25;
}

//----------------------------------------------------------------------------// setLightTime
void setLightTime(int32_t *lightTime){
    
    // Limit values to avoid problems
    if(*lightTime < LIGHT_TIME_MIN) *lightTime = LIGHT_TIME_MIN;
    if(*lightTime > LIGHT_TIME_MAX) *lightTime = LIGHT_TIME_MAX;
    
    appData.lightTime = *lightTime;
}

int32_t getLightTime(void){
    
    return appData.lightTime;
}




//----------------------------------------------------------------------------// takePicture
void takePicture(LED_ID ledId){

    appData.ledId = ledId;
    /* Start the LED sequence */
    DRV_TMR0_Start();
}


void scanSwitch(void){
    
    // Save old states for debounce
    S1.state[3] = S1.state[2];
    S1.state[2] = S1.state[1];
    S1.state[1] = S1.state[0];
    S1.state[0] = SWITCHStateGet();
    
    // Check if switch is pressed
    if(S1.state[0] == 1 && S1.state[1] == 1
            && S1.state[2] == 0 && S1.state[3] == 0){
        
        S1.isPressed = true;
    }
}

bool getSwitchEvent(void){
    
    bool isPressed = S1.isPressed;
    S1.isPressed = 0;
    
    return isPressed;
}
/*******************************************************************************
 End of File
 */
