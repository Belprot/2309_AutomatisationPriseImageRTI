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
    appData.lightIntensity = 2500;
//    appData.lightTime = 800;
    appData.exposureDuration = 100;
//    appData.timeBetweenPictures = appData.lightTime + appData.focusDuration + 10;
    appData.timeBetweenPictures = 1000;
    appData.isFiveShotsSeqEnable = false;
    appData.seqClock1_ms = 0;
    appData.angleBwEachSeq = 10;
    
    initMenuParam();
    initStepperData();
}



//----------------------------------------------------------------------------// APP_Tasks
void APP_Tasks ( void ){
    
    static uint16_t counter = 0;
    static uint16_t counter2 = 0;
    
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
            
            /* Read data from EEPROM to restore presets */
            readDataFromEeprom(getStepperStruct());
            
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
            
            /* Start Timers */
            
            DRV_TMR1_Start();
            DRV_TMR2_Start();
            
            
            /* Create a degree symbol at 0x01 address of the CG RAM */
            DefineCharacter(0x01, &degreeSymbol[0]);
            
            printMainMenu();
            
            /* States machines update */
            APP_UpdateAppState(APP_STATE_WAIT);
            break;
            
        //--------------------------------------------------------------------// APP_STATE_SERVICE_TASKS 
        /* Frequency = 10000Hz */
        case APP_STATE_SERVICE_TASKS:
            
            
//            /* Take pictures if the flag is true */
//            if(appData.isFiveShotsSeqEnable == true){
//                
//                fiveShotsSeqProcess();
//            }
            
            if(appData.isFullImaginSeqEnable == true){
                
                fullImagingSeqProcess();
            }
            
            
            
            
            counter++;
            counter2++;
            if(counter2 >= 10){
                counter2 = 0;
                /* Scan the activity of the rotary encoder */
                scanPec12();  
                /* Scan the activity of the switch S1 */
                scanSwitch();
            }
            if(counter >= 500){
                
                counter = 0;
                /* Frequency = 20Hz */
                menuManagementProcess();
            }
            
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

//----------------------------------------------------------------------------// setExposureTime
void setExposureTime(int32_t *exposureTime){
   
    // Limit values to avoid problems
    if(*exposureTime < EXPOSURE_TIME_MIN) *exposureTime = EXPOSURE_TIME_MIN;
    if(*exposureTime > EXPOSURE_TIME_MAX) *exposureTime = EXPOSURE_TIME_MAX;
    
    appData.exposureDuration = *exposureTime;
}
int32_t getExposureTime(void){
    
    return appData.exposureDuration;
}

//----------------------------------------------------------------------------// setTimeBwPictures
void setTimeBwPictures(int32_t *timeBwPictures){
    
    // Limit values to avoid problems
    if(*timeBwPictures < TIME_BW_PICTURES_MIN) *timeBwPictures = TIME_BW_PICTURES_MIN;
    if(*timeBwPictures > TIME_BW_PICTURES_MAX) *timeBwPictures = TIME_BW_PICTURES_MAX;
    
    appData.timeBetweenPictures = *timeBwPictures;
}
int32_t getTimeBwPictures(void){
    
    return appData.timeBetweenPictures;
}


void fullImagingSeqProcess(){
    
    do{
        fiveShotsSeqProcess();
    // move motor (appData.angleBwEachSeq)
    }while(10);
    
}

//----------------------------------------------------------------------------// imagingSeqProcess
/* This function takes 5 pictures with 5 different LED */
void fiveShotsSeqProcess(void){
    
    if(appData.seqClock1_ms == 0){
        appData.ledId = PWR_LED1;
        
    } else if(appData.seqClock1_ms == 1 * appData.timeBetweenPictures){
        appData.ledId = PWR_LED2;
        
    } else if(appData.seqClock1_ms == 2 * appData.timeBetweenPictures){
        appData.ledId = PWR_LED3;
        
    } else if(appData.seqClock1_ms == 3 * appData.timeBetweenPictures){
        appData.ledId = PWR_LED4;
        
    } else if(appData.seqClock1_ms == 4 * appData.timeBetweenPictures){
        appData.ledId = PWR_LED5;

    } else if(appData.seqClock1_ms == 5 * appData.timeBetweenPictures){
        DRV_TMR0_Stop();
        DRV_TMR4_Stop();
        appData.isFiveShotsSeqEnable = false;
        appData.seqClock1_ms = 0;   
        appData.seqClock2_ms = 0; 
    }
    simpleShotProcess();
}

//----------------------------------------------------------------------------// simpleShotProcess
void simpleShotProcess(void){

    //------------------------------------------------------------------------// Start of sequence
    if(appData.seqClock2_ms == 0){

        switch (appData.ledId){
            /* Turn on LED */
            case PWR_LED1:
                turnOffAllPwrLeds();
                LED1_CMDOn();
                break;

            case PWR_LED2:
                turnOffAllPwrLeds();
                LED2_CMDOn();
                break;

            case PWR_LED3:
                turnOffAllPwrLeds();
                LED3_CMDOn();
                break;

            case PWR_LED4:
                turnOffAllPwrLeds();
                LED4_CMDOn();
                break;

            case PWR_LED5:
                turnOffAllPwrLeds();
                LED5_CMDOn();
                break;
        }
    }
    if(appData.seqClock2_ms == MARGIN_LED_DELAY){

        /* Capture the target */
        FOCUS_CMDOn();
        TRIGGER_CMDOn();
    }
    if(appData.seqClock2_ms == appData.exposureDuration + MARGIN_LED_DELAY){

        TRIGGER_CMDOff();
        FOCUS_CMDOff();
    }
    //------------------------------------------------------------------------// End of sequence
    if(appData.seqClock2_ms >= appData.exposureDuration + (2 * MARGIN_LED_DELAY)){

        turnOffAllPwrLeds();
        appData.seqClock2_ms = 0;
        appData.ledId = ALL_LED_DISABLE;
    }
}

void turnOffAllPwrLeds(void){
    
    /* Turn off all power LED */
    LED1_CMDOff();
    LED2_CMDOff();
    LED3_CMDOff();
    LED4_CMDOff();
    LED5_CMDOff();
}


/* Start a sequence for 5 shots */
void startFiveShotsSequence(){
    
    appData.isFiveShotsSeqEnable = true;
    DRV_TMR0_Start();
    DRV_TMR4_Start();
}


void startFullImagingSequence(void){
    
    appData.isFullImaginSeqEnable = true;
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
