/********************************************************************************************************
*
* File                : LCD22(main)
* Hardware Environment:
* Build Environment   : Quartus II Version 10.1 / Nios II 10.1
* Version             :
* By                  : Su Wei Feng
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/
#include "alt_types.h"
#include <io.h>
#include <unistd.h>
#include <system.h>
#include "altera_avalon_pio_regs.h"
#include "LCD32.h"
#include "touch.h"
#define msleep(ms)  usleep(1000*ms)
#define DIRECTION_PA(data)  IOWR_ALTERA_AVALON_PIO_DIRECTION(PA_BASE, data)
#define DATA_PA(data)  IOWR_ALTERA_AVALON_PIO_DATA(PA_BASE, data)
int main(void)
{     
       //IOWR_ALTERA_AVALON_PIO_DATA(VCC_BASE, 1);
       //IOWR_ALTERA_AVALON_PIO_DATA(GND_BASE, 0);
       IOWR_ALTERA_AVALON_PIO_DATA(BL_P_BASE, 1);
       IOWR_ALTERA_AVALON_PIO_DATA(BL_N_BASE, 0);
       
       TP_Init(); 
	   LCD_Initializtion();
       
       LCD_Clear(Blue);
       LCD_Clear(Yellow);
       LCD_Clear(White);
       LCD_Clear(Black);
       LCD_Clear(Magenta);
       LCD_Clear(Red);
       LCD_Clear(Green);
       LCD_Clear(Cyan);
        
     // LCD_show_test();
         
        TouchPanel_Calibrate(); 
   
    while (1)  
  {
    getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
    TP_DrawPoint(display.x,display.y);
  }
   

	return 0;
}
