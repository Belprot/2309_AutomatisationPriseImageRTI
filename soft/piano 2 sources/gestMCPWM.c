/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

// todo include le minimum
#include "app.h"
#include "gestMCPWM.h"
#include "driver/mcpwm/drv_mcpwm_static.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

// mets a jour le PWM applique a un electro-aimant
void MCPWM_Actualise_Velocity(uint8_t nPWM, uint8_t Velocity)
{
    uint16_t DutyCycle;
    
    DutyCycle = (Velocity * MaxMCPWM) / MaxVelocity;
    
    // nPWM ne correspond pas à MCPWM_CHANNEL
        // voir le schema pour le n° du channel de chaque nPWM
    // j'aurais pu créer un tableau de 12 cases avec chaque case un MCPWM_CHANNEL7
        // PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , TabChannel[nPWM] , DutyCycle);
    switch (nPWM)
    {
        case 0 :
        {
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , MCPWM_CHANNEL7 , DutyCycle);
            break;
        }
        case 1 :
        {
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , MCPWM_CHANNEL1 , DutyCycle);
            break;
        }
        case 2 :
        {
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , MCPWM_CHANNEL10 , DutyCycle);
            break;
        }
        case 3 :
        {
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , MCPWM_CHANNEL4 , DutyCycle);
            break;
        }
        case 4 :
        {
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , MCPWM_CHANNEL8 , DutyCycle);
            break;
        }
        case 5 :
        {
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , MCPWM_CHANNEL2 , DutyCycle);
            break;
        }
        case 6 :
        {
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , MCPWM_CHANNEL9 , DutyCycle);
            break;
        }
        case 7 :
        {
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , MCPWM_CHANNEL3 , DutyCycle);
            break;
        }
        case 8 :
        {
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , MCPWM_CHANNEL11 , DutyCycle);
            break;
        }
        case 9 :
        {
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , MCPWM_CHANNEL12 , DutyCycle);
            break;
        }
        case 10 :
        {
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , MCPWM_CHANNEL5 , DutyCycle);
            break;
        }
        case 11 :
        {
            PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , MCPWM_CHANNEL6 , DutyCycle);
            break;
        }
        default :
        {
            break;
        }
    }
}

// electro-aimant au max au demarage pendant un temps limite
void MCPWM_Start_up(uint8_t nPWM)
{
    MCPWM_Actualise_Velocity(nPWM, MaxVelocity);
    
    // todo gestion timer de depart
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

// defini le PWM a appliquer en fonction de la velocite
void gest_MCPWM_New_Velocity(uint8_t nPWM, uint8_t Velocity)
{
    // il est defini que si la velocity == 0, la cmd devient NoteOff
    if (Velocity == 0)
    {
        gest_MCPWM_Off(nPWM);
    }
    else
    {
        // todo mise en memoire de Velocity a appliquer plus tard
        
        // MCPWM_Start_up(nPWM);
        
        // todo del, pour tester
        MCPWM_Actualise_Velocity(nPWM, Velocity);
    }
}

// PWM off pour electro-aimant off
void gest_MCPWM_Off(uint8_t nPWM)
{
    MCPWM_Actualise_Velocity(nPWM, 0);
}



/* *****************************************************************************
 End of File
 */
