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

typedef struct{

    bool isCW;
    float motorAngle;
    float realAngle;
    float degreePerStep;
    uint16_t stepPerSec;
    uint16_t nbrFullTurn;
    uint16_t stepPerTurn;
    uint16_t gearValue;
    uint32_t motorStepNumber;
    uint32_t performedStep;
    uint32_t stepToDo;

} STEPPER_DATA;

void turnOffStepperPwms(void);
void changeSpeed(STEPPER_DATA *pStepperData);
//void processStepper(void);
void processStepper(STEPPER_DATA *pStepperData);


#ifdef	__cplusplus
}
#endif

#endif	/* STEPPERDRIVER_H */

