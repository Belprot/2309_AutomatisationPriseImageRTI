/*--------------------------------------------------------*/
// Mc32NVMUtil.h
/*--------------------------------------------------------*/
//	Description :	Utilitaire gestion écriture dans memoire programme (NVM)
//	Auteur 		: 	C. HUBER
//      Création	: 	17.02.2015
//      Sur la base de l'exemple Harmony sous :
//      C:\microchip\harmony\v1_00\apps\examples\peripheral\nvm\flash_modify
//  Environnement d'origine :
//  	Version KIT     PCB 11020_B
//		Version		:	V1.61
//		Compilateur	:	XC32 V1.40 + Harmony 1.06
//
//  LISTE DES MODIFICATIONS :
//	24.03.16		CHR  correction dans NVMpageErase
//  08.03.17 v1.61	SCA  correction attente fin écriture dans NVMwriteRow
//                  compilateur xc32 1.42 avec Harmony 1.08   
//
/*--------------------------------------------------------*/

#ifndef Mc32NVMUtil_H
#define Mc32NVMUtil_H

#include <stdint.h>
#include "stepperDriver.h"

/* Row size for pic32mx795 device is 512 bytes */
#define DEVICE_ROW_SIZE_DIVIDED_BY_4           32
/* Page size for pic32mx795 device is 4 Kbytes */
#define DEVICE_PAGE_SIZE_DIVIDED_BY_4           256

extern  const uint32_t  eedata_addr[DEVICE_ROW_SIZE_DIVIDED_BY_4 ] __attribute__((aligned(4096), space(prog)));

#define NVM_PROGRAM_PAGE ((uint32_t)&eedata_addr[0])
// prototypes des fonctions

// Zone ram source
extern uint32_t databuff[DEVICE_ROW_SIZE_DIVIDED_BY_4] __attribute__((coherent));

#define DATA_BUFFER_START ((uint32_t)&databuff[0])


void Init_DataBuff(void);  // pour test
uint32_t NVM_ArrayRead(uint32_t index);  // pour test

// Fonction de base
void NVMpageErase(uint32_t address);
void NVMwriteRow(uint32_t destAddr, uint32_t srcAddr);

// pour stockage et lecture d'une  structure
void NVM_ReadBlock(uint32_t *pData, uint32_t DataSize);
void NVM_WriteBlock(uint32_t *pData, uint32_t DataSize);


#endif
