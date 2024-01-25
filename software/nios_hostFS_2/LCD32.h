/*********************************************************************************************************
*
* File                : LCD.h
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#ifndef __LCD32_H 
#define __LCD32_H

#include <io.h>
#include <system.h>
#include <unistd.h>
#include "alt_types.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_spi_regs.h"
#include "altera_avalon_spi.h"

#define delay_ms(ms)    usleep(1000*ms)
/* Includes ------------------------------------------------------------------*/
//#include <math.h>


//#define DISP_HOR_RESOLUTION               320
//#define DISP_VER_RESOLUTION               240

//#define DISP_ORIENTATION                  0
#define DISP_ORIENTATION                    90
//#define DISP_ORIENTATION                  180
//#define DISP_ORIENTATION                  270

#if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

#define  MAX_X  320
#define  MAX_Y  240   

#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

#define  MAX_X  240
#define  MAX_Y  320   

#endif


#define Set_Cs        IOWR_ALTERA_AVALON_PIO_DATA(LCD_CS_BASE, 1)
#define Clr_Cs        IOWR_ALTERA_AVALON_PIO_DATA(LCD_CS_BASE, 0)

#define Set_Rs        IOWR_ALTERA_AVALON_PIO_DATA(LCD_RS_BASE, 1)
#define Clr_Rs        IOWR_ALTERA_AVALON_PIO_DATA(LCD_RS_BASE, 0)

#define Set_nWr       IOWR_ALTERA_AVALON_PIO_DATA(LCD_WR_BASE, 1)
#define Clr_nWr       IOWR_ALTERA_AVALON_PIO_DATA(LCD_WR_BASE, 0)

#define Set_nRd       IOWR_ALTERA_AVALON_PIO_DATA(LCD_RD_BASE, 1)
#define Clr_nRd       IOWR_ALTERA_AVALON_PIO_DATA(LCD_RD_BASE, 0)

#define Set_RST       IOWR_ALTERA_AVALON_PIO_DATA(RESET_BASE, 1)
#define Clr_RST       IOWR_ALTERA_AVALON_PIO_DATA(RESET_BASE, 0)

/* LCD color */

#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#define RGB565CONVERT(red, green, blue) (int) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))

/* Private function prototypes -----------------------------------------------*/
void LCD_Initializtion(void);
void LCD_Clear(alt_u16 Color); 
alt_u16 LCD_BGR2RGB(alt_u16 color);
//void LCD_SetBacklight(uint8_t intensity);
alt_u16 LCD_GetPoint(alt_u16 Xpos,alt_u16 Ypos);
void LCD_SetPoint(alt_u16 Xpos,alt_u16 Ypos,alt_u16 point);
void PutChar(alt_u16 Xpos,alt_u16 Ypos,alt_u8 c,alt_u16 charColor,alt_u16 bkColor);
void LCD_DrawLine( alt_u16 x0, alt_u16 y0, alt_u16 x1, alt_u16 y1 , alt_u16 color );
//void PutChinese(alt_u16 Xpos,alt_u16 Ypos,alt_u8 *str,alt_u16 Color,alt_u16 bkColor); 
void GUI_Text(alt_u16 Xpos, alt_u16 Ypos, alt_u8 *str,alt_u16 Color, alt_u16 bkColor);
//void GUI_Chinese(alt_u16 Xpos, alt_u16 Ypos, alt_u8 *str,alt_u16 Color, alt_u16 bkColor);  
//void LCD_DrawPicture(alt_u16 StartX,alt_u16 StartY,alt_u16 EndX,alt_u16 EndY,alt_u16 *pic);


__inline void LCD_WriteIndex(alt_u16 index);
__inline void LCD_WriteData(alt_u16 data);
__inline alt_u16 LCD_ReadData(void);
__inline alt_u16 LCD_ReadReg(alt_u16 LCD_Reg);
__inline void LCD_WriteReg(alt_u16 LCD_Reg,alt_u16 LCD_RegValue);
void LCD_SetCursor( alt_u16 Xpos, alt_u16 Ypos );
//void delay_ms(alt_u16 ms);

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

