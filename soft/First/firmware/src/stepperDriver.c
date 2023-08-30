
#include <stepperDriver.h>
#include <stdint.h>
#include "system_definitions.h"

void processStepper(void){
    
    static uint8_t counter = 0;
    
    
    switch(counter){
        
        case 0:
            PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
            PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
            PLIB_MCPWM_ChannelPWMxHDisable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
            PLIB_MCPWM_ChannelPWMxLDisable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
            break;
        
        case 1:
            PLIB_MCPWM_ChannelPWMxHDisable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
            PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
            PLIB_MCPWM_ChannelPWMxHEnable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
            PLIB_MCPWM_ChannelPWMxLDisable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
            break;
            
        case 2:
            PLIB_MCPWM_ChannelPWMxHDisable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
            PLIB_MCPWM_ChannelPWMxLEnable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
            PLIB_MCPWM_ChannelPWMxHDisable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
            PLIB_MCPWM_ChannelPWMxLDisable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
            break;

        case 3:
            PLIB_MCPWM_ChannelPWMxHDisable (MCPWM_ID_0 ,MCPWM_CHANNEL1);
            PLIB_MCPWM_ChannelPWMxLDisable(MCPWM_ID_0 ,MCPWM_CHANNEL1);
            PLIB_MCPWM_ChannelPWMxHDisable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
            PLIB_MCPWM_ChannelPWMxLEnable (MCPWM_ID_0 ,MCPWM_CHANNEL2);
            break;
    }

    counter++;
    if(counter == 4) counter = 0;
}
