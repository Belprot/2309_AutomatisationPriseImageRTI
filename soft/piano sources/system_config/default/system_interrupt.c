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
#include "../../gestMCPWM.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************
 
void __ISR(_UART1_TX_VECTOR, ipl0AUTO) _IntHandlerDrvUsartTransmitInstance0(void)
{
    DRV_USART_TasksTransmit(sysObj.drvUsart0);
}
void __ISR(_UART1_RX_VECTOR, ipl1AUTO) _IntHandlerDrvUsartReceiveInstance0(void)
{
    // On a reçu une donnee, on passe en traitement
    APP_UpdateState(APP_STATE_SERVICE_TASKS);
    
    DRV_USART_TasksReceive(sysObj.drvUsart0);
}
void __ISR(_UART1_FAULT_VECTOR, ipl1AUTO) _IntHandlerDrvUsartErrorInstance0(void)
{
    DRV_USART_TasksError(sysObj.drvUsart0);
    
    // traitement d'une erreur possible
    if (PLIB_USART_ReceiverOverrunHasOccurred(USART_ID_1))
    {
        PLIB_USART_ReceiverOverrunErrorClear(USART_ID_1);
    }
}
 
 

 

 

 

 

 
 
 
// période 100us
void __ISR(_TIMER_1_VECTOR, ipl1AUTO) IntHandlerDrvTmrInstance0(void)
{
    static uint32_t i = 0;
    uint8_t j;
    
    // apres CmptOctave * 0.0001 [s]
    if (i >= CmptOctave)
    {
        // on mets a jour l'octave
        APP_UpdateState(APP_STATE_OCTAVE);
        i = 0;
    }
    else
    {
        i++;
    }
    
    // on actualise les 12 PWM
    for (j = 0; j < 12; j++)
    {
        // si le PWM en cours et en StartTime
        if (DataMCPWM.RemainingStartTime[j] > 0)
        {
            // on reduit le StartTime
            DataMCPWM.RemainingStartTime[j] --;
            // si le StartTime est tombe a 0 (on est passe en MonteTime)
            if (DataMCPWM.RemainingStartTime[j] == 0)
            {
                // on actualise le PWM
                gest_MCPWM_Actualise_Velocity(j, DataMCPWM.ValPWM[j]);
            }
        }
        // sinon si le PWM en cours et en MonteTime
        else if (DataMCPWM.RemainingMonteTime[j] > 0)
        {
            // on reduit le MonteTime
            DataMCPWM.RemainingMonteTime[j] --;
            // si le MonteTime est tombe a 0 (on est passe en Maintien)
            if (DataMCPWM.RemainingMonteTime[j] == 0)
            {
                // on actualise le PWM
                // gest_MCPWM_Actualise_Velocity(x, 0) met le PWMx
                    // au PWM de maintien
                gest_MCPWM_Actualise_Velocity(j, 0);
            }
        }
    }
    
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_1);
}
 


/*******************************************************************************
 End of File
*/
