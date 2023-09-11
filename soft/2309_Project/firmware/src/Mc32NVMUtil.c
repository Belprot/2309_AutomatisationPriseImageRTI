/*--------------------------------------------------------*/
// Mc32NVMUtil.c
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

#include "Mc32NVMUtil.h"
#include "peripheral\NVM\plib_nvm.h"
#include <sys/kmem.h>


// Row dans flash pour data
const uint32_t  eedata_addr[DEVICE_ROW_SIZE_DIVIDED_BY_4 ] __attribute__((aligned(4096), space(prog)));

// Zone ram source pour copie row
uint32_t databuff[DEVICE_ROW_SIZE_DIVIDED_BY_4] __attribute__((coherent));



void Init_DataBuff(void)
{
    int i;
    for (i= 0 ; i < DEVICE_ROW_SIZE_DIVIDED_BY_4 ; i++) {
        databuff[i] = i * 10;
    }
}


//*****************************************************************************
/*
  Function:
    uint32_t virtualToPhysical (uint32_t address)

  Summary:
    Converts a virtual memory address to a physical one
*/
uint32_t virtualToPhysical(uint32_t address)
{
   return (address & 0x1FFFFFFF);
}

//******************************************************************************
/*
  Function:
    void NVMpageErase (uint32_t address)

  Summary:
    Erases a page in flash memory (4 KB)
*/
void NVMpageErase(uint32_t address)
{
   bool status;
    
   // Base address of page to be erased
   PLIB_NVM_FlashAddressToModify(NVM_ID_0, virtualToPhysical(address));
   
   // Disable flash write/erase operations
   PLIB_NVM_MemoryModifyInhibit(NVM_ID_0);

   // Select page erase function & enable flash write/erase operations
   PLIB_NVM_MemoryOperationSelect(NVM_ID_0, PAGE_ERASE_OPERATION);

   // Allow memory modifications
   PLIB_NVM_MemoryModifyEnable(NVM_ID_0);

   // Write the unlock key sequence
   PLIB_NVM_FlashWriteKeySequence(NVM_ID_0, 0x0);
   PLIB_NVM_FlashWriteKeySequence(NVM_ID_0, 0xAA996655);
   PLIB_NVM_FlashWriteKeySequence(NVM_ID_0, 0x556699AA);

   // Start the operation
   // PLIB_NVM_FlashWriteStart(NVM_ID_0);
   PLIB_NVM_FlashEraseStart(NVM_ID_0);   // CHR correction du 24.03.2016

   // Wait until the operation has completed
   do {
       status = PLIB_NVM_FlashWriteCycleHasCompleted(NVM_ID_0);
   }  while (status == false);

   // Disable flash write/erase operations
   PLIB_NVM_MemoryModifyInhibit(NVM_ID_0);
   
   // Test si l'action c'est terminée prematurément
   //if (PLIB_NVM_WriteOperationHasTerminated(NVM_ID_0) == false) {
       // OK quittance par LED_7
       // LED7_W = 0; 
   //}
}

//******************************************************************************
/*
  Function:
    void NVMwriteRow(uint32_t address, uint32_t dataAddress)

  Summary:
    Writes a row in flash memory (2KB)
*/
void NVMwriteRow(uint32_t destAddr, uint32_t srcAddr)
{
   // Base address of row to be written to (destination)
   PLIB_NVM_FlashAddressToModify(NVM_ID_0, virtualToPhysical(destAddr));
    
   // Data buffer address (source)
   PLIB_NVM_DataBlockSourceAddress(NVM_ID_0, virtualToPhysical(srcAddr));
   
   // Disable flash write/erase operations
   PLIB_NVM_MemoryModifyInhibit(NVM_ID_0);

   // Select row write function & enable flash write/erase operations
   PLIB_NVM_MemoryOperationSelect(NVM_ID_0, ROW_PROGRAM_OPERATION);

   // Allow memory modifications
   PLIB_NVM_MemoryModifyEnable(NVM_ID_0);

   // Write the unlock key sequence
   PLIB_NVM_FlashWriteKeySequence(NVM_ID_0, 0xAA996655);
   PLIB_NVM_FlashWriteKeySequence(NVM_ID_0, 0x556699AA);

   // Start the operation
   PLIB_NVM_FlashWriteStart(NVM_ID_0);

   //Correction SCA 8.03 : attente fin de l'écriture
   while (!PLIB_NVM_FlashWriteCycleHasCompleted(NVM_ID_0));
}


uint32_t NVM_ArrayRead(uint32_t index)
{
    uint32_t Res;
    Res = eedata_addr[index];
    return Res;
}

 
 int NVMCheckLoop(void)
 {
   int i, stat;
   
   stat = 0;
   for (i=0; i < DEVICE_ROW_SIZE_DIVIDED_BY_4 ; i++) {
       if (eedata_addr[i]  != databuff[i]) {
           stat = 100000 + i;
       }
   }
   return stat;
 };

// Cette fonction écrit un bloc de data au début de la zone flash
// PData correspond à l'adresse du bloc de donnée
// DataSize est la taille en octets du bloc de donnée

void NVM_WriteBlock(uint32_t *pData, uint32_t DataSize)
{
    int i, iMax;
   
    // Efface la page dans la flash
    NVMpageErase(NVM_PROGRAM_PAGE);
         
    if ( (DataSize % 4) != 0  ) {
        iMax = (DataSize / 4 ) + 1;
    } else {
        iMax = DataSize / 4 ;
    }

    // Copie le bloc dans databuff
    for ( i = 0 ; i < iMax; i++ ) {
        databuff[i] = *pData;
        pData++;
    }
    NVMwriteRow(NVM_PROGRAM_PAGE, DATA_BUFFER_START);
}

void NVM_ReadBlock(uint32_t *pData, uint32_t DataSize)
{
    int i, iMax;

    if ( (DataSize % 4) != 0  ) {
        iMax = (DataSize / 4 ) + 1;
    } else {
        iMax = DataSize / 4 ;
    }
    for ( i = 0 ; i < iMax; i++ ) {
        *pData = eedata_addr[i];
        pData++;
    }
}

