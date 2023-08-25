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
#include "gestDataIN.h"
#include "gestMCPWM.h"

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

// pour les test avec IRIG MIDI2, octave 3, note a choix
uint8_t instrument = 0x00;

// pour le test avec sequenceur, octave 2, note 7
//uint8_t instrument = 0x0f;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */


/* utilise pour pointer sur PWMAPINx  1 a 6
 * (parfois nomme PINMAPINx)
 * PWMAPINx active MCPWM(x+6)H plutot que MCPWMxL sur la pin concerne
 * PWMAPIN1 = BIT18, PWMAPIN2 = BIT19, ...
*/
#define PWMAPIN1 0x40000
#define PWMAPIN2 0x80000
#define PWMAPIN3 0x100000
#define PWMAPIN4 0x200000
#define PWMAPIN5 0x400000
#define PWMAPIN6 0x800000

void APP_Initialize ( void )
{
    /* Dans la version 2.06 d'harmony,
     * CFGCON.PINMAPINx n'est jamais modifie.
     * Cela a pour effet d'empecher l'utilisation des PWMxH
     * qui sont sur la meme pin qu'un PWMxL.
     * Afin de modifier CFGCON.PINMAPINx il faut unlock le registre :
     * "SYSKEY = 0xAA996655;" puis "SYSKEY = 0x556699AA;"
     * La on peut modifier CFGCON.
     * Enfin par securite il faut re-lock :
     * "SYSKEY = 0x0;"
     * Le lock est "time sensitive", cela a deux consequence :
     * 1) il n'est pas necessaire de le relock, mais il est mieux de le faire
     * 2) on a un temps limite pour modifier CFGCON,
     * on va donc couper les interrupts pendant ce temps pour etre sur
     * de ne pas etre interrompu
     * 
     * Arthur Guichard, 01.06.21
    */
    
    /* Disable Global Interrupts */
    SYS_INT_Disable();
    // Write Unlock Sequence to allow write access to CFGCON register
    SYSKEY = 0xAA996655; // Write Key1 to SYSKEY
    SYSKEY = 0x556699AA; // Write Key2 to SYSKEY
    // Dans notre cas on veut que les pins PWMxL de 1 à 4 soit remplacer,
        // on peut donc enlever PWMAPINx 5 et 6
    CFGCON |= (PWMAPIN1 + PWMAPIN2 + PWMAPIN3 + PWMAPIN4 /*+ PWMAPIN5 + PWMAPIN6*/);
    SYSKEY = 0x0; //Lock Write access to CFGCON register  
    /* Enable Global Interrupts */
    SYS_INT_Enable();
    
    
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    static uint8_t Octave = 0;
    static uint8_t OctaveMemory = 0;
    uint8_t i;

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            gest_USART_Initialize();
            
            DRV_USART0_Initialize();
            
            // afin de mettre a 0 les controles avance
            for (i = 0; i < 12; i++)
            {
                gest_MCPWM_Off(i);
            }
            
            DRV_MCPWM_Enable();
            
            TestOut1Off();
            TestOut2Off();
            TestOut3Off();
            TestOut4Off();
            
            DRV_TMR0_Start();
            
            APP_UpdateState(APP_STATE_OCTAVE);
            break;
        }
        
        
        case APP_STATE_OCTAVE:
        {
            // actualise l'octave
            Octave = Octave1StateGet();
            Octave += 2*Octave2StateGet();
            Octave += 4*Octave3StateGet();
            Octave += 8*Octave4StateGet();
            
            // si on change d'octave contrôlé on mets a 0 tout les PWMS
            if (OctaveMemory != Octave)
            {
                OctaveMemory = Octave;
                for (i = 0; i < 12; i++)
                {
                    gest_MCPWM_Off(i);
                }
            }
            
            APP_UpdateState(APP_STATE_WAIT);
            break;
        }

        
        case APP_STATE_SERVICE_TASKS:
        {
            LEDToggle();

            // si Octave == 0x0F
            if (Octave == 15)
            {
                // todo integrer un mode cmd pour changer le n° d'instru
                    // utiliser une fonction inspire de gest_USART_Traitement
                
                TestOut4StateSet((instrument & 0x8) >> 3);
                TestOut3StateSet((instrument & 0x4) >> 2);
                TestOut2StateSet((instrument & 0x2) >> 1);
                TestOut1StateSet(instrument & 0x1);
            }
            else
            {
                while (DRV_USART0_ReceiverBufferIsEmpty() == false)
                {
                    gest_USART_New_Data(DRV_USART0_ReadByte());
                }
                
                gest_USART_Traitement(instrument, Octave);
            }
            
            APP_UpdateState(APP_STATE_WAIT);
            break;
        }
        
        
        case APP_STATE_WAIT:
        {
            // code test sorties MCPWM, a faire tourner sans entre connecte
                // et avec le contenu de APP_STATE_SERVICE_TASKS en comments
            /*static uint32_t i = 0;
            static uint32_t j = 0;
            if (i == 100)
            {
                gest_MCPWM_Actualise_Velocity(j , 0);
                j++;
                if (j == 12)
                {
                    j = 0;
                }
                gest_MCPWM_Actualise_Velocity(j , 127);
                i = 0;
            }
            i++;*/
                    
            break;
        }
        
        
        /* The default state should never be executed. */
        default:
        {
            APP_UpdateState(APP_STATE_OCTAVE);
            break;
        }
    }
}


void APP_UpdateState ( APP_STATES NewState )
{
    appData.state = NewState;
}

/*******************************************************************************
 End of File
 */
