/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "stepperDriver.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "system_config.h"
#include "system_definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

#define SYS_CLK 40000000

#define PWM_A_CMD_CH MCPWM_CHANNEL1
#define PWM_B_CMD_CH MCPWM_CHANNEL1
#define PWM_C_CMD_CH MCPWM_CHANNEL2
#define PWM_D_CMD_CH MCPWM_CHANNEL2
#define PWM_BL_CH MCPWM_CHANNEL3
#define PWM_BUZZER_CH MCPWM_CHANNEL4
#define PWM_DIM_CH MCPWM_CHANNEL6
    
#define MARGIN_LED_DELAY 50
    
/* Intensity in percent */
#define BACKLIGHT_INTENSITY_MIN 0    
#define BACKLIGHT_INTENSITY_MAX 100
/* Intensity in percent */
#define LIGHT_INTENSITY_MIN 10    
#define LIGHT_INTENSITY_MAX 100
/* Time in ms */
#define LIGHT_TIME_MIN 50
#define LIGHT_TIME_MAX 1000
/* Time in ms */
#define EXPOSURE_TIME_MIN 50
#define EXPOSURE_TIME_MAX 3000
/* Time in ms */
#define TIME_BW_PICTURES_MIN 50
#define TIME_BW_PICTURES_MAX 9999
    
/* Value used to check if the EEPROM is already writent by this code */
#define CONTROL_VALUE 0x11223344

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
	/* Application's state machine's initial state. */
	APP_STATE_INIT=0,
	APP_STATE_SERVICE_TASKS,
    APP_STATE_SERVICE_CAPTURE,
    APP_STATE_WAIT,
} APP_STATES;

typedef enum{
    
    SYS_STATE_MENU = 0,
    SYS_STATE_MANUAL,
    SYS_STATE_AUTO
} SYSTEM_STATES;



// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef enum{
    
    ALL_LED_DISABLE = 0,
    PWR_LED1,
    PWR_LED2,
    PWR_LED3,
    PWR_LED4,
    PWR_LED5,
            
}LED_ID;


typedef struct
{
    /* The application's current state */
    APP_STATES appState;
    SYSTEM_STATES systemState;
    LED_ID ledId;
    uint32_t msCounter;
    
    /* LED config */
    uint16_t lightIntensity;
    uint16_t timeBetweenPictures;
    uint16_t exposureDuration;
    
    /* Auto mode param */
    uint8_t angleBwEachSeq;
    
    uint32_t seqClock1_ms;
    uint32_t seqClock2_ms;
    bool isFiveShotsSeqEnable;
    bool isFullImaginSeqEnable;
    
    uint16_t backLightIntensitiy;
    
} APP_DATA;
    
typedef struct
{
    bool state[4];
    bool isPressed;
    
} SW;

typedef struct{
    
    /* Motor data */
    int16_t     stepPerSec;
    uint16_t    stepPerTurn;
    uint16_t    gearValue;
    float       anglePerStep;
    
    /* LEDs data */
    uint16_t lightIntensity;
    uint16_t timeBetweenPictures;
    uint16_t exposureDuration;
    
    uint16_t backLightIntensitiy;
    
    /* Security value */
    uint32_t controlValue;
        
    } DATA_IN_EEPROM;
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/
	
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the 
    application in its initial state and prepares it to run so that its 
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks( void );
void APP_Delay_ms(uint32_t ms);

void setBlIntensity(int32_t *backLightIntensitiy);
int32_t getBlIntensity(void);
void setLightTime(int32_t *lightTime);
int32_t getLightTime(void);
void setExposureTime(int32_t *exposureTime);
int32_t getExposureTime(void);
void setTimeBwPictures(int32_t *timeBwPictures);
int32_t getTimeBwPictures(void);

void startSimpleShotSequence(LED_ID ledId);
void simpleShotProcess(void);
void fiveShotsSeqProcess(void);
void startFullImagingSequence(void);

void scanSwitch(void);
bool getSwitchEvent(void);

void turnOffAllPwrLeds(void);


#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

