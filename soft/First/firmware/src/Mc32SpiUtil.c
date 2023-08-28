// Mc32SpiUtil.C
// Utilitaire  SPI
//
//	Description : 	fonctions SPI CCS like
//	Auteur 		: 	C. HUBER
//      Création	: 	08.04.2014
//      Modifications   :       09.02.2015 CHR

//  LISTE DES MODIFICATIONS :
//      Utilisation des stdint                  09.02.2015 CHR
//      Adaptation à la plib_spi                09.02.2015 CHR
//      maj version compilateur et Harmony      24.05.2016 CHR
//      Correction de la séquence de lecture    25.05.2016 CHR
//      Version KIT     PCB 11020_B
//	Version		:	V1.2
//	Compilateur	:	XC32 V1.40 + Harmony 1.06
//
/*--------------------------------------------------------*/

// #define MARKER_READ 1

#include "app.h"
#include "Mc32SpiUtil.h"
#include "peripheral\SPI\plib_spi.h"


void spi_write1( uint8_t Val){
   int SpiBusy;
   
   PLIB_SPI_BufferWrite(SPI_ID_1, Val);

   do {
     SpiBusy =  PLIB_SPI_IsBusy(SPI_ID_1) ;
   } while (SpiBusy == 1);
}


void spi_write2( uint8_t Val){
   int SpiBusy;
   
   PLIB_SPI_BufferWrite(SPI_ID_2, Val);
   do {
     SpiBusy =  PLIB_SPI_IsBusy(SPI_ID_2) ;
   } while (SpiBusy == 1);
}

uint8_t spi_read1( uint8_t Val){
   int SpiBusy;  
   uint32_t lu;
   
   PLIB_SPI_BufferWrite(SPI_ID_1, Val);
   // Attends fin transmission
   do {
        SpiBusy =  PLIB_SPI_IsBusy(SPI_ID_1) ;
   } while (SpiBusy == 1);
   
   // Attend arrivée dans fifo
   while (PLIB_SPI_ReceiverFIFOIsEmpty(SPI_ID_1));
#ifdef MARKER_READ
   LED3_W  = 1;
#endif
   lu = PLIB_SPI_BufferRead(SPI_ID_1);
#ifdef MARKER_READ
   LED3_W  = 0;
#endif
   return lu;
}

 uint8_t spi_read2( uint8_t Val){
   int SpiBusy;
   uint8_t lu;
   
   PLIB_SPI_BufferWrite(SPI_ID_2, Val);
  
   do {
      SpiBusy =  PLIB_SPI_IsBusy(SPI_ID_2) ;
   } while (SpiBusy == 1);
   
   // Attend arrivée dans fifo
   while (PLIB_SPI_ReceiverFIFOIsEmpty(SPI_ID_2));
   
   lu = PLIB_SPI_BufferRead(SPI_ID_2);
   return lu;
}



