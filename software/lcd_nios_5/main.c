/*********************************************************************************************************
* File                : main.c
* Hardware Environment:
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : v0.0.1
* By                  : NGUYEN Tien Thanh
* Created             : Jan 18, 2024
*********************************************************************************************************/
#include "alt_types.h"
#include <io.h>
#include <unistd.h>
#include <system.h>
#include "altera_avalon_pio_regs.h"
#include "LCD32.h"
#include "touch.h"
#include "game.h"
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
   
    /*while (1)
	{
    	getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
    	TP_DrawPoint(display.x,display.y);
	}*/
        DrawOnLCDAndStore();
        delay_ms(500);
        DisplayNumber(42);
        delay_ms(500);
        DisplayNumber(9999);
        delay_ms(500);
        DisplayStoredImage();
   

	return 0;
}
