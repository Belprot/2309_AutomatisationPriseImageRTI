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
#include "Delays.h"
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
STEPPER_DATA stepperData;
extern PEC12 pec12;

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
    
    /* Init all PEC12 state values */
    appData.primaryPwmPeriod = 50000;
    
    stepperData.isCW = true;
    stepperData.motorStepNumber = 200;
    stepperData.stepPerSec = 100;
    stepperData.stepPerTurn = 200;
    stepperData.nbrFullTurn = 10;
    stepperData.gearValue = 1;
    stepperData.degreePerStep = 1.8;
    stepperData.performedStep = 0;
    stepperData.motorAngle = 0;
    stepperData.stepToDo = 0;
}



//----------------------------------------------------------------------------// APP_Tasks
void APP_Tasks ( void )
{
    /* Main state machine */
    switch(appData.appState){
        
        case APP_STATE_INIT:
            
            PLIB_MCPWM_Enable(MCPWM_ID_0);
            turnOffStepperPwms();
            
            /* Turn ON required PWMs */
            PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,PWM_BL_CH);
            PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,PWM_BUZZER_CH);
            PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,PWM_DIM_CH);
            /* Change PWMs DutyCycle */
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0, PWM_BL_CH, 700);
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0, PWM_BUZZER_CH, 500);
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0, PWM_DIM_CH, 2500);
            
            
            RESET_LCD_CMDOff();
            delay_ms(1);
            RESET_LCD_CMDOn();
            
            
            RESET_AB_CMDOn();
            RESET_CD_CMDOn();
            
            printInit();
            
            DRV_TMR0_Start();
            DRV_TMR1_Start();
            DRV_TMR2_Start();
            
            printMainMenu();
            initMenuParam();
            
            /* Trigger and focus camera */
            //TRIGGER_CMDOn();
            //FOCUS_CMDOn();
            
            /* States machines update */
            APP_UpdateAppState(APP_STATE_WAIT);
            break;
            
        case APP_STATE_SERVICE_TASKS:
            
            /* 20Hz */
            processSelection();
            
            /* States machines update */
            APP_UpdateAppState(APP_STATE_WAIT);
            break;
        
        
        case APP_STATE_WAIT:
            /* Nothing is supposed to happen here */

            break;


        /* The default state should never be executed. */
        default:
            break;
    }
}

 

/*******************************************************************************
 End of File
 */
