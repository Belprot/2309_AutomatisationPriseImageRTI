//-----------------------------------------------------
//File: lcd_spi.c
//Auth: ELECTRONIC ASSEMBLY JM
//Modifié par : JONATHAN CHAFLA
//DATE: 08-25-2013
//-----------------------------------------------------

//---Includes---

#include "lcd_spi.h"
#include "Mc32SpiUtil.h"
#include "app.h"
#include "Delays.h"

//--- module global varibles ---

//-----------------------------------------------------
//Func: initDispl()
//Desc: inits Display
//-----------------------------------------------------
void initDispl(void)
{
    DRV_SPI0_Initialize();
    delay_ms(1);
    
	//init Display
	WriteIns(0x3A);	//8-Bit data length extension Bit RE=1; REV=0
	WriteIns(0x09);	//4 line display
	WriteIns(0x06);	//Bottom view
	WriteIns(0x1E);	//Bias setting BS1=1
	WriteIns(0x39);	//8-Bit data length extension Bit RE=0; IS=1
	WriteIns(0x1B);	//BS0=1 -> Bias=1/6
	WriteIns(0x6E); //Devider on and set value
	WriteIns(0x57); //Booster on and set contrast (BB1=C5, DB0=C4)
	WriteIns(0x72); //Set contrast (DB3-DB0=C3-C0)
	WriteIns(0x38); //8-Bit data length extension Bit RE=0; IS=0
    WriteIns(0x0F); //Display on, cursor on, blink on

	ClrDisplay();
	DisplayOnOff(DISPLAY_ON | CURSOR_ON | BLINK_ON);
}

//-----------------------------------------------------
//Func: WriteChar(character)
//Desc: sends a single character to display
//-----------------------------------------------------
void WriteChar (char character)
{
	WriteData(character);
}

//-----------------------------------------------------
//Func: WriteString(string)
//Desc: sends a string to display, must be 0 terminated
//-----------------------------------------------------
void WriteString( char * string ,  ...)
{
	do
	{
		WriteData(*string++);
	}
	while(*string);
}


//-----------------------------------------------------
//Func: SetPostion(postion)
//Desc: set postion
//-----------------------------------------------------
void SetPostion(char pos)
{
	WriteIns(LCD_HOME_L1+pos);
}

//-----------------------------------------------------
//Func: DisplayOnOff(control)
//Desc: use definitions of header file to set display
//-----------------------------------------------------
void DisplayOnOff(char data)
{
	WriteIns(0x08+data);
}

//-----------------------------------------------------
//Func: DefineCharacter(memory postion, character data)
//Desc: stores an own defined character
//-----------------------------------------------------
void DefineCharacter(unsigned char postion, unsigned char *data)
{
	unsigned char i=0;
	WriteIns(0x40+8*postion);

	for (i=0; i<8; i++)
	{
		WriteData(data[i]);
	}
	SetPostion(LINE1);
}
//-----------------------------------------------------
//Func: ClrDisplay
//Desc: Clears entire Display content and set home pos
//-----------------------------------------------------
void ClrDisplay(void)
{
	WriteIns(0x01);
	SetPostion(LINE1);
}
//-----------------------------------------------------
//Func: ClrDisplay
//Desc: Clears entire Display content and set home pos
//-----------------------------------------------------
void ClrDisplayL2(void)
{
	WriteIns(0x01);
	SetPostion(LINE2);
}
//-----------------------------------------------------
//Func: ClrDisplay
//Desc: Clears entire Display content and set home pos
//-----------------------------------------------------
void ClrDisplayL3(void)
{
	WriteIns(0x01);
	SetPostion(LINE3);
}
//-----------------------------------------------------
//Func: ClrDisplay
//Desc: Clears entire Display content and set home pos
//-----------------------------------------------------
void ClrDisplayL4(void)
{
	WriteIns(0x01);
	SetPostion(LINE4);
}
//-----------------------------------------------------
//Func: SetContrast
//Desc: Sets contrast 0..63
//-----------------------------------------------------
void SetContrast(unsigned char contr)
{
	WriteIns(0x39);
	WriteIns(0x54 | (contr >> 4));
	WriteIns(0x70 | (contr & 0x0F));
	WriteIns(0x38);
}
//-----------------------------------------------------
//Func: SetView
//Desc: view bottom view(0x06), top view (0x05)
//-----------------------------------------------------
void SetView(unsigned char view)
{
	WriteIns(0x3A);
	WriteIns(view);
	WriteIns(0x38);
}
//-----------------------------------------------------
//Func: SetROM
//Desc: Changes the Rom code (ROMA=0x00, ROMB=0x04, ROMC=0x0C)
//---------------------------------------------------
void SetROM (unsigned char rom)
{
	WriteIns(0x2A);
	WriteIns(0x72);
	WriteData(rom);
	WriteIns(0x28);
}
//-----------------------------------------------------
//Func: WriteIns(instruction)
//Desc: sends instruction to display
//-----------------------------------------------------
 void WriteIns(uint8_t ins)
{
    spi_write1(0x1F);
    spi_write1(ins & 0x0F);
    spi_write1((ins>>4) & 0x0F);
}
//-----------------------------------------------------
//Func: WriteData(data)
//Desc: sends data to display
//-----------------------------------------------------
 void WriteData(char data)
{
	spi_write1(0x5F);				//Send 5 synchronisation bits, RS = 1, R/W = 0
	spi_write1(data & 0x0F);		//send lower data bits
	spi_write1((data>>4) & 0x0F);	//send higher data bits
}





