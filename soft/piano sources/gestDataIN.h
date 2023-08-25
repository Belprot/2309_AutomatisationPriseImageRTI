/* ************************************************************************** */
/** Descriptive File Name

  Company
    ETML

  File Name
    gestDataIN.h

  Summary
    Gestion de la reception des trames de donnees

  Description
    Declaration des fonctions et define des valeurs utile
 */
/* ************************************************************************** */

#ifndef GESTDATAIN_H    /* Guard against multiple inclusion */
#define GESTDATAIN_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */
    
#define Normal 0
#define FoundCmd 1
#define Empty 2
    
#define NoteOff 0x80
#define NoteOn 0x90
#define SystemRealTime 0xf8
    
#define Do0 12
    
    // valeur arbitraire
    // lorsque l'on mets en pause une musique, l'IRIG envoie une trame de ~100data
#define SizeBufferLocalData 200
    
    
    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************
    
    
    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

uint8_t USART_FindNextData();
    
void gest_USART_New_Data(uint8_t NewData);
void gest_USART_Traitement(uint8_t Instrument, uint8_t Octave);
void gest_USART_Initialize();


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* GESTDATAIN_H */

/* *****************************************************************************
 End of File
 */
