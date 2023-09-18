/* 
 * File:   sequence.h
 * Author: ricch
 *
 * Created on September 5, 2023, 7:16 PM
 */

#ifndef LIGHTS_H
#define	LIGHTS_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* Intensity in percent */
    #define LIGHT_INTENSITY_MIN 0
    #define LIGHT_INTENSITY_MAX 100
    /* Time in ms */
    #define LIGHT_TIME_MIN 50
    #define LIGHT_TIME_MAX 3000
    /* Time in ms */
    #define EXPOSURE_TIME_MIN 50
    #define EXPOSURE_TIME_MAX 3000
    /* Time in ms */
    #define TIME_BW_PICTURES_MAX 5000
    /* Time in ms */
    #define MARGIN_LED_DELAY 200
    
    void sequenceManagementProcess(void);
    
    void turnOffAllPwrLeds(void);
    
    
    void startFiveShotsSequence(void);
    void startStopFullImagingSequence(void);
    
    void startSimpleShotProcess(void);
    void startFiveShotsSeqProcess(void);
    void stopImagingProcess(void);
    
    void setLightIntensity(int32_t *lightIntensity);
    int32_t getLightIntensity(void);
    
    void setTimeBwPictures(int32_t *timeBwPictures);
    int32_t getTimeBwPictures(void);
    void adaptTimeBwPictures(void);
    
    void setExposureTime(int32_t *exposureTime);
    int32_t getExposureTime(void);

    
    
    void Timer0Id1_CallbackFunction(void);
    void Timer4Id5_CallbackFunction(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LIGHTS_H */

