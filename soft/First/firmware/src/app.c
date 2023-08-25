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


/* utilise pour pointer sur PWMAPINx  1 a 6
 * (parfois nommé PINMAPINx)
 * PWMAPINx active MCPWM(x+6)H plutot que MCPWMxL sur la pin concerne
 * PWMAPIN1 = BIT18, PWMAPIN2 = BIT19, ...
*/

#define PWMAPIN1 0x40000
#define PWMAPIN2 0x80000
#define PWMAPIN3 0x100000
#define PWMAPIN4 0x200000
#define PWMAPIN5 0x400000
#define PWMAPIN6 0x800000

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

    /* Disable Global Interrupts */
    SYS_INT_Disable();
    // Write Unlock Sequence to allow write access to CFGCON register
    SYSKEY = 0xAA996655; // Write Key1 to SYSKEY
    SYSKEY = 0x556699AA; // Write Key2 to SYSKEY
    // Dans notre cas on veut que les pins PWMxL de 1 soit remplacer,
    // on peut donc enlever PWMAPINx 2 à 6
    CFGCON |= (PWMAPIN4 /*+ PWMAPIN2  + PWMAPIN3 + PWMAPIN4 + PWMAPIN5 + PWMAPIN6*/);
    SYSKEY = 0x0; //Lock Write access to CFGCON register  
    /* Enable Global Interrupts */
    SYS_INT_Enable();
   // CapH_StateGet();
}



//----------------------------------------------------------------------------// APP_Tasks
void APP_Tasks ( void )
{
    // Main state machine
    switch(appData.appState){
        
        case APP_STATE_INIT:
            
            SIGN_LED_CMDOff();
            //DRV_MCPWM_Initialize();
            //DRV_MCPWM_Enable();
            
            
            // States machines update
            APP_UpdateAppState(APP_STATE_WAIT);
            break;

        case APP_STATE_SERVICE_TASKS:
            break;
        
        
        case APP_STATE_WAIT:
            // Nothing is supposed to happen here
            break;

        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 

/*******************************************************************************
 End of File
 */
