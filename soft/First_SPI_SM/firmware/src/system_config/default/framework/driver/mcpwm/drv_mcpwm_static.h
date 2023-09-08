/*******************************************************************************
  MCPWM Driver Interface Declarations for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_mcpwm_static.h

  Summary:
    MCPWM driver interface declarations for the static single instance driver.

  Description:
    The MCPWM device driver provides a simple interface to manage the MCPWM
    modules on Microchip microcontrollers. This file defines the interface
    Declarations for the IC driver.
    
  Remarks:
    Static interfaces incorporate the driver instance number within the names
    of the routines, eliminating the need for an object ID or object handle.
    
    Static single-open interfaces also eliminate the need for the open handle.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

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
//DOM-IGNORE-END

#ifndef _DRV_MCPWM_STATIC_H
#define _DRV_MCPWM_STATIC_H

#include "peripheral/mcpwm/plib_mcpwm.h"
#include "peripheral/int/plib_int.h"


// *****************************************************************************
// *****************************************************************************
// Section: Interface Headers for the static driver
// *****************************************************************************
// *****************************************************************************

void DRV_MCPWM_Initialize(void);
void DRV_MCPWM_Enable(void);
void DRV_MCPWM_Disable(void);

#endif // #ifndef _DRV_MCPWM_STATIC_H

/*******************************************************************************
 End of File
*/
