/* 
 * File:   pec12.h
 * Author: ricch
 *
 * Created on August 30, 2023, 9:15 AM
 */

#ifndef PEC12_H
#define	PEC12_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdbool.h>

typedef struct
{
    bool state[4];

} CHANNEL;
    
typedef struct
{
    CHANNEL chA;
    CHANNEL chB;
    CHANNEL chC;
    int8_t incrOrDecr;
    bool isPressed;

} PEC12;




void scanPec12(void);
int8_t getPec12IncrOrDecr(void);




#ifdef	__cplusplus
}
#endif

#endif	/* PEC12_H */

