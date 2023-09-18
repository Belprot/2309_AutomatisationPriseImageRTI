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
    #define STEP_PER_SEC_MIN 0
    #define STEP_PER_SEC_MAX 1000

    #define GEAR_VALUE_MIN 1
    #define GEAR_VALUE_MAX 1000

    #define STEP_PER_TURN_MIN 4
    #define STEP_PER_TURN_MAX 400

    #define ANGLE_PER_STEP_MIN 0.1
    #define ANGLE_PER_STEP_MAX 10.0
    
    #define ROTATION_TO_DO_MIN -50000
    #define ROTATION_TO_DO_MAX 50000

    #define ANGLE_TO_DO_REACH_MIN -1000
    #define ANGLE_TO_DO_REACH_MAX 1000
    
    /* Period for 50kHz PWMs */
    #define MCPWM_PRIMARY_PERIOD 199
    #define MCPWM_DUTYCYCLE_MIN 0
    #define MCPWM_DUTYCYCLE_MAX 189
    
    // Structures
    typedef struct{

        /* Motion motor data */
        bool        isAtHomeInCW;
        bool        isAtHomeInCCW;
        bool        isIndexed;
        bool        isInAutoHomeSeq;
        
        int32_t     performedSteps;
        int32_t     stepToReach;

        /* Motor characteristics */
        int16_t     stepPerSec;
        
        uint16_t    stepPerTurn;
        uint16_t    gearValue;
        
        float       anglePerStep;
        
        uint16_t    dutyCycleStepper;
        
    } STEPPER_DATA;



    // Prototypes
    void initStepperParam(void);
    void turnOffStepperPwms(void);
    void changeSpeed(STEPPER_DATA *pStepperData);
    void processStepper(STEPPER_DATA *pStepperData);
   
    
    void setSpeed(STEPPER_DATA *pStepperData, uint32_t *pStepPerSec);
    int32_t getSpeed(STEPPER_DATA *pStepperData);
    void setGearReduction(STEPPER_DATA *pStepperData, uint32_t *pGearValue);
    uint32_t getGearReduction(STEPPER_DATA *pStepperData);
    void setAnglePerStep(STEPPER_DATA *pStepperData, uint32_t *pAnglePerStep);
    uint32_t getAnglePerStep(STEPPER_DATA *pStepperData);
    int32_t getPerformedSteps(STEPPER_DATA *pStepperData);
    int32_t getStepToReach(STEPPER_DATA *pStepperData);
    void setRotationToDo(STEPPER_DATA *pStepperData, int32_t *pRotationToDo);
    int32_t getRotationToDo(STEPPER_DATA *pStepperData);
    void startAutoHome(STEPPER_DATA *pStepperData);
    
    void setStepperPower(STEPPER_DATA *pStepperData, uint16_t *pDutyCycleStepper);
    int16_t getStepperPower(STEPPER_DATA *pStepperData);
    
    STEPPER_DATA* getMyStepperStruct(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* STEPPERDRIVER_H */

