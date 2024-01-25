/*********************************************************************************************************
* File                : LCD32.c
* Hardware Environment:
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : v0.0.1
* By                  : NGUYEN Tien Thanh
* Created             : Jan 18, 2024
*********************************************************************************************************/
#include "LCD32.h" 
#include "AsciiLib.h"
/* Private define ------------------------------------------------------------*/
#define  ILI9320    0  /* 0x9320 */
#define  ILI9325    1  /* 0x9325 */
#define  ILI9328    2  /* 0x9328 */
#define  ILI9331    3  /* 0x9331 */
#define  SSD1298    4  /* 0x8999 */
#define  SSD1289    5  /* 0x8989 */
#define  ST7781     6  /* 0x7783 */
#define  LGDP4531   7  /* 0x4531 */
#define  SPFD5408B  8  /* 0x5408 */
#define  R61505U    9  /* 0x1505 0x0505 */     
#define  HX8347D    10 /* 0x0047 */
#define  HX8347A    11 /* 0x0047 */ 
#define  LGDP4535   12 /* 0x4535 */  
#define  SSD2119    13 /* 3.5 LCD 0x9919 */

const alt_u16 colorfol1[]={0xf800,0x07e0,0x001f,0xffe0,0x0000,0xffff,0x07ff,0xf81f};
/* Private variables ---------------------------------------------------------*/
static alt_u8 LCD_Code;

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : 
* Input          : - index:
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
__inline void LCD_WriteIndex(alt_u16 index)
{
    Clr_Rs;
    Set_nRd;
    
    IOWR_ALTERA_AVALON_PIO_DIRECTION(LCD32_DATA_BASE, 0XFFFF) ;  /* GPIO_Write(GPIOE,index); */
    IOWR_ALTERA_AVALON_PIO_DATA(LCD32_DATA_BASE, index);
    
    Clr_nWr;
    Set_nWr;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : 
* Input          : - index:
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
__inline void LCD_WriteData(alt_u16 data)
{
    Set_Rs;

    IOWR_ALTERA_AVALON_PIO_DIRECTION(LCD32_DATA_BASE, 0XFFFF) ;  /* GPIO_Write(GPIOE,index); */
    IOWR_ALTERA_AVALON_PIO_DATA(LCD32_DATA_BASE, data);

    Clr_nWr;
    Set_nWr;
}

/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
__inline alt_u16 LCD_ReadData(void)
{
    alt_u16 value;

    Set_Rs;
    Set_nWr;
    Clr_nRd;

/* 
    PE.00(D0), PE.01(D1), PE.02(D2), PE.03(D3), PE.04(D4), PE.05(D5), PE.06(D6), PE.07(D7), PE.08(D8)
    PE.09(D9), PE.10(D10), PE.11(D11), PE.12(D12), PE.13(D13), PE.14(D14), PE.15(D15)   */
/* 
    GPIO_InitTypeDef GPIO_InitStructure; 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
*/
    IOWR_ALTERA_AVALON_PIO_DIRECTION(LCD32_DATA_BASE, 0X0000) ;  /* GPIO_Write(GPIOE,index); */
    value=IORD_ALTERA_AVALON_PIO_DATA(LCD32_DATA_BASE);
     value=IORD_ALTERA_AVALON_PIO_DATA(LCD32_DATA_BASE);

    IOWR_ALTERA_AVALON_PIO_DIRECTION(LCD32_DATA_BASE, 0XFFFF) ;

    Set_nRd;

    return value;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
__inline void LCD_WriteReg(alt_u16 LCD_Reg,alt_u16 LCD_RegValue)
{ 
    /* Write 16-bit Index, then Write Reg */  
    Clr_Cs;
    LCD_WriteIndex(LCD_Reg);         
    /* Write 16-bit Reg */
    LCD_WriteData(LCD_RegValue);  
    Set_Cs; 
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention      : None
*******************************************************************************/
__inline alt_u16 LCD_ReadReg(alt_u16 LCD_Reg)
{
    alt_u16 LCD_RAM;
    
    /* Write 16-bit Index (then Read Reg) */
    Clr_Cs;
    LCD_WriteIndex(LCD_Reg);
    /* Read 16-bit Reg */
    LCD_RAM = LCD_ReadData();       
    Set_Cs;
    return LCD_RAM;
}

      

void LCD_SetCursor( alt_u16 Xpos, alt_u16 Ypos )
{
    #if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

        alt_u16 temp = Xpos;
        Xpos = Ypos;
        Ypos = ( MAX_X - 1 ) - temp;  

    #elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )
        
    #endif

    switch (LCD_Code) {
        default: /* 0x9320 0x9325 0x9328 0x9331 0x5408 0x1505 0x0505 0x7783 0x4531 0x4535 */
            LCD_WriteReg(0x0020, Xpos);
            LCD_WriteReg(0x0021, Ypos);
            break;

        case SSD1298: /* 0x8999 */
        case SSD1289: /* 0x8989 */
            LCD_WriteReg(0x004e, Xpos);
            LCD_WriteReg(0x004f, Ypos);
            break;

        case HX8347A: /* 0x0047 */
        case HX8347D: /* 0x0047 */
            LCD_WriteReg(0x02, Xpos >> 8);
            LCD_WriteReg(0x03, Xpos);

            LCD_WriteReg(0x06, Ypos >> 8);
            LCD_WriteReg(0x07, Ypos);

            break;

        case SSD2119: /* 3.5 LCD 0x9919 */
            break;
    }
}




/*******************************************************************************
* Function Name  : delay_ms
* Description    : Delay Time
* Input          : - cnt: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention      : None
*******************************************************************************/
/*void delay_ms(alt_u16 ms)    
{ 
    alt_u16 i,j; 
    for( i = 0; i < ms; i++ )
    { 
        for( j = 0; j < 1141; j++ );
    }
} 
*/
/*******************************************************************************
* Function Name  : LCD_Initializtion
* Description    : SSD1963 Resets
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_Initializtion(void)
{
    alt_u16 DeviceCode;

    // LCD_Configuration();
    Clr_RST ;      /* LCD_RESET *////0
    delay_ms(100);
    Set_RST;    /////////1

    /* Set MN(multipliers) of PLL, VCO = crystal freq * (N+1) */
    /* PLL freq = VCO/M with 250MHz < VCO < 800MHz */
    /* The max PLL freq is around 120MHz. To obtain 120MHz as the PLL freq */

    DeviceCode = LCD_ReadReg(0x0000);       /* Read LCD ID  */                 
    
    /*For ILI9325*/ 
    if( DeviceCode == 0x9325 || DeviceCode == 0x9328 )  
    {
        LCD_Code = ILI9325;
        LCD_WriteReg(0x00e7,0x0010);      
        LCD_WriteReg(0x0000,0x0001);    /* start internal osc */
        LCD_WriteReg(0x0001,0x0100);     
        LCD_WriteReg(0x0002,0x0700);    /* power on sequence */
        LCD_WriteReg(0x0003,(1<<12)|(1<<5)|(1<<4)|(0<<3) );     /* importance */
        LCD_WriteReg(0x0004,0x0000);                                   
        LCD_WriteReg(0x0008,0x0207);               
        LCD_WriteReg(0x0009,0x0000);         
        LCD_WriteReg(0x000a,0x0000);    /* display setting */        
        LCD_WriteReg(0x000c,0x0001);    /* display setting */        
        LCD_WriteReg(0x000d,0x0000);                    
        LCD_WriteReg(0x000f,0x0000);
        /* Power On sequence */
        LCD_WriteReg(0x0010,0x0000);   
        LCD_WriteReg(0x0011,0x0007);
        LCD_WriteReg(0x0012,0x0000);                                                                 
        LCD_WriteReg(0x0013,0x0000);                 
        delay_ms(50);  /* delay 50 ms */        
        LCD_WriteReg(0x0010,0x1590);   
        LCD_WriteReg(0x0011,0x0227);
        delay_ms(50);  /* delay 50 ms */        
        LCD_WriteReg(0x0012,0x009c);                  
        delay_ms(50);  /* delay 50 ms */        
        LCD_WriteReg(0x0013,0x1900);   
        LCD_WriteReg(0x0029,0x0023);
        LCD_WriteReg(0x002b,0x000e);
        delay_ms(50);  /* delay 50 ms */        
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x0000);           
        delay_ms(50);  /* delay 50 ms */        
        LCD_WriteReg(0x0030,0x0007); 
        LCD_WriteReg(0x0031,0x0707);   
        LCD_WriteReg(0x0032,0x0006);
        LCD_WriteReg(0x0035,0x0704);
        LCD_WriteReg(0x0036,0x1f04); 
        LCD_WriteReg(0x0037,0x0004);
        LCD_WriteReg(0x0038,0x0000);        
        LCD_WriteReg(0x0039,0x0706);     
        LCD_WriteReg(0x003c,0x0701);
        LCD_WriteReg(0x003d,0x000f);
        delay_ms(50);  /* delay 50 ms */        
        LCD_WriteReg(0x0050,0x0000);        
        LCD_WriteReg(0x0051,0x00ef);   
        LCD_WriteReg(0x0052,0x0000);     
        LCD_WriteReg(0x0053,0x013f);
        LCD_WriteReg(0x0060,0xa700);        
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006a,0x0000);
        LCD_WriteReg(0x0080,0x0000);
        LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0082,0x0000);
        LCD_WriteReg(0x0083,0x0000);
        LCD_WriteReg(0x0084,0x0000);
        LCD_WriteReg(0x0085,0x0000);
          
        LCD_WriteReg(0x0090,0x0010);     
        LCD_WriteReg(0x0092,0x0000);  
        LCD_WriteReg(0x0093,0x0003);
        LCD_WriteReg(0x0095,0x0110);
        LCD_WriteReg(0x0097,0x0000);        
        LCD_WriteReg(0x0098,0x0000);  
        /* display on sequence */    
        LCD_WriteReg(0x0007,0x0133);
        
        LCD_WriteReg(0x0020,0x0000);
        LCD_WriteReg(0x0021,0x0000);
    }
    else if( DeviceCode == 0x8989 )  /*For SSD1289*/
    {
        LCD_Code = SSD1289;
        LCD_WriteReg(0x0000,0x0001);    delay_ms(50);   /* Enable LCD Oscillator */
        LCD_WriteReg(0x0003,0xA8A4);    delay_ms(50);   
        LCD_WriteReg(0x000C,0x0000);    delay_ms(50);   
        LCD_WriteReg(0x000D,0x080C);    delay_ms(50);   
        LCD_WriteReg(0x000E,0x2B00);    delay_ms(50);   
        LCD_WriteReg(0x001E,0x00B0);    delay_ms(50);   
        LCD_WriteReg(0x0001,0x2B3F);    delay_ms(50);   /* 320*240 0x2B3F */
        LCD_WriteReg(0x0002,0x0600);    delay_ms(50);
        LCD_WriteReg(0x0010,0x0000);    delay_ms(50);
        LCD_WriteReg(0x0011,0x6070);    delay_ms(50);
        LCD_WriteReg(0x0005,0x0000);    delay_ms(50);
        LCD_WriteReg(0x0006,0x0000);    delay_ms(50);
        LCD_WriteReg(0x0016,0xEF1C);    delay_ms(50);
        LCD_WriteReg(0x0017,0x0003);    delay_ms(50);
        LCD_WriteReg(0x0007,0x0133);    delay_ms(50);         
        LCD_WriteReg(0x000B,0x0000);    delay_ms(50);
        LCD_WriteReg(0x000F,0x0000);    delay_ms(50);
        LCD_WriteReg(0x0041,0x0000);    delay_ms(50);
        LCD_WriteReg(0x0042,0x0000);    delay_ms(50);
        LCD_WriteReg(0x0048,0x0000);    delay_ms(50);
        LCD_WriteReg(0x0049,0x013F);    delay_ms(50);
        LCD_WriteReg(0x004A,0x0000);    delay_ms(50);
        LCD_WriteReg(0x004B,0x0000);    delay_ms(50);
        LCD_WriteReg(0x0044,0xEF00);    delay_ms(50);
        LCD_WriteReg(0x0045,0x0000);    delay_ms(50);
        LCD_WriteReg(0x0046,0x013F);    delay_ms(50);
        LCD_WriteReg(0x0030,0x0707);    delay_ms(50);
        LCD_WriteReg(0x0031,0x0204);    delay_ms(50);
        LCD_WriteReg(0x0032,0x0204);    delay_ms(50);
        LCD_WriteReg(0x0033,0x0502);    delay_ms(50);
        LCD_WriteReg(0x0034,0x0507);    delay_ms(50);
        LCD_WriteReg(0x0035,0x0204);    delay_ms(50);
        LCD_WriteReg(0x0036,0x0204);    delay_ms(50);
        LCD_WriteReg(0x0037,0x0502);    delay_ms(50);
        LCD_WriteReg(0x003A,0x0302);    delay_ms(50);
        LCD_WriteReg(0x003B,0x0302);    delay_ms(50);
        LCD_WriteReg(0x0023,0x0000);    delay_ms(50);
        LCD_WriteReg(0x0024,0x0000);    delay_ms(50);
        LCD_WriteReg(0x0025,0x8000);    delay_ms(50);
        LCD_WriteReg(0x004f,0);
        LCD_WriteReg(0x004e,0);
    }
     
    delay_ms(50);   /* delay 50 ms */       
}

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : 
* Input          : - Color: Screen Color
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_Clear(alt_u16 Color)
{
    alt_u32 index=0;
    
    if( LCD_Code == HX8347D || LCD_Code == HX8347A )
    {
        LCD_WriteReg(0x02,0x00);
        LCD_WriteReg(0x03,0x00);

        LCD_WriteReg(0x04,0x00);
        LCD_WriteReg(0x05,0xEF);

        LCD_WriteReg(0x06,0x00);
        LCD_WriteReg(0x07,0x00);

        LCD_WriteReg(0x08,0x01);
        LCD_WriteReg(0x09,0x3F);
    }
    else
    {
        LCD_SetCursor(0,0); 
    }
    Clr_Cs; 
    LCD_WriteIndex(0x0022);
    for( index = 0; index < MAX_X * MAX_Y; index++ )
    {
        LCD_WriteData(Color);
    }
    Set_Cs;
}
void LCD_show_test()
{ alt_u32 index=0,i;
    if( LCD_Code == HX8347D || LCD_Code == HX8347A )
    {
        LCD_WriteReg(0x02,0x00);
        LCD_WriteReg(0x03,0x00);
                        
        LCD_WriteReg(0x04,0x00);
        LCD_WriteReg(0x05,0xEF);
                         
        LCD_WriteReg(0x06,0x00);
        LCD_WriteReg(0x07,0x00);
                       
        LCD_WriteReg(0x08,0x01);
        LCD_WriteReg(0x09,0x3F);
    }
    else
    {
        LCD_SetCursor(0,0); 
    }
    Clr_Cs; 
    LCD_WriteIndex(0x0022);
  for( i = 0; i< 8; i++ )
    for( index = i*40 * 240; index < (i+1)*40 * 240; index++ )
    {
        LCD_WriteData(colorfol1[i]);
    }
    while(1);
    Set_Cs;
}

