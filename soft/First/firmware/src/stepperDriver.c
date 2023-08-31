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
    
    frequency = 4 * pStepperData->stepPerSec;
    
    tmrPerdiod = SYS_CLK / (frequency * TMR_PRESCALE_VALUE_8);
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
