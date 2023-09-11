/* ************************************************************************** */
/** gestion SPI via machine d'�tat
 *
 *  @Company
 *    ETML-ES - SCA
 *
 *  @File Name
 *    Mc32Ex8_2_spi_sm.h
 *
 *  @Summary
 *    gestion SPI via machine d'�tat
 *
 *  @Description
 *    gestion SPI via machine d'�tat
 *    Test� pour fonctionner avec LM70 sur SPI1
 * 
 *   Cr�ation 14.03.2017 SCA
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

//fonction � appeler 1x au d�marrage pour init.
void SPI_Init(void);

//Ecriture.
//Comme le SPI est obligatoirement full-duplex,
//les donn�es re�ues ne seront pas trait�es
void SPI_StartWrite(uint32_t nBytes, uint8_t* pBytesToWrite);

//Lecture/�criture.
//Comme le SPI est obligatoirement full-duplex,
//des donn�es sont re�ues simultan�ment � l'envoi
void SPI_StartReadWrite(uint32_t nBytes, uint8_t* pBytesToWrite);

//Lecture.
//Comme le SPI est obligatoirement full-duplex,
//il faut envoyer des donn�es bidons pour faire une lecture
void SPI_StartRead(uint32_t nBytes);

//pour obtenir l'�tat interne de la SM spi
SPI_STATES SPI_GetState (void);

//lecture d'un byte dans buffer r�ception
uint8_t SPI_ReadByte(void);

//fonction � appeler p�riodiquement pour gestion SPI
void SPI_DoTasks(void);


#endif /* _SPI_SM_H */

/* *****************************************************************************
 End of File
 */
