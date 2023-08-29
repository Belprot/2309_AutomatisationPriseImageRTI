/*--------------------------------------------------------*/
//	Mc32Delays.c
/*--------------------------------------------------------*/
//	Description :	Fonctions de delay afin
//			de pouvoir utiliser la même syntaxe
//			que sous CCS.
//
//	Auteurs 		: 	F. Dominé / C. Huber
//
//	Version		:	V2.0
//	Compilateur	:	nécessaire pour XC32 V 1.40
//
// Revu / modifié:
// CHR 17.12.2015    besoin du fichier maison Mc32CoreTimer.h
// SCA 11.10.2016   ajouté fonction délais core timer
/*--------------------------------------------------------*/


#include "Mc32Delays.h"
// Nécessaire pour la fonction delay_msCt
//#include "Mc32CoreTimer.h"
#include <xc.h> //pour les fonctions d'accès au Core Timer
#include <stdint.h>

/*--------------------------------------------------------*/
// Fonction delay500ns
/*--------------------------------------------------------*/
void delay500ns(void)
{
  // Il a fallu touiller un peu pour arriver à 500ns
  // CHR 09.09.2014 avec 8 250 ns avec 9 612 ns ??????
	unsigned int cptcycles;
	  for (cptcycles=0; cptcycles < 9; cptcycles++) {
		delay_cycle();
      }
}

/*--------------------------------------------------------*/
// Fonction delay_us
/*--------------------------------------------------------*/
void delay_us(unsigned int us)
{
  // Il a fallu touiller un peu pour arriver à la us
  // CHR 09.09.2014 grosse retouche 17 au lieu de 3 ???????
	unsigned int i,j,cptcycles;
	for(i=1;i<=us;i++){
	  for (cptcycles=0; cptcycles < 17; cptcycles++) {
		delay_cycle();
      }
      if (i==us) {
        delay_cycle();
        delay_cycle();
        delay_cycle();
        delay_cycle();
      }
      else { //compensation des call, return, sauvegardes et restitutions
        j=0;
        j++;
        j++;
        j++;
        j++;
        j++;
        j++;
        delay_cycle();
        delay_cycle();
        delay_cycle();
     }
  }
}
/*--------------------------------------------------------*/
// Fonction delay_ms
/*--------------------------------------------------------*/
void delay_ms(unsigned int ms)
{

	unsigned int i;

	for(i=1;i<=ms;i++)
	{
		// 1ms = 1000us
		delay_us(1000);
	}
}

/*--------------------------------------------------------*/
// Fonctions de délais utilisant le Core Timer
/*--------------------------------------------------------*/
//SCA 11.10.2016:
// 1. mis les fonctions du Core Timer
//  _CP0_GET_COUNT() : lecture valeur Core Timer
//  et _CP0_SET_COUNT() : écriture nouvelle valeur Core Timer
//  en dur dans ce fichier
// 2. Forcé l'optimisation 0 des fonctions delay_msCt() et delay_usCt()
// ci-dessous
// Tout ceci pour éviter toute optimisation et obtenir un comportement reproductible
//
// Fonctions testées avec MPLABX 3.40, xc32 1.42 et Harmony 1.08.01 sur Starter-kit ES
// PIC32MXF795L @ 80 MHz. Mesures :
//  Fonction    |    delta(cycles@80MHz, optimisation 0) delta(cycles@80MHz, optimisation 1)
//  ------------+---------------------------------------------------------------------------
//  delay_msCt  |    0 (exact)                           0 (exact)
//  delay_usCt  |    +3                                  +4
//  delay500nsCt|    0 (exact)                           +3 (537.5ns)

// Les délais sont exacts en optimisation 1. 4 cycles CPU de surplus en optimisation 0
// (appels aux fonctions ? / toggle des IO ?)

#ifndef SYS_FREQ
    #define SYS_FREQ (80000000L)    //80 MHz
#endif

//le core timer est incrémenté tous les 2 SYSCLK
#define TICK_CT_MS (SYS_FREQ / 2000L)
#define TICK_CT_US (SYS_FREQ / 2000000L)
#define TICK_OVERHEAD 15    //pour ajustement. mesuré 15 cycles Core Timer de surplus
//valeur quasi-fixe (pu observer 3 cycles CPU en plus en optimisation 0
//par rapport à 1.

/*--------------------------------------------------------*/
// Fonction delay_msCt Core Timer
/*--------------------------------------------------------*/
//attente passive n * ms
//utilise le Core Timer
void __attribute__((optimize("-O0"))) delay_msCt(uint32_t NbMs)
{
    uint32_t time_to_wait;

    _CP0_SET_COUNT(0);
    time_to_wait = (TICK_CT_MS * NbMs) - TICK_OVERHEAD;
    while(_CP0_GET_COUNT()  < time_to_wait) {
        // Waiting
    }
}

/*--------------------------------------------------------*/
// Fonction delay_usCt Core Timer
/*--------------------------------------------------------*/
//attente passive n * us
//utilise le Core Timer
void __attribute__((optimize("-O0"))) delay_usCt(uint32_t NbUs)
{
    uint32_t time_to_wait;

    _CP0_SET_COUNT(0);
    time_to_wait = (TICK_CT_US * NbUs) - TICK_OVERHEAD;
    while(_CP0_GET_COUNT()  < time_to_wait) {
        // Waiting
    }
}

/*--------------------------------------------------------*/
// Fonction delay500nsCt Core Timer
/*--------------------------------------------------------*/
//attente passive 500ns
//utilise le Core Timer
void __attribute__((optimize("-O0"))) delay500nsCt(void)
{
    _CP0_SET_COUNT(0);

    while(_CP0_GET_COUNT()  < 5L) {
        // Waiting
    }
}
