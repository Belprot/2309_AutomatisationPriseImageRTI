#ifndef Mc32SpiUtil_H
#define Mc32SpiUtil_H
/*--------------------------------------------------------*/
// Mc32SpiUtil.h
/*--------------------------------------------------------*/
//	Description :	Utilitaire gestion SPI CCS like
//
//	Auteur 		: 	C. Huber
//	Version		:	V1.1
//	Compilateur	:	XC32 V1.40
//
/*--------------------------------------------------------*/

#include <stdint.h>

// prototypes des fonctions
void spi_write( uint8_t Val);
//void spi_write2( uint8_t Val);
uint8_t spi_read( uint8_t Val);
//uint8_t spi_read2( uint8_t Val);

#endif
