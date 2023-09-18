/* 
 * File:   stepperDriver.c
 * Author: ricch
 *
 * Created on August 30, 2023, 10:39 PM
 * 
 * DRV8432 driver 2H bridge
 */


#include <stepperDriver.h>

static STEPPER_DATA stepperData;
extern APP_DATA appData;


//----------------------------------------------------------------------------// initStepperData
void initStepperParam(void){
    
    stepperData.isAtHomeInCW     = false;
    stepperData.isAtHomeInCCW    = false;
//    stepperData.isIndexed        = false;
    stepperData.isInAutoHomeSeq  = false;
    
    stepperData.performedSteps   = 0; 
    stepperData.stepToReach      = 0; 
    
    stepperData.stepPerSec       = 50; /* speed */
    
    stepperData.stepPerTurn      = 200; /* Data from igus 
                                         * MOT-AN-S-060-005-042-L-A-AAAA */
    stepperData.anglePerStep     = 1.8; /* " */
    
    stepperData.gearValue        = 1; /* 260 -> Data from meca student */
    
    stepperData.dutyCycleStepper = 10; /* Min 0 - max 199 */ 
}

void initStepperMotor(){
    
    changeSpeed(getMyStepperStruct());
    setStepperPower(getMyStepperStruct(), &stepperData.dutyCycleStepper); // -< param ??!! faux
    /* Disable RESET on both H bridge */
    RESET_AB_CMDOn();
    RESET_CD_CMDOn();
}

//----------------------------------------------------------------------------// turnOffStepperPwms
/* Disable all PWMs for motor control */
void turnOffStepperPwms(void){
    
    /* A */
    PLIB_MCPWM_ChannelPWMxHDisable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
    /* B */
    PLIB_MCPWM_ChannelPWMxHDisable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
    /* A_ */
    PLIB_MCPWM_ChannelPWMxLDisable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
    /* B_ */
    PLIB_MCPWM_ChannelPWMxLDisable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
}

//----------------------------------------------------------------------------// changeSpeed
void changeSpeed(STEPPER_DATA *pStepperData){
    
    uint16_t tmr2Id3Perdiod = 0;
    uint16_t frequency = 0;
    
    frequency = pStepperData->stepPerSec;
    tmr2Id3Perdiod = SYS_CLK / (frequency * 16) - 1; /* PRESCALER = 16 */
    PLIB_TMR_Counter16BitClear(TMR_ID_3);
    PLIB_TMR_Period16BitSet(TMR_ID_3, tmr2Id3Perdiod);
}

