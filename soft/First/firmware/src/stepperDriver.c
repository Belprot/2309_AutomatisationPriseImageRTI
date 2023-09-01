/* 
 * File:   stepperDriver.c
 * Author: ricch
 *
 * Created on August 30, 2023, 10:39 PM
 * 
 * DRV8432 driver 2H bridge
 */


#include <stepperDriver.h>

void turnOffStepperPwms(void){
    
    PLIB_MCPWM_ChannelPWMxHDisable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
    PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
    PLIB_MCPWM_ChannelPWMxHDisable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
    PLIB_MCPWM_ChannelPWMxLDisable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
}

void changeSpeed(STEPPER_DATA *pStepperData){
    
    uint16_t tmrPerdiod = 0;
    uint16_t frequency = 0;
    //uint16_t presc = 0;
    
    
    frequency = pStepperData->stepPerSec;
    //presc = TMR_PrescaleGet_Default(TMR_ID_3);
    tmrPerdiod = SYS_CLK / (frequency * 16) - 1;
    PLIB_TMR_Counter16BitClear(TMR_ID_3);
    PLIB_TMR_Period16BitSet(TMR_ID_3, tmrPerdiod);
    
}

void processStepper(STEPPER_DATA *pStepperData){
    
    static uint8_t counter = 0;

    if(pStepperData->performedStep < pStepperData->stepToDo && 
            pStepperData->isCW == true){
        switch(counter){
            
            case 1:
                PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
                PLIB_MCPWM_ChannelPWMxHDisable(MCPWM_ID_0 ,MCPWM_CHANNEL2);
                PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
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
        counter++;
        if(counter == 4){
            
            counter = 0;
            pStepperData->performedStep += 4;
        }
    }
    else if(pStepperData->performedStep > pStepperData->stepToDo && 
            pStepperData->isCW == false){
        switch(counter){
            
            case 1:
                PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
                PLIB_MCPWM_ChannelPWMxHDisable(MCPWM_ID_0 ,MCPWM_CHANNEL2);
                PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
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
        counter++;
        if(counter == 4){
            
            counter = 0;
            pStepperData->performedStep -= 4;
        }
    }
    
    if(pStepperData->performedStep == pStepperData->stepToDo){
        
        int8_t i = 0;
        //turnOffStepperPwms();
    }
} 





//----------------------------------------------------------------------------//
void setSpeed(STEPPER_DATA *pStepperData, uint16_t *pStepPerSec){
    
    // Limit values to avoid problems
    if(*pStepPerSec < STEP_PER_SEC_MIN) *pStepPerSec = STEP_PER_SEC_MIN;
    if(*pStepPerSec > STEP_PER_SEC_MAX) *pStepPerSec = STEP_PER_SEC_MAX;
    
    // Save data
    pStepperData->stepPerSec = *pStepPerSec;
}

int16_t getSpeed(STEPPER_DATA *pStepperData){
    
    return pStepperData->stepPerSec;
}

//----------------------------------------------------------------------------//
void setGearReduction(STEPPER_DATA *pStepperData, uint16_t *pGearValue){
    
    // Limit values to avoid problems
    if(*pGearValue < GEAR_VALUE_MIN) *pGearValue = GEAR_VALUE_MIN;
    if(*pGearValue > GEAR_VALUE_MAX) *pGearValue = GEAR_VALUE_MAX;
    
    // Save data
    pStepperData->gearValue = *pGearValue;
}

uint16_t getGearReduction(STEPPER_DATA *pStepperData){
    
    return pStepperData->gearValue;
}

//----------------------------------------------------------------------------//
void setAnglePerStep(STEPPER_DATA *pStepperData, uint16_t *pAnglePerStep){
    
    float temp = (*pAnglePerStep / 10.0);
    
    // Limit values to avoid problems
    if(temp < ANGLE_PER_STEP_MIN) temp = (ANGLE_PER_STEP_MIN);
    if(temp > ANGLE_PER_STEP_MAX) temp = (ANGLE_PER_STEP_MAX);
    *pAnglePerStep = temp * 10;
    
    // Save data
    pStepperData->anglePerStep = temp;
}

uint16_t getAnglePerStep(STEPPER_DATA *pStepperData){
    
    // x10 ???
    return pStepperData->anglePerStep * 10;
}







//----------------------------------------------------------------------------//
void setRotationToDo(STEPPER_DATA *pStepperData, uint16_t *pRotationToDo){
    
    // Limit values to avoid problems
    if(*pRotationToDo < ROTATION_TO_DO_MIN) *pRotationToDo = ROTATION_TO_DO_MIN;
    if(*pRotationToDo > ROTATION_TO_DO_MAX) *pRotationToDo = ROTATION_TO_DO_MAX;
    
    // Save data
    pStepperData->stepToDo = *pRotationToDo * pStepperData->stepPerTurn;
}

uint16_t getRotationToDo(STEPPER_DATA *pStepperData){
    
    return pStepperData->stepToDo * pStepperData->stepPerTurn;
}


void returnToHome(void){
    
    
}
