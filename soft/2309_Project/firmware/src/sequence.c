/* 
 * File:   sequence.c
 * Author: ricch
 *
 * Created on September 5, 2023, 7:15 PM
 */

#include "app.h"

extern APP_DATA appData;

//----------------------------------------------------------------------------// lightManagementProcess
void sequenceManagementProcess(void){
    
    static int32_t order = 0;
    
    //---------------------------------------------// Auto mode active
    if(appData.isFullImagingSeqEnable){
        
        /* System is indexed */
        if(appData.indexState == IS_DONE){
            
            if(appData.isSeqFirstPass){

                appData.isSeqFirstPass = false;
                /* Set the first angle to reach */
                order = appData.angleBwEachSeq;
                appData.nbrOfShotsPerformed = 0;
                appData.sequenceState = SEQ_STATE_START_IMAGING;
            }

            /* Sequence state machine */
            switch (appData.sequenceState){

                case SEQ_STATE_STANDBY:
                    /* Wait next commands */
                    break;
                    
                case SEQ_STATE_START_IMAGING:
                    startFiveShotsSeqProcess();
                    /* Update sequence state machine */
                    appData.sequenceState = SEQ_STATE_BUSY_IMAGING;
                    break;
                    
                case SEQ_STATE_START_MOTOR_ROT:
                    /* Give order to the stepper */
                    setAngleToReach(getMyStepperStruct(), &order);
                    
                    
                    if(order > (355)){ // <<-----------------------  360?????
                        appData.isFullImagingSeqEnable = false;
                        /* Update sequence state machine */
                        appData.sequenceState = SEQ_STATE_STANDBY;
                        /* Return home */
                        startAutoHome(getMyStepperStruct());
                        order = 0;
//                        isFirstPass = true;
                    } else {
                        order += appData.angleBwEachSeq;
                        /* Update sequence state machine */
                        appData.sequenceState = SEQ_STATE_BUSY_ROTATION;
                    }
                    break;
                    
                case SEQ_STATE_BUSY_IMAGING:
                    if(appData.isSequenceEnded == true){
                        
                        appData.isSequenceEnded = false;
                        /* Update sequence state machine */
//                        appData.sequenceState = SEQ_STATE_START_MOTOR_ROT;
                    }
                    break;
                    
                case SEQ_STATE_BUSY_ROTATION:
                    /* When the stepper reach the given angle */
                    if(getPerformedSteps(getMyStepperStruct()) 
                            == getStepToReach(getMyStepperStruct())){
                        
                        /* Update sequence state machine */
                        appData.sequenceState = SEQ_STATE_START_IMAGING;
                    }
                    break;
            }
        }
        
        
        /* System is not indexed */
        if(appData.indexState == NOT_DONE){
            
            /* Start auto-home process */
            startAutoHome(getMyStepperStruct());
            appData.indexState = IN_PROCESS;
        }
    }
}













//----------------------------------------------------------------------------// turnOffAllPwrLeds
void turnOffAllPwrLeds(void){
    
    /* Turn off all power LED */
    LED1_CMDOff();
    LED2_CMDOff();
    LED3_CMDOff();
    LED4_CMDOff();
    LED5_CMDOff();
}

//----------------------------------------------------------------------------// startFiveShotsSequence
/* Start a sequence for 5 shots */
void startFiveShotsSequence(void){
    
    appData.isFiveShotsSeqEnable = true;
}

//----------------------------------------------------------------------------// startStopFullImagingSequence
void startStopFullImagingSequence(void){
    
    if(appData.isFullImagingSeqEnable == false){
        
        appData.isFullImagingSeqEnable  = true;
        appData.isSeqFirstPass          = true;
        appData.nbrOfShotsPerformed     = 0;
        appData.indexState              = NOT_DONE;
    } else {
        
        appData.isFullImagingSeqEnable  = false;
//        turnOffAllPwrLeds();
        stopImagingProcess();
    }
}