/******************************************************************************
* Function Name  : LCD_BGR2RGB
* Description    : RRRRRGGGGGGBBBBB convert to BBBBBGGGGGGRRRRR
* Input          : RGB color
* Output         : None
* Return         : RGB color
* Attention      :
*******************************************************************************/
alt_u16 LCD_BGR2RGB(alt_u16 color)
{
    alt_u16  r, g, b, rgb;

    b = ( color>>0 )  & 0x1f;
    g = ( color>>5 )  & 0x3f;
    r = ( color>>11 ) & 0x1f;
    
    rgb =  (b<<11) + (g<<5) + (r<<0);

    return( rgb );
}


/******************************************************************************
* Function Name  : LCD_GetPoint
* Description    : Get color of the point
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : Screen Color
* Attention      : None
*******************************************************************************/
alt_u16 LCD_GetPoint(alt_u16 Xpos, alt_u16 Ypos)
{
    alt_u16 dummy;

    // Set the cursor position on the LCD
    LCD_SetCursor(Xpos, Ypos);

    // Clear the chip select (CS) signal
    Clr_Cs;

    // Send the command to read the pixel color
    LCD_WriteIndex(0x0022);

    // Switch statement based on the LCD controller code
    switch (LCD_Code)
    {
        // Cases for specific LCD controllers
        case ST7781:
        case LGDP4531:
        case LGDP4535:
        case SSD1289:
        case SSD1298:
            // Read dummy data twice (the actual pixel color)
            dummy = LCD_ReadData();
            dummy = LCD_ReadData();
            // Set the chip select (CS) signal
            Set_Cs;
            return dummy;

        case HX8347A:
        case HX8347D:
            // Read RGB components of the pixel color and combine them
            {
            	alt_u8 red, green, blue;
                red = LCD_ReadData() >> 3;
                green = LCD_ReadData() >> 3;
                blue = LCD_ReadData() >> 2;
                dummy = (green << 11) | (blue << 5) | red;
            }
            // Set the chip select (CS) signal
            Set_Cs;
            return dummy;

        default:
            // Default case for other LCD controllers
            // Read dummy data twice and convert the color format
            dummy = LCD_ReadData();
            dummy = LCD_ReadData();
            // Set the chip select (CS) signal
            Set_Cs;
            // Convert color format from BGR to RGB
            return LCD_BGR2RGB(dummy);
    }
}


