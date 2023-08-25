/* ************************************************************************** */
/** Descriptive File Name

  Company
    ETML

  File Name
    gestMCPWM.c

  Summary
    Gestion des PWM

  Description
  Contient toutes les fonctions necessaires pour utiliser les MCPWM_CHANNEL
	avec des controles avances
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "app.h"
#include "gestMCPWM.h"
#include "driver/mcpwm/drv_mcpwm_static.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */


// nPWM ne correspond pas à MCPWM_CHANNEL
    // voir le schema pour le n° du channel de chaque nPWM
const uint32_t ChannelMCPWM [12] =
{
    MCPWM_CHANNEL7,
    MCPWM_CHANNEL1,
    MCPWM_CHANNEL10,
    MCPWM_CHANNEL4,
    MCPWM_CHANNEL8,
    MCPWM_CHANNEL2,
    MCPWM_CHANNEL9,
    MCPWM_CHANNEL3,
    MCPWM_CHANNEL11,
    MCPWM_CHANNEL12,
    MCPWM_CHANNEL5,
    MCPWM_CHANNEL6,
};

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

// mets a jour le PWM applique a un electro-aimant
void gest_MCPWM_Actualise_Velocity(uint8_t nPWM, uint8_t Velocity)
{
    uint16_t DutyCycle;
    
    // On prends en compte le PWM de maintien
    DutyCycle = ((MaintienPWM + Velocity) * PulseWidthMaxMCPWM) / (MaintienPWM + VelocityMax);
    
    // actualisation du PWM
    PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , ChannelMCPWM[nPWM] , DutyCycle);
}



// gere les controles avances des touches lorsque l'on recoit un nouveau Note On
void gest_MCPWM_New_Velocity(uint8_t nPWM, uint8_t Velocity)
{
    // il est defini qu'en MIDI si la velocity est de 0, la cmd devient NoteOff
    if (Velocity == 0)
    {
        gest_MCPWM_Off(nPWM);
    }
    else
    {
        // garde en memoire la velocite
        DataMCPWM.ValPWM[nPWM] = Velocity;
        // set le compteur pour le temps de lancement du mouvement
        DataMCPWM.RemainingStartTime[nPWM] = StartTime;
        // set le compteur pour le temps du PWM depends de velocite
        DataMCPWM.RemainingMonteTime[nPWM] = MonteTime;
        
        // PWM au max pour lancer le mouvement
        gest_MCPWM_Actualise_Velocity(nPWM, VelocityMax);
    }
}



// PWM off pour electro-aimant off
void gest_MCPWM_Off(uint8_t nPWM)
{
    // PWM à 0
    PLIB_MCPWM_ChannelPrimaryDutyCycleSet (MCPWM_ID_0 , ChannelMCPWM[nPWM] , 0);
    
    // enleve les donnees de la structure
    DataMCPWM.ValPWM[nPWM] = 0;
    DataMCPWM.RemainingStartTime[nPWM] = 0;
    DataMCPWM.RemainingMonteTime[nPWM] = 0;
}



/* *****************************************************************************
 End of File
 */