//----------------------------------------------------------------------------// simpleShotProcess
void startSimpleShotProcess(void){
    
    appData.seqClock2_ms = 0;
    DRV_TMR4_Start();
}

//----------------------------------------------------------------------------// startFiveShotsSeqProcess
void startFiveShotsSeqProcess(void){
    
    appData.seqClock1_ms = 0;
    DRV_TMR0_Start();
}

//----------------------------------------------------------------------------// stopImagingProcess
void stopImagingProcess(void){
    
//    appData.isFullImagingSeqEnable = false;
    DRV_TMR0_Stop();
    DRV_TMR4_Start();
    appData.seqClock1_ms = 0;
    appData.seqClock2_ms = 0;
}

//----------------------------------------------------------------------------// startFocusLighting
void startFocusLighting(void){
    
    LED1_CMDOn();
    APP_Delay_ms(1000 * appData.focusDuration);
    turnOffAllPwrLeds();
}

//----------------------------------------------------------------------------// setLighIntensity
void setFocusDuration(int32_t *focusDuration){
    
    // Limit values to avoid problems
    if(*focusDuration < FOCUS_DURATION_MIN) *focusDuration = FOCUS_DURATION_MIN;
    if(*focusDuration > FOCUS_DURATION_MAX) *focusDuration = FOCUS_DURATION_MAX;
    
    appData.focusDuration = *focusDuration;
}
int32_t getFocusDuration(void){
    
    return appData.focusDuration;
}

//----------------------------------------------------------------------------// setLighIntensity
void setLightIntensity(int32_t *lightIntensity){
    
    // Limit values to avoid problems
    if(*lightIntensity < LIGHT_INTENSITY_MIN) *lightIntensity 
            = LIGHT_INTENSITY_MIN;
    if(*lightIntensity > LIGHT_INTENSITY_MAX) *lightIntensity 
            = LIGHT_INTENSITY_MAX;
    
    /* 25 = 2500 / 100 */
    appData.lightIntensity = *lightIntensity * 25;
    PLIB_MCPWM_ChannelPrimaryDutyCycleSet(MCPWM_ID_0, PWM_DIM_CH, appData.lightIntensity);
}
int32_t getLightIntensity(void){
    
    return appData.lightIntensity / 25;
}

//----------------------------------------------------------------------------// setTimeBwPictures
void setTimeBwPictures(int32_t *timeBwPictures){
    
    /* Adaptation for incrementing and decrementing 10 by 10 */
    *timeBwPictures = *timeBwPictures * 10;
    
    int32_t time_bw_pictures_min = appData.exposureDuration + 
            3 * MARGIN_LED_DELAY;
    // Limit values to avoid problems
    if(*timeBwPictures < time_bw_pictures_min) *timeBwPictures 
            = time_bw_pictures_min;
    if(*timeBwPictures > TIME_BW_PICTURES_MAX) *timeBwPictures
            = TIME_BW_PICTURES_MAX;
    
    appData.timeBetweenPictures = *timeBwPictures;
    
    /* Adaptation for incrementing and decrementing 10 by 10 */
    *timeBwPictures = *timeBwPictures / 10;
}
int32_t getTimeBwPictures(void){
    
    return (appData.timeBetweenPictures / 10);
}
//----------------------------------------------------------------------------// setTimeBwPictures
void adaptTimeBwPictures(void){
    
    int32_t time_bw_pictures_min = appData.exposureDuration + 
            3 * MARGIN_LED_DELAY;
    // Limit values to avoid problems
    if(appData.timeBetweenPictures < time_bw_pictures_min) 
        appData.timeBetweenPictures = time_bw_pictures_min;
    if(appData.timeBetweenPictures > TIME_BW_PICTURES_MAX) 
        appData.timeBetweenPictures = TIME_BW_PICTURES_MAX;
}

