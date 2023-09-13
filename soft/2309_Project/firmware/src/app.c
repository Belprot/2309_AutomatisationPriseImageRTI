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
#include "Mc32_spi_sm.h"
#include "lcd_spi.h"
#include "system/devcon/src/sys_devcon_local.h"
#include "pec12.h"
#include "menu.h"



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
    SPI_Init();
    
    initAppParam();
    initMenuParam();
    initStepperParam();
}



//----------------------------------------------------------------------------// APP_Tasks
void APP_Tasks ( void ){
    
    static uint16_t counter1 = 0;
    static uint16_t counter2 = 0;
    
    /* Main state machine */
    switch(appData.appState){
        //--------------------------------------------------------------------// APP_STATE_INIT
        case APP_STATE_INIT:
            /* Read data from EEPROM to restore presets */
            readDataFromEeprom(getMyStepperStruct());  
            /* Initialization of the motor */
            initStepperMotor();
            /* Update MCPWM Duty-cycle of other PWM with EEPROM data */
            updateMcpwmDuty();
            /* Turn on MCPWM */
            PLIB_MCPWM_Enable(MCPWM_ID_0); 
            /* Initialization sequence */
            initLcd();
            /* Print initialization menu */
            printLcdInit();
            /* Start useful Timers */
            DRV_TMR1_Start();
            DRV_TMR2_Start();
            /* Print main menu once all peripherals are configured */
            printMainMenu();            
            /* States machines update */
            APP_UpdateAppState(APP_STATE_WAIT);
            break;
            
        //--------------------------------------------------------------------// APP_STATE_SERVICE_TASKS 
        /* Frequency = 10'000Hz */
        case APP_STATE_SERVICE_TASKS:
           
            sequenceManagementProcess();
            
            if(counter2 >= 10){
                /* Frequency = 1'000Hz */
                counter2 = 0;
                /* Scan the activity of the rotary encoder */
                scanPec12();  
                /* Scan the activity of the switch S1 */
                scanSwitch();
            }
            if(counter1 >= 1000){
                /* Frequency = 10Hz */
                counter1 = 0;
                menuManagementProcess();
                SIGN_LED_CMDToggle();
            }
            counter1++;
            counter2++;
            
            // Calls the SPI do task state machine
            SPI_DoTasks();
            
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

void initAppParam(){
    
    /* Place the App state machine in its initial state. */
    appData.appState                = APP_STATE_INIT;
    appData.msCounter               = 0;
    appData.backLightIntensitiy     = 2500; /* 100% */
    appData.lightIntensity          = 2500; /* 100% */
    appData.exposureDuration        = 100;
    appData.timeBetweenPictures     = 2000;
    appData.isFiveShotsSeqEnable    = false;
    appData.seqClock1_ms            = 0;
    appData.angleBwEachSeq          = 10;
    appData.nbrOfShotsPerformed     = 0;
    appData.buzzerIntensity         = 0; /* 0% */
    appData.valSeq                  = 0;
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
    PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0, PWM_BL_CH, 
            appData.backLightIntensitiy);
}
int32_t getBlIntensity(void){
    
    return appData.backLightIntensitiy / 25;
}

//----------------------------------------------------------------------------// setAngleBwSeq
void setAngleBwEachSeq(int32_t *angleBwEachSeq){
    
    // Limit values to avoid problems
    if(*angleBwEachSeq < ANGLE_BW_EACH_SEQ_MIN) *angleBwEachSeq 
            = ANGLE_BW_EACH_SEQ_MIN;
    if(*angleBwEachSeq > ANGLE_BW_EACH_SEQ_MAX) *angleBwEachSeq 
            = ANGLE_BW_EACH_SEQ_MAX;
    
    /* 25 = 2500 / 100 */
    appData.angleBwEachSeq = *angleBwEachSeq;
}
int32_t getAngleBwEachSeq(void){
    
    return appData.angleBwEachSeq;
}

//----------------------------------------------------------------------------// scanSwitch
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
//----------------------------------------------------------------------------// getSwitchEvent
bool getSwitchEvent(void){
    
    bool isPressed = S1.isPressed;
    S1.isPressed = 0;
    
    return isPressed;
}

//----------------------------------------------------------------------------// initLcdSeq
void initLcd(void){
    
    RESET_LCD_CMDOff();
    APP_Delay_ms(1);
    RESET_LCD_CMDOn();
    APP_Delay_ms(10);
    initDispl();
    /* Create degree symbol for LCD uses */
    CreateLcdDegreeSymbol(0x01);
}

void updateMcpwmDuty(void){

    /* Update PWMs DutyCycle with data from EEPROM */
    PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0, PWM_BL_CH, 
            appData.backLightIntensitiy);
    PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0, PWM_BUZZER_CH, 
            appData.buzzerIntensity);
    PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0, PWM_DIM_CH, 
            appData.lightIntensity);
}
    


/*******************************************************************************
 End of File
 */