//----------------------------------------------------------------------------// processStepper
void processStepper(STEPPER_DATA *pStepperData){
    
    static uint8_t step = 0;
    //---------------------------// Counter clockwise CCW
    if(pStepperData->performedSteps > pStepperData->stepToReach){
        if(pStepperData->isAtHomeInCCW == false){
            switch(step){
                /* Sequence of 4 steps for CCW rotation */
                case 1:
                    /* A */
                    PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    /* B */
                    PLIB_MCPWM_ChannelPWMxHDisable(MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    /* A_ */
                    PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    /* B_ */
                    PLIB_MCPWM_ChannelPWMxLEnable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    break;

                case 2:
                    PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    break;

                case 3:
                    PLIB_MCPWM_ChannelPWMxHDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    PLIB_MCPWM_ChannelPWMxLEnable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    break;

                case 0:
                    PLIB_MCPWM_ChannelPWMxHDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    PLIB_MCPWM_ChannelPWMxHDisable(MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    PLIB_MCPWM_ChannelPWMxLEnable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    PLIB_MCPWM_ChannelPWMxLEnable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    break;
            }
            step++;
        }
        /* Four steps performed in CCW */
        if(step == 4){
            
            step = 0;
            pStepperData->performedSteps -= 4;
        }
    }
        
    /* Index is reach in CCW */
    if(INDEXStateGet() && pStepperData->isAtHomeInCW == false){

        pStepperData->isAtHomeInCCW = true;

        if(pStepperData->isInAutoHomeSeq == true){

            /* Set the values to zero */
            pStepperData->stepToReach = 0;
            pStepperData->performedSteps = 0;
            /* Reset the flag isInAutoHomeSeq */
            pStepperData->isInAutoHomeSeq = false;

            appData.indexState = IS_DONE;
        }
    } else {
        pStepperData->isAtHomeInCCW = false;
    }    
    
    
    
    //---------------------------// Clockwise CW
    if(pStepperData->performedSteps < pStepperData->stepToReach){
        if(pStepperData->isAtHomeInCW == false){
            switch(step){
                /* Sequence of 4 steps for CW rotation */
                case 1:
                    /* A */
                    PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    /* B */
                    PLIB_MCPWM_ChannelPWMxHDisable(MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    /* A_ */
                    PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    /* B_ */
                    PLIB_MCPWM_ChannelPWMxLEnable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    break;

                case 0:
                    PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    break;

                case 3:
                    PLIB_MCPWM_ChannelPWMxHDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    PLIB_MCPWM_ChannelPWMxLEnable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    break;

                case 2:
                    PLIB_MCPWM_ChannelPWMxHDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    PLIB_MCPWM_ChannelPWMxHDisable(MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    PLIB_MCPWM_ChannelPWMxLEnable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
                    PLIB_MCPWM_ChannelPWMxLEnable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
                    break;
            }
            step++;
        }
        /* Four steps performed in CW */
        if(step == 4){
            
            step = 0;
            pStepperData->performedSteps += 4;
        }
    }
    /* Index is reach in CW */
    if(INDEXStateGet() && pStepperData->isAtHomeInCCW == false){

        pStepperData->isAtHomeInCW = true;
        /* Stop the motor */
        pStepperData->stepToReach = pStepperData->performedSteps;
    }
    else pStepperData->isAtHomeInCW = false;
}



//----------------------------------------------------------------------------// setSpeed
void setSpeed(STEPPER_DATA *pStepperData, uint32_t *pStepPerSec){
    
    // Limit values to avoid problems
    if(*pStepPerSec < STEP_PER_SEC_MIN) *pStepPerSec = STEP_PER_SEC_MIN;
    if(*pStepPerSec > STEP_PER_SEC_MAX) *pStepPerSec = STEP_PER_SEC_MAX;
    
    // Save data
    pStepperData->stepPerSec = *pStepPerSec;
}

int32_t getSpeed(STEPPER_DATA *pStepperData){
    
    return pStepperData->stepPerSec;
}

//----------------------------------------------------------------------------// setGearReduction
void setGearReduction(STEPPER_DATA *pStepperData, uint32_t *pGearValue){
    
    // Limit values to avoid problems
    if(*pGearValue < GEAR_VALUE_MIN) *pGearValue = GEAR_VALUE_MIN;
    if(*pGearValue > GEAR_VALUE_MAX) *pGearValue = GEAR_VALUE_MAX;
    
    // Save data
    pStepperData->gearValue = *pGearValue;
}
//----------------------------------------------------------------------------// getGearReduction
uint32_t getGearReduction(STEPPER_DATA *pStepperData){
    
    return pStepperData->gearValue;
}

//----------------------------------------------------------------------------// setAnglePerStep
void setAnglePerStep(STEPPER_DATA *pStepperData, uint32_t *pAnglePerStep){
    
    float temp = (*pAnglePerStep / 10.0);
    
    // Limit values to avoid problems
    if(temp < ANGLE_PER_STEP_MIN) temp = (ANGLE_PER_STEP_MIN);
    if(temp > ANGLE_PER_STEP_MAX) temp = (ANGLE_PER_STEP_MAX);
    *pAnglePerStep = temp * 10;
    
    // Save data
    pStepperData->anglePerStep = temp;
}
//----------------------------------------------------------------------------// getAnglePerStep
uint32_t getAnglePerStep(STEPPER_DATA *pStepperData){
    
    // x10 ???
    return pStepperData->anglePerStep * 10;
}

//----------------------------------------------------------------------------// getPerformedSteps
int32_t getPerformedSteps(STEPPER_DATA *pStepperData){
    
    return pStepperData->performedSteps;
}

//----------------------------------------------------------------------------// getStepToReach
int32_t getStepToReach(STEPPER_DATA *pStepperData){
    
    return pStepperData->stepToReach;
}

//----------------------------------------------------------------------------// setRotationToDo
void setRotationToDo(STEPPER_DATA *pStepperData, int32_t *pRotationToDo){
    
    // Limit values to avoid problems
    if(*pRotationToDo < ROTATION_TO_DO_MIN) *pRotationToDo = ROTATION_TO_DO_MIN;
    if(*pRotationToDo > ROTATION_TO_DO_MAX) *pRotationToDo = ROTATION_TO_DO_MAX;
    
    // Save data
    pStepperData->stepToReach = *pRotationToDo * pStepperData->stepPerTurn;
}
//----------------------------------------------------------------------------// getRotationTodo
int32_t getRotationToDo(STEPPER_DATA *pStepperData){
    
    return pStepperData->stepToReach / pStepperData->stepPerTurn;
}

//----------------------------------------------------------------------------// setRotationToDo
void setAngleToReach(STEPPER_DATA *pStepperData, int32_t *pAngleToReach){
    
    // Limit values to avoid problems
    if(*pAngleToReach < ANGLE_TO_DO_REACH_MIN) *pAngleToReach 
            = ANGLE_TO_DO_REACH_MIN;
    if(*pAngleToReach > ANGLE_TO_DO_REACH_MAX) *pAngleToReach 
            = ANGLE_TO_DO_REACH_MAX;
    
    float multiplier = 1.0 / (pStepperData->anglePerStep 
    / pStepperData->gearValue);

    int32_t result = (float)(*pAngleToReach * multiplier); // 
    // Calculate the adjustment needed to make the result a multiple of 4.
    int32_t adjustment = result % 4;
    /* If the adjustment is not zero, increase the result to the next multiple 
     * of 4. */
    if (adjustment != 0) {
        result += 4 - adjustment;
    }
    // Store the adjusted result
    pStepperData->stepToReach = result;
}
//----------------------------------------------------------------------------// getRotationTodo
//int32_t getAngleToReach(STEPPER_DATA *pStepperData){
//    
//    return pStepperData->stepToReach / pStepperData->stepPerTurn;
//}

//----------------------------------------------------------------------------// autoHome
void startAutoHome(STEPPER_DATA *pStepperData){
    
    pStepperData->isInAutoHomeSeq = true;
    // Check if the arm is not at home
    if(pStepperData->isAtHomeInCCW == false){
        // Put steps to do for returning home in CCW
        pStepperData->stepToReach = -50000; // DEFINE? STEP_TO_DO_MAX
    }
}

//----------------------------------------------------------------------------// setStepperPower
void setStepperPower(STEPPER_DATA *pStepperData, uint16_t *pDutyCycleStepper){
    
    uint16_t dutyValCh1 = 0;
    
    // Limit values to avoid problems
    if(*pDutyCycleStepper < MCPWM_DUTYCYCLE_MIN) *pDutyCycleStepper 
            = MCPWM_DUTYCYCLE_MIN;
    if(*pDutyCycleStepper > MCPWM_DUTYCYCLE_MAX) *pDutyCycleStepper 
            = MCPWM_DUTYCYCLE_MAX;

    /* Save configuration in the structure */
    pStepperData->dutyCycleStepper = *pDutyCycleStepper;
    
    /* Must be the inverse of the CHANNEL 1 */
    dutyValCh1 = MCPWM_PRIMARY_PERIOD - *pDutyCycleStepper;
    
    PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0 ,MCPWM_CHANNEL1,
            dutyValCh1);
    PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0 ,MCPWM_CHANNEL2, 
            *pDutyCycleStepper);
}

int16_t getStepperPower(STEPPER_DATA *pStepperData){
    
    return pStepperData->dutyCycleStepper;
}


//----------------------------------------------------------------------------// getStepperStruct
STEPPER_DATA* getMyStepperStruct(void){
    
    /* Return the address of the structure */
    return &stepperData;
}