//----------------------------------------------------------------------------// setExposureTime
void setExposureTime(int32_t *exposureTime){
   
    /* Adaptation for incrementing and decrementing 10 by 10 */
    *exposureTime = *exposureTime * 10;
            
    // Limit values to avoid problems
    if(*exposureTime < EXPOSURE_TIME_MIN) *exposureTime = EXPOSURE_TIME_MIN;
    if(*exposureTime > EXPOSURE_TIME_MAX) *exposureTime = EXPOSURE_TIME_MAX;
    
    appData.exposureDuration = *exposureTime;
    
    /* Adaptation for incrementing and decrementing 10 by 10 */
    *exposureTime = *exposureTime / 10;
    
    /* Adapt the time between pictures */
    adaptTimeBwPictures();
}
int32_t getExposureTime(void){
    
    return appData.exposureDuration / 10;
}






void Timer0Id1_CallbackFunction(void){
    
    /* Control sequence of the 5 LEDs */
    if(appData.seqClock1_ms == 0){
        appData.ledId = PWR_LED1;
        startSimpleShotProcess();
        
    } else if(appData.seqClock1_ms == 1 * appData.timeBetweenPictures){
        appData.ledId = PWR_LED2;
        startSimpleShotProcess();
        
    } else if(appData.seqClock1_ms == 2 * appData.timeBetweenPictures){
        appData.ledId = PWR_LED3;
        startSimpleShotProcess();
        
    } else if(appData.seqClock1_ms == 3 * appData.timeBetweenPictures){
        appData.ledId = PWR_LED4;
        startSimpleShotProcess();
        
    } else if(appData.seqClock1_ms == 4 * appData.timeBetweenPictures){
        appData.ledId = PWR_LED5;
        startSimpleShotProcess();
    }
    /* End of the sequence of 5 shots */
    if(appData.seqClock1_ms >= 5 * appData.timeBetweenPictures){
        
        DRV_TMR0_Stop();
        appData.seqClock1_ms = 0;
        appData.isFiveShotsSeqEnable = false;
        appData.sequenceState = 1;
        appData.isSequenceEnded = true;
        /* Update sequence state machine */
        appData.sequenceState = SEQ_STATE_START_MOTOR_ROT;
    } else {
    appData.seqClock1_ms++;
    } 
}

void Timer4Id5_CallbackFunction(void){
    
    //------------------------------------------------------------------------// Start of sequence
    if(appData.seqClock2_ms == 0){

        switch (appData.ledId){
            /* Turn on LED */
            case PWR_LED1:
                turnOffAllPwrLeds();
                LED1_CMDOn();
                break;

            case PWR_LED2:
                turnOffAllPwrLeds();
                LED2_CMDOn();
                break;

            case PWR_LED3:
                turnOffAllPwrLeds();
                LED3_CMDOn();
                break;

            case PWR_LED4:
                turnOffAllPwrLeds();
                LED4_CMDOn();
                break;

            case PWR_LED5:
                turnOffAllPwrLeds();
                LED5_CMDOn();
                break;
        }
        FOCUS_CMDOn(); // <------------------------------------------------- TEST
    }
    if(appData.seqClock2_ms == MARGIN_LED_DELAY){
        
        /* Capture the target */
//        FOCUS_CMDOn(); // <------------------------------------------------- TEST
        TRIGGER_CMDOn();
        appData.nbrOfShotsPerformed++; // --------------------<<< CONDITION DANS SM DIFFERENTE DE NBR CAPTURES
//        SIGN_LED_CMDOn();
    }
    if(appData.seqClock2_ms == appData.exposureDuration + MARGIN_LED_DELAY){

        TRIGGER_CMDOff();
        FOCUS_CMDOff();
    }
    //------------------------------------------------------------------------// End of sequence
    if(appData.seqClock2_ms >= appData.exposureDuration + (2 * MARGIN_LED_DELAY)){
        
        /* Turn off TMR4 */
        DRV_TMR4_Stop();
        turnOffAllPwrLeds();
        appData.seqClock2_ms = 0;
        appData.ledId = ALL_LED_DISABLE;
    } else {
        appData.seqClock2_ms++;
    }
}