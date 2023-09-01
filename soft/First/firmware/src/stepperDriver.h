/* 
 * File:   stepperDriver.h
 * Author: ricch
 *
 * Created on August 30, 2023, 10:39 PM
 */

#ifndef STEPPERDRIVER_H
#define	STEPPERDRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <stdint.h>
    #include "system_definitions.h"

    // Defines
    #define STEP_PER_SEC_MIN 40
    #define STEP_PER_SEC_MAX 50000

    #define GEAR_VALUE_MIN 1
    #define GEAR_VALUE_MAX 1000

    #define STEP_PER_TURN_MIN 4
    #define STEP_PER_TURN_MAX 400

    #define ANGLE_PER_STEP_MIN 0.1
    #define ANGLE_PER_STEP_MAX 10.0
    
    #define ROTATION_TO_DO_MIN 0
    #define ROTATION_TO_DO_MAX 50000

    // Structures
    typedef struct{

        // Motor real time data
        bool isCW;
        int16_t stepPerSec;
        uint32_t performedStep;
        int32_t stepToDo;
        float motorAngle;
        float realAngle;

        // Motor characteristics
        float anglePerStep;
        uint16_t nbrFullTurn;
        uint16_t stepPerTurn;
        uint16_t gearValue;
        uint32_t motorStepNumber;

    } STEPPER_DATA;



    // Prototypes
    void turnOffStepperPwms(void);
    void changeSpeed(STEPPER_DATA *pStepperData);
    //void processStepper(void);
    void processStepper(STEPPER_DATA *pStepperData);


#ifdef	__cplusplus
}
#endif

#endif	/* STEPPERDRIVER_H */

