#ifndef __MC32DELAYS_H
#define __MC32DELAYS_H
/*--------------------------------------------------------*/
//      Mc32Delays.h
/*--------------------------------------------------------*/
//	Description :	Adaptation des fonctions de delay afin
//					de pouvoir utiliser la m�me syntaxe
//					que sous CCS.
//
//	Auteur 		: 	F. Domin�
//
//	Version		:	V1.0
//	Compilateur	:	C32 V2.01
//
/*--------------------------------------------------------*/


/*--------------------------------------------------------*/
// D�finition des fonctions prototypes
/*--------------------------------------------------------*/
// Quelques commentaires:
// Avec notre configuration, le temps n�cessire pour passer
// une sortie � 0 puis � 1 est denviron 65ns
// En ajoutant un nop, on obtient environ 100ns
// On peut en conclure que le temps de cycle est de 33ns
#define delay_cycle() __asm__( "nop" )
void delay500ns(); // n�cessaire pour lcd
void delay_us(unsigned int); // 32 bits
void delay_ms(unsigned int); // 32 bits

//Fonctions bas�es sur Core Timer
void delay_msCt(unsigned int NbMs);
void delay_usCt(unsigned int NbUs);
void delay500nsCt(void);


#endif
