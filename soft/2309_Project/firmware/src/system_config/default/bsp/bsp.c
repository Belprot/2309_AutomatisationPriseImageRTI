/*******************************************************************************
  Board Support Package Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    bsp.c

  Summary:
    Board Support Package implementation for PIC32MZ Embedded Connectivity (EC)
    Starter Kit.

  Description:
    This file contains routines that implement the board support package for
    PIC32MZ Embedded Connectivity (EC) Starter Kit.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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

#include "bsp.h"


// *****************************************************************************
/* Data Structure: 
    switch_port_channel_map[]

  Summary:
    Maps each switch to its port channel
  
  Description:
    The switch_port_channel_map array, indexed by BSP_SWITCH, maps each switch 
    to its port channel.

  Remarks:
    Refer to bsp.h for usage information.
*/
static const PORTS_CHANNEL switch_port_channel_map[] =
{
    PORT_CHANNEL_None,
    PORT_CHANNEL_C,
    PORT_CHANNEL_C
};

// *****************************************************************************
/* Data Structure: 
    switch_port_bit_pos_map[]

  Summary:
    Maps each switch to its port bit position
  
  Description:
    The switch_port_bit_pos_map array, indexed by BSP_SWITCH, maps each switch to its port bit position

  Remarks:
    Refer to bsp.h for usage information.
*/
static const PORTS_BIT_POS switch_port_bit_pos_map[] =
{
    PORTS_BIT_POS_-1,
    PORTS_BIT_POS_12,
    PORTS_BIT_POS_15
};


// *****************************************************************************
/* Function: 
    void BSP_SwitchStateGet(BSP_SWITCH switch);

  Summary:
    Returns the present state (pressed or not pressed) of the specified switch.
  
  Description:
    This function returns the present state (pressed or not pressed) of the
    specified switch.

  Remarks:
    Refer to bsp.h for usage information.
*/

BSP_SWITCH_STATE BSP_SwitchStateGet( BSP_SWITCH bspSwitch )
{
    return ( PLIB_PORTS_PinGet(PORTS_ID_0, switch_port_channel_map[bspSwitch], switch_port_bit_pos_map[bspSwitch]) );
}

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function: 
    void BSP_Initialize(void)

  Summary:
    Performs the necessary actions to initialize a board
  
  Description:
    This function initializes the LED, Switch and other ports on the board.
    This function must be called by the user before using any APIs present in
    this BSP.  

  Remarks:
    Refer to bsp.h for usage information.
*/

void BSP_Initialize(void )
{

}

/*******************************************************************************
 End of File
*/
