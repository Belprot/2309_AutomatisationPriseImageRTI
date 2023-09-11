//-----------------------------------------------------
//File: lcd_spi.h
//Auth: ELECTRONIC ASSEMBLY JM
//DATE: 08-07-2012
//-----------------------------------------------------
#ifndef LCDSPI_H
#define LCDSPI_H
//#include "PortConfig.h"
#include <stdint.h>

//--- Port definitions ---

//#define SOD		PORTBbits.RB5
//#define SCLK	PORTBbits.RB14
//#define SID		PORTBbits.RB1

//--- Standard definitions for LCD ---
#define LCD_HOME_L1	0x80
#define LINE1	0
/*
//HD44780
#define LINE2	LINE1+0x40
#define LINE3	LINE1+0x14
#define	LINE4 	LINE2+0x14
*/
//KS0073/SSD1803(A)
#define LINE2	LINE1+0x20
#define LINE3	LINE1+0x40
#define	LINE4 	LINE1+0x60

#define DISPLAY_ON 	0x04
#define DISPLAY_OFF 0x03
#define CURSOR_ON	0x02
#define CURSOR_OFF	0x05
#define BLINK_ON	0x01
#define BLINK_OFF	0x06


#define TOPVIEW		0x05
#define BOTTOMVIEW	0x06
#define ROMA		0x00
#define ROMB		0x04
#define ROMC		0x0C

/*Declaration*/
#define GETCURSORADDR()	CheckBusy()

void WriteString( char * string ,  ...);
void SetPostion		(char pos);
void DisplayOnOff	(char data);
void DefineCharacter(unsigned char postion, unsigned char *data);
void ClrDisplay    (void);
void ClrDisplayL2    (void);
void ClrDisplayL3    (void);
void ClrDisplayL4    (void);
void SetContrast	(unsigned char contr);
void SetView		(unsigned char view);
void SetROM			(unsigned char rom);
void CreateLcdDegreeSymbol(uint8_t adress);


//Normally you don't need these functions outside this module
 void WriteIns		(uint8_t ins);
 void WriteData		(char data);
//
//unsigned char CheckBusy		(void);
// void   SPI_put 		(unsigned char byte);

void initDispl(void);

#endif