/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/



/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_SetPoint(alt_u16 Xpos,alt_u16 Ypos,alt_u16 point)
{
    if( Xpos >= MAX_X || Ypos >= MAX_Y )
    {
        return;
    }
    LCD_SetCursor(Xpos,Ypos);
    LCD_WriteReg(0x0022,point);
}


/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Bresenham's line algorithm
* Input          : - x0:
*                  - y0:
*                          - x1:
*                      - y1:
*                  - color:
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_DrawLine(alt_u16 x0, alt_u16 y0, alt_u16 x1, alt_u16 y1, alt_u16 color)
{
    short dx, dy;
    short temp;

    // Swap coordinates if needed to ensure x0 <= x1 and y0 <= y1
    if (x0 > x1)
    {
        short temp = x1;
        x1 = x0;
        x0 = temp;
    }
    if (y0 > y1)
    {
        short temp = y1;
        y1 = y0;
        y0 = temp;
    }

    dx = x1 - x0;
    dy = y1 - y0;

    // Handle special cases for lines with zero slope
    if (dx == 0)
    {
        do
        {
            LCD_SetPoint(x0, y0, color);
            y0++;
        } while (y1 >= y0);
        return;
    }
    if (dy == 0)
    {
        do
        {
            LCD_SetPoint(x0, y0, color);
            x0++;
        } while (x1 >= x0);
        return;
    }

    // Bresenham's line algorithm
    if (dx > dy)
    {
        temp = 2 * dy - dx;
        while (x0 != x1)
        {
            LCD_SetPoint(x0, y0, color);
            x0++;
            if (temp > 0)
            {
                y0++;
                temp += 2 * dy - 2 * dx;
            }
            else
            {
                temp += 2 * dy;
            }
        }
        LCD_SetPoint(x0, y0, color);
    }
    else
    {
        temp = 2 * dx - dy;
        while (y0 != y1)
        {
            LCD_SetPoint(x0, y0, color);
            y0++;
            if (temp > 0)
            {
                x0++;
                temp += 2 * dy - 2 * dx;
            }
            else
            {
                temp += 2 * dy;
            }
        }
        LCD_SetPoint(x0, y0, color);
    }
}

