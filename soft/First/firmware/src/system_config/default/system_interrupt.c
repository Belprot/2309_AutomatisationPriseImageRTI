/*******************************************************************************
 System Interrupts File

  File Name:
    system_interrupt.c

  Summary:
    Raw ISR definitions.

  Description:
    This file contains a definitions of the raw ISRs required to support the
    interrupt sub-system.

  Summary:
    This file contains source code for the interrupt vector functions in the
    system.

  Description:
    This file contains source code for the interrupt vector functions in the
    system.  It implements the system and part specific vector "stub" functions
    from which the individual "Tasks" functions are called for any modules
    executing interrupt-driven in the MPLAB Harmony system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    interrupt-driven in the system.  These handles are passed into the individual
    module "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2011-2014 released Microchip Technology Inc.  All rights reserved.

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

#include "system/common/sys_common.h"
#include "app.h"
#include "system_definitions.h"
#include "lcd_spi.h"
#include "pec12.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************

extern APP_DATA appData;
extern STEPPER_DATA stepperData;

//----------------------------------------------------------------------------// TMR ID 0
void __ISR(_TIMER_1_VECTOR, ipl1AUTO) IntHandlerDrvTmrInstance0(void){
    
    static uint16_t counter = 0;
    /* Frequency = 10kHz */
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_1);
    
    
    
    //------------------------------------------------------------------------// Start of sequence
    if(counter == 0){
        
        switch (appData.ledId){
            /* Turn on LED */
            case PWR_LED1:
                LED1_CMDOn();
                break;

            case PWR_LED2:
                LED2_CMDOn();
                break;

            case PWR_LED3:
                LED3_CMDOn();
                break;

            case PWR_LED4:
                LED4_CMDOn();
                break;

            case PWR_LED5:
                LED5_CMDOn();
                break;
            }
        
        /* Focus on the target */
        FOCUS_CMDOn();
    }
    
    if(counter == appData.focusTime){
        
        /* Capture the target */
        TRIGGER_CMDOn();
    }
    
    if(counter == (appData.exposureTime - appData.focusTime)){
        
        TRIGGER_CMDOff();
        FOCUS_CMDOff();
    }
    
    //------------------------------------------------------------------------// End of sequence
    if(counter >= appData.lightTime){

        /* Turn off all power LED */
        LED1_CMDOff();
        LED2_CMDOff();
        LED3_CMDOff();
        LED4_CMDOff();
        LED5_CMDOff();
        counter = 0;
        DRV_TMR0_Stop();
    } else {
        
        counter++;
    }
    
}

//----------------------------------------------------------------------------// TMR ID 1
void __ISR(_TIMER_2_VECTOR, ipl1AUTO) IntHandlerDrvTmrInstance1(void){
    
    static uint8_t counter = 0;
    /* Frequency = 500Hz */
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_2);
    
    
    
    /* Scan the activity of the rotary encoder */
    scanPec12();
    
    /* Scan the activity of the switch S1 */
    scanSwitch();
    
    counter++;
    /* 20Hz */
    if(counter >= 50){
        
        counter = 0;
        
        /* States machines update */
        APP_UpdateAppState(APP_STATE_SERVICE_TASKS);
        SIGN_LED_CMDToggle();
    }
}

//----------------------------------------------------------------------------// TMR ID 2
void __ISR(_TIMER_3_VECTOR, ipl2AUTO) IntHandlerDrvTmrInstance2(void){
    
    /* Variable frequency */
//    SIGN_LED_CMDOff();
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_3);
    
    changeSpeed(getStepperStruct());
    processStepper(getStepperStruct());
//    SIGN_LED_CMDOn();
}
 
//----------------------------------------------------------------------------// TMR ID 3
void __ISR(_TIMER_4_VECTOR, ipl1AUTO) IntHandlerDrvTmrInstance3(void){
    
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_4);
    
    /* Timer for ms delay */
    appData.msCounter++;
}






/*******************************************************************************
 End of File
*/
