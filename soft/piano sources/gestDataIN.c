/* ************************************************************************** */
/** Descriptive File Name

  Company
    ETML

  File Name
    gestDataIN.c

  Summary
    Gestion de la reception des trames de donnees

  Description
    Contient toutes les fonctions necessaires pour extraire interpreter
		les donne de la trame MIDI
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "app.h"
#include "gestMCPWM.h"
#include "gestDataIN.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

uint8_t CaseNewData;
uint8_t CaseOldestData;
uint8_t data[SizeBufferLocalData];

extern uint8_t instrument;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

// actualise OldestData sur la prochaine data/commande dans le tableau
    // retourne un flag "Normal" si l'on atteint la fin des donnee en reserve
        // (OldestData reste sur sa dernière valeur)
    // retourne un flag "FoundCmd" si l'on rencontre une nouvelle commande
        // (OldestData pointe sur la nouvelle commande)
    // retourne un flag "Empty" si l'on a vide le tableau
uint8_t USART_FindNextData()
{
    int8_t i = 1;
    
    // tant que le tableau n'est pas vide
    while ((CaseOldestData + i) != CaseNewData)
    {
        // la case est une data
            // (NoteOff est la cmd avec la valeur la plus faible)
        if (data[CaseOldestData + i] < NoteOff)
        {
            CaseOldestData = CaseOldestData + i;
            
            return (Normal);
        }
        // la case est une cmd System Real Time, seul cmd qui peut avoir lieu
            // au milieu d'une trame, on l'ignore
        else if (data[CaseOldestData + i] >= SystemRealTime)
        {
            i++;
            // si l'on est hors du tableau
            if ((CaseOldestData + i) >= SizeBufferLocalData)
            {
                // on change i pour retourner a la case 0
                i = -CaseOldestData;
            }
        }
        // la case est une commande
        else
        {
            // on pointe sur la cmd trouve
            CaseOldestData = CaseOldestData + i;
            
            return (FoundCmd);
        }
        
        // si l'on est hors du tableau
        if ((CaseOldestData + i) >= SizeBufferLocalData)
        {
            // on change i pour retourner a la case 0
            i = -CaseOldestData;
        }
    }
    
    return (Empty);
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

// mets en memoire le caractere recu
void gest_USART_New_Data(uint8_t NewData)
{
    // mets le caractere dans le tableau de data
    data[CaseNewData] = NewData;
    
    // incremente le pointeur qui indique la prochaine case a remplir
    CaseNewData ++;
    
    // si l'on est hors du tableau
    if (CaseNewData >= SizeBufferLocalData)
    {
        // on retourne a la case 0
        CaseNewData = 0;
    }
    
    // dans le cas ou l'on va ecraser des donne non traite lors de la prochaine save
        // autrement dit : si le tableau est plein (ne devrait pas arriver)
    if (CaseNewData == CaseOldestData)
    {
        // on "oublie" le plus ancien caractere non traite du tableau
        CaseOldestData++;
        
        // si l'on est hors du tableau
        if (CaseOldestData >= SizeBufferLocalData)
        {
            CaseOldestData = 0;
        }
    }
}


// traite les donnee en memoire et appel l'actualisation des PWM
void gest_USART_Traitement(uint8_t Instrument, uint8_t Octave)
{
    uint8_t CaseCmd;
    uint8_t CaseNote;
    
    uint8_t EventFindData = Normal;
    uint8_t StartNote = Do0 + (12 * Octave);
    
    uint8_t Touche;
    
    // tant que le tableau n'est pas vide
    while ((CaseOldestData != CaseNewData) && (EventFindData != Empty))
    {
        // tant qu'on a pas trouve de commande traite par notre code
            // et que l'on a pas vide le tableau
        while ((data[CaseOldestData] != (NoteOff + Instrument)) &&
            (data[CaseOldestData] != (NoteOn + Instrument)) &&
                (CaseOldestData != CaseNewData))
        {
            CaseOldestData++;
            // si l'on est sorti du tableau
            if (CaseOldestData >= SizeBufferLocalData)
            {
                CaseOldestData = 0;
            }
        }
        
        // si l'on a pas vide le tableau (cmd que l'on traite trouve)
        if (CaseOldestData != CaseNewData)
        {
            CaseCmd = CaseOldestData;
            
            EventFindData = USART_FindNextData();

            // celon l'event retourne par la fonction
            switch (EventFindData)
            {
                // data trouve
                case Normal :
                {
                    // si note dans l'octave controlee
                    if ((data[CaseOldestData] >= StartNote) &&
                            (data[CaseOldestData] < (StartNote + 12)))
                    {
                        CaseNote = CaseOldestData;

                        EventFindData = USART_FindNextData();
                        
                        Touche = data[CaseNote] - (12 * (Octave + 1));
                        
                        if (data[CaseCmd] == (NoteOn + instrument))
                        {
                            // celon l'event retourne par la fonction
                            switch (EventFindData)
                            {
                                // data trouve
                                case Normal :
                                {
                                    gest_MCPWM_New_Velocity(Touche, data[CaseOldestData]);
                                    break;
                                }
                                // tableau vide, ou une nouvelle cmd a ete trouve
                                case FoundCmd :
                                {
                                    // une nouvelle commande a ete trouve
                                        // avant de finir la commande en cours
                                    // on ignore la commande en cours
                                    break;
                                }
                                // tableau vide
                                case Empty :
                                {
                                    // le nombre de parametres recu est insuffisant
                                        // on retourne a la case de la cmd
                                    CaseOldestData = CaseCmd;
                                    break;
                                }
                            }
                        }
                        else if (data[CaseCmd] == (NoteOff + instrument))
                        {
                            gest_MCPWM_Off(Touche);
                        }
                    }
                    break;
                }
                // une nouvelle cmd a ete trouve
                case FoundCmd :
                {
                    // une nouvelle commande a ete trouve
                        // avant de finir la commande en cours
                    // on ignore la commande en cours
                    break;
                }
                // tableau vide
                case Empty :
                {
                    // la commande en cours n'a pas suffisamment de parametres
                    break;
                }
            }
        }
    }
}


void gest_USART_Initialize()
{
    CaseNewData = 0;
    CaseOldestData = 0;
}


/* *****************************************************************************
 End of File
 */
