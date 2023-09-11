/* ************************************************************************** */
/** gestion SPI via machine d'état
 *
 *  @Company
 *    ETML-ES - SCA
 *
 *  @File Name
 *    Mc32Ex8_2_spi_sm.h
 *
 *  @Summary
 *    gestion SPI via machine d'état
 *
 *  @Description
 *    gestion SPI via machine d'état
 *    Testé pour fonctionner avec LM70 sur SPI1
 * 
 *   Création 14.03.2017 SCA
 *  
*/

#ifndef _SPI_SM_H    /* Guard against multiple inclusion */
#define _SPI_SM_H

#include <stdint.h>


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


typedef enum
{
	/* SPI's state machine's states. */
	SPI_STATE_UNINITIALIZED=0,
    SPI_STATE_IDLE,
    SPI_STATE_IDLE_READ_DATA_AVAILABLE,
    SPI_STATE_BUSY_WRITE,        
    SPI_STATE_BUSY_READ_WRITE,
    SPI_STATE_BUSY_READ,
} SPI_STATES;

//extern SPI_STATES spiState;

//fonction à appeler 1x au démarrage pour init.
void SPI_Init(void);

//Ecriture.
//Comme le SPI est obligatoirement full-duplex,
//les données reçues ne seront pas traitées
void SPI_StartWrite(uint32_t nBytes, uint8_t* pBytesToWrite);

//Lecture/écriture.
//Comme le SPI est obligatoirement full-duplex,
//des données sont reçues simultanément à l'envoi
void SPI_StartReadWrite(uint32_t nBytes, uint8_t* pBytesToWrite);

//Lecture.
//Comme le SPI est obligatoirement full-duplex,
//il faut envoyer des données bidons pour faire une lecture
void SPI_StartRead(uint32_t nBytes);

//pour obtenir l'état interne de la SM spi
SPI_STATES SPI_GetState (void);

//lecture d'un byte dans buffer réception
uint8_t SPI_ReadByte(void);

//fonction à appeler périodiquement pour gestion SPI
void SPI_DoTasks(void);


#endif /* _SPI_SM_H */

/* *****************************************************************************
 End of File
 */
