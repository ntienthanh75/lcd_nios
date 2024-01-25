/*********************************************************************************************************
* File                : touch.h
* Hardware Environment:
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : v0.0.1
* By                  : NGUYEN Tien Thanh
* Created             : Jan 18, 2024
*********************************************************************************************************/

#ifndef _TOUCH_H_
#define _TOUCH_H_

#include <io.h>
#include <system.h>
#include <unistd.h>
#include "alt_types.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_spi_regs.h"
#include "altera_avalon_spi.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct Coordinate
{
   alt_u16  x,
            y;
  
}Coordinate;


typedef struct Matrix 
{                       
long double An,  
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
} Matrix ;

/* Private variables ---------------------------------------------------------*/
extern Coordinate ScreenSample[3];
extern Coordinate DisplaySample[3];
extern Matrix matrix ;
extern Coordinate  display ;

/* Private define ------------------------------------------------------------*/

#define CHX     0x90
#define CHY     0xd0

void LCD_show_test();

#define TP_CS(x)    x ? IOWR_ALTERA_AVALON_PIO_DATA(TOUCH_CS_BASE, 1): IOWR_ALTERA_AVALON_PIO_DATA(TOUCH_CS_BASE, 0)

#define TP_INT_IN   IORD_ALTERA_AVALON_PIO_DATA(TOUCH_IRQ_BASE)

#define TOUCH_SPI_INIT  IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_TOUCH_BASE,0x00)
/* Private function prototypes -----------------------------------------------*/                
void TP_Init(void); 
Coordinate *Read_Ads7846(void);
void TouchPanel_Calibrate(void);
//void DrawCross(alt_u16 Xpos,alt_u16 Ypos);
//void TP_DrawPoint(alt_u16 Xpos,alt_u16 Ypos);///////////////////////////////
alt_u8 setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);
alt_u8 getDisplayPoint(Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr );
void DelayUS(alt_u32 cnt);
void WR_CMD (alt_u8 cmd);  
int RD_AD(void);
int Read_X(void);  
int Read_Y(void);
void TP_GetAdXY(int *x,int *y);
void TP_DrawPoint(alt_u32 Xpos,alt_u32 Ypos);
void DrawCross(alt_u32 Xpos,alt_u32 Ypos);





#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


