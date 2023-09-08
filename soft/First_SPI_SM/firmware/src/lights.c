#include "app.h"

extern APP_DATA appData;

//----------------------------------------------------------------------------// lightManagementProcess
void sequenceManagementProcess(void){
    
    static int32_t order = 5; //= angleDesired / gear;
    
    if(appData.isFiveShotsSeqEnable){
        
        /* Sequence of 5 pictures is enable */
        //fiveShotsSeqProcess();
        //startFiveShotsSeqProcess();
    }
    if(appData.isFullImaginSeqEnable){
        
        /* Full sequence is enable */
        switch (appData.valSeq){
            
            case 0:
//                appData.valSeq += fiveShotsSeqProcess();
                break;
            case 1:
                setRotationToDo(getMyStepperStruct(), &order);
                if(getPerformedSteps(getMyStepperStruct()) == order){
                    order += 5; // appData.angleBwEachSeq;
                    appData.valSeq = 0;
                    appData.seqClock1_ms = 0;
                    appData.seqClock2_ms = 0;
                    startFiveShotsSeqProcess();
                }
                break;
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
    
    appData.seqClock1_ms = 0;
    appData.seqClock2_ms = 0;
    appData.isFiveShotsSeqEnable = true;
}

//----------------------------------------------------------------------------// startFullImagingSequence
void startFullImagingSequence(void){
    
    appData.seqClock1_ms = 0;
    appData.seqClock2_ms = 0;
    appData.isFullImaginSeqEnable = true;
    appData.valSeq = 0;
    appData.nbrOfShotsPerformed = 0;
    startFiveShotsSeqProcess();
}

//----------------------------------------------------------------------------// simpleShotProcess
void startSimpleShotProcess(void){
    
    appData.seqClock2_ms = 0;
    DRV_TMR4_Start();
}

void startFiveShotsSeqProcess(void){
    
    appData.seqClock1_ms = 0;
    DRV_TMR0_Start();
}

//----------------------------------------------------------------------------// imagingSeqProcess
/* This function takes 5 pictures with 5 different LEDs */
bool fiveShotsSeqProcess(void){
    
//    if(appData.seqClock1_ms == 0){
//        appData.ledId = PWR_LED1;
//        startSimpleShotProcess();
//        
//    } else if(appData.seqClock1_ms == 1 * appData.timeBetweenPictures){
//        appData.ledId = PWR_LED2;
//        startSimpleShotProcess();
//        
//    } else if(appData.seqClock1_ms == 2 * appData.timeBetweenPictures){
//        appData.ledId = PWR_LED3;
//        startSimpleShotProcess();
//        
//    } else if(appData.seqClock1_ms == 3 * appData.timeBetweenPictures){
//        appData.ledId = PWR_LED4;
//        startSimpleShotProcess();
//        
//    } else if(appData.seqClock1_ms == 4 * appData.timeBetweenPictures){
//        appData.ledId = PWR_LED5;
//        startSimpleShotProcess();
//    }
//    if(appData.seqClock1_ms >= 5 * appData.timeBetweenPictures){
//        
//        appData.seqClock1_ms = 0;
//        appData.seqClock2_ms = 0;
//        appData.isFiveShotsSeqEnable = false;
//        return 1;
//    }
//    return 0;
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
    
    int32_t time_bw_pictures_min = appData.exposureDuration + 
            3 * MARGIN_LED_DELAY;
    // Limit values to avoid problems
    if(*timeBwPictures < time_bw_pictures_min) *timeBwPictures 
            = time_bw_pictures_min;
    if(*timeBwPictures > TIME_BW_PICTURES_MAX) *timeBwPictures
            = TIME_BW_PICTURES_MAX;
    
    appData.timeBetweenPictures = *timeBwPictures;
}
int32_t getTimeBwPictures(void){
    
    return appData.timeBetweenPictures;
}

//----------------------------------------------------------------------------// setExposureTime
void setExposureTime(int32_t *exposureTime){
   
    // Limit values to avoid problems
    if(*exposureTime < EXPOSURE_TIME_MIN) *exposureTime = EXPOSURE_TIME_MIN;
    if(*exposureTime > EXPOSURE_TIME_MAX) *exposureTime = EXPOSURE_TIME_MAX;
    
    appData.exposureDuration = *exposureTime;
}
int32_t getExposureTime(void){
    
    return appData.exposureDuration;
}