/******************************************************************************
* Function Name  : PutChar
* Description    : 
* Input          : - Xpos:
*                  - Ypos:
*                          - ASCI:
*                          - charColor:
*                          - bkColor:
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void PutChar( alt_u16 Xpos, alt_u16 Ypos, alt_u8 ASCI, alt_u16 charColor, alt_u16 bkColor )
{
    alt_u16 i, j;
    alt_u8 buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
            {
                LCD_SetPoint( Xpos + j, Ypos + i, charColor );
            }
            else
            {
                LCD_SetPoint( Xpos + j, Ypos + i, bkColor );
            }
        }
    }
}

/******************************************************************************
* Function Name  : GUI_Text
* Description    : 
* Input          : - Xpos: 
*                  - Ypos: 
*                          - str:
*                          - charColor:
*                          - bkColor:
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void GUI_Text(alt_u16 Xpos, alt_u16 Ypos, alt_u8 *str,alt_u16 Color, alt_u16 bkColor)
{
    alt_u8 TempChar;
    do
    {
        TempChar = *str++;  
        PutChar( Xpos, Ypos, TempChar, Color, bkColor );    
        if( Xpos < MAX_X - 8 )
        {
            Xpos += 8;
        } 
        else if ( Ypos < MAX_Y - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }    
    }
    while ( *str != 0 );
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/













