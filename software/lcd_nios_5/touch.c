/*********************************************************************************************************
* File                : touch.c
* Hardware Environment:
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : v0.0.1
* By                  : NGUYEN Tien Thanh
* Created             : Jan 18, 2024
*********************************************************************************************************/
#include "touch.h"
#include "LCD32.h"

/* Private variables ---------------------------------------------------------*/
Matrix matrix ;
Coordinate  display ;


Coordinate ScreenSample[3];

Coordinate DisplaySample[3] = {{ 50, 45},{ 220, 45},{ 160, 210}};

/* Private define ------------------------------------------------------------*/
#define THRESHOLD 2



/*******************************************************************************
* Function Name  : TP_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void TP_Init(void) 
{
    TP_CS(1); 
    TOUCH_SPI_INIT; 
} 


/*******************************************************************************
* Function Name  : DelayUS
* Description    : 
* Input          : - cnt:
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void DelayUS(alt_u32 cnt)
{
    usleep(cnt);
}


/*******************************************************************************
* Function Name  : WR_CMD
* Description    : 
* Input          : - cmd: 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void WR_CMD (alt_u8 cmd)  
{
    alt_avalon_spi_command(SPI_TOUCH_BASE,0,1,&cmd,0,NULL,ALT_AVALON_SPI_COMMAND_MERGE);
}

/*******************************************************************************
* Function Name  : RD_AD
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
int RD_AD(void)  
{ 
    unsigned short buf; 
    alt_u8 temp;
    alt_avalon_spi_command(SPI_TOUCH_BASE,0,0,NULL,1,&temp,ALT_AVALON_SPI_COMMAND_MERGE);

    buf=((alt_u16)temp)<<8; 
    DelayUS(1);

    alt_avalon_spi_command(SPI_TOUCH_BASE,0,0,NULL,1,&temp,ALT_AVALON_SPI_COMMAND_MERGE);

    buf |=(alt_u16)temp; 

    buf>>=3; 
    buf&=0xfff; 
    return buf; 
} 


/*******************************************************************************
* Function Name  : Read_X
* Description    : Read ADS7843 ADC X 
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
int Read_X(void)
{
    int i;
    TP_CS(0);
    DelayUS(1);
    WR_CMD(CHX);
    DelayUS(1);
    i=RD_AD();
    TP_CS(1);
    return i;
}

/*******************************************************************************
* Function Name  : Read_Y
* Description    : Read ADS7843 ADC Y
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
int Read_Y(void)  
{
    int i;
    TP_CS(0);
    DelayUS(1);
    WR_CMD(CHY);
    DelayUS(1);
    i=RD_AD();
    TP_CS(1);
    return i;
}


/*******************************************************************************
* Function Name  : TP_GetAdXY
* Description    : Read ADS7843
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
void TP_GetAdXY(int *x,int *y)  
{ 
    int adx,ady;
    adx=Read_X();
    DelayUS(1);
    ady=Read_Y();
    *x=adx;
    *y=ady;
} 

/*******************************************************************************
* Function Name  : TP_DrawPoint
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void TP_DrawPoint(alt_u32 Xpos,alt_u32 Ypos)
{
    LCD_SetPoint(Xpos,Ypos,Blue);     /* Center point */
    LCD_SetPoint(Xpos+1,Ypos,Blue);
    LCD_SetPoint(Xpos,Ypos+1,Blue);
    LCD_SetPoint(Xpos+1,Ypos+1,Blue);
}

/*******************************************************************************
* Function Name  : DrawCross
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void DrawCross(alt_u32 Xpos,alt_u32 Ypos)
{
    LCD_DrawLine(Xpos-15,Ypos,Xpos-2,Ypos,Red);
    LCD_DrawLine(Xpos+2,Ypos,Xpos+15,Ypos,Red);
    LCD_DrawLine(Xpos,Ypos-15,Xpos,Ypos-2,Red);
    LCD_DrawLine(Xpos,Ypos+2,Xpos,Ypos+15,Red);

    LCD_DrawLine(Xpos-15,Ypos+15,Xpos-7,Ypos+15,RGB565CONVERT(184,158,131));
    LCD_DrawLine(Xpos-15,Ypos+7,Xpos-15,Ypos+15,RGB565CONVERT(184,158,131));

    LCD_DrawLine(Xpos-15,Ypos-15,Xpos-7,Ypos-15,RGB565CONVERT(184,158,131));
    LCD_DrawLine(Xpos-15,Ypos-7,Xpos-15,Ypos-15,RGB565CONVERT(184,158,131));

    LCD_DrawLine(Xpos+7,Ypos+15,Xpos+15,Ypos+15,RGB565CONVERT(184,158,131));
    LCD_DrawLine(Xpos+15,Ypos+7,Xpos+15,Ypos+15,RGB565CONVERT(184,158,131));

    LCD_DrawLine(Xpos+7,Ypos-15,Xpos+15,Ypos-15,RGB565CONVERT(184,158,131));
    LCD_DrawLine(Xpos+15,Ypos-15,Xpos+15,Ypos-7,RGB565CONVERT(184,158,131));
}

/*******************************************************************************
* Function Name  : Read_Ads7846
* Description    : Get TouchPanel X Y
* Input          : None
* Output         : None
* Return         : Coordinate *
* Attention      : None
*******************************************************************************/
Coordinate *Read_Ads7846(void)
{
    static Coordinate screen;  // Static variable to store the result
    int m0, m1, m2, TP_X[1], TP_Y[1], temp[3];
    alt_u8 count = 0;
    int buffer[2][3] = {{0}, {0}};  // Buffer to store multiple readings

    do
    {
        TP_GetAdXY(TP_X, TP_Y);  // Function to get raw touch panel coordinates
        buffer[0][count] = TP_X[0];
        buffer[1][count] = TP_Y[0];
        count++;
    } while (!TP_INT_IN && count < 3);  // Continue reading until the interrupt occurs or three readings are obtained

    if (count == 3)  // If three readings are obtained
    {
        // Average X
        temp[0] = buffer[0][0];
        temp[1] = buffer[0][1];
        temp[2] = buffer[0][2];

        m0 = temp[0] - temp[1];
        m1 = temp[1] - temp[2];
        m2 = temp[2] - temp[0];

        m0 = m0 > 0 ? m0 : (-m0);
        m1 = m1 > 0 ? m1 : (-m1);
        m2 = m2 > 0 ? m2 : (-m2);

        if (m0 < m1)
        {
            if (m2 < m0)
                screen.x = (temp[0] + temp[2]) / 2;
            else
                screen.x = (temp[0] + temp[1]) / 2;
        }
        else if (m2 < m1)
            screen.x = (temp[0] + temp[2]) / 2;
        else
            screen.x = (temp[1] + temp[2]) / 2;

        // Average Y
        temp[0] = buffer[1][0];
        temp[1] = buffer[1][1];
        temp[2] = buffer[1][2];
        m0 = temp[0] - temp[1];
        m1 = temp[1] - temp[2];
        m2 = temp[2] - temp[0];
        m0 = m0 > 0 ? m0 : (-m0);
        m1 = m1 > 0 ? m1 : (-m1);
        m2 = m2 > 0 ? m2 : (-m2);

        if (m0 < m1)
        {
            if (m2 < m0)
                screen.y = (temp[0] + temp[2]) / 2;
            else
                screen.y = (temp[0] + temp[1]) / 2;
        }
        else if (m2 < m1)
            screen.y = (temp[0] + temp[2]) / 2;
        else
            screen.y = (temp[1] + temp[2]) / 2;

        return &screen;  // Return the averaged coordinates
    }

    return 0;  // Return NULL if less than three readings are obtained
}



/*******************************************************************************
* Function Name  : setCalibrationMatrix
* Description    : Calculate K A B C D E F
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
alt_u8 setCalibrationMatrix(Coordinate *displayPtr, Coordinate *screenPtr, Matrix *matrixPtr)
{
    alt_u8 retTHRESHOLD = 1; // Default return value, set to 1 if successful
    matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                         ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y));

    if (matrixPtr->Divider == 0)
    {
        retTHRESHOLD = 0; // Set return value to 0 if the division is not possible
    }
    else
    {
        // Calculate calibration matrix coefficients
        matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                        ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y));

        matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) -
                        ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x));

        matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                        (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                        (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y;

        matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) -
                        ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y));

        matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) -
                        ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x));

        matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                        (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                        (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y;
    }
    return (retTHRESHOLD);
}


/*******************************************************************************
* Function Name  : getDisplayPoint
* Description    : Touch panel X Y to display X Y
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
alt_u8 getDisplayPoint(Coordinate *displayPtr, Coordinate *screenPtr, Matrix *matrixPtr)
{
    alt_u8 retTHRESHOLD = 1; // Default return value, set to 1 if successful

    if (matrixPtr->Divider != 0)
    {
        // XD = AX + BY + C
        displayPtr->x = ((matrixPtr->An * screenPtr->x) +
                         (matrixPtr->Bn * screenPtr->y) +
                         matrixPtr->Cn) / matrixPtr->Divider;

        // YD = DX + EY + F
        displayPtr->y = ((matrixPtr->Dn * screenPtr->x) +
                         (matrixPtr->En * screenPtr->y) +
                         matrixPtr->Fn) / matrixPtr->Divider;
    }
    else
    {
        retTHRESHOLD = 0; // Set return value to 0 if the division is not possible (Divider is 0)
    }

    return retTHRESHOLD;
}


/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void TouchPanel_Calibrate(void)
{
    alt_u8 i;
    Coordinate *Ptr;

    for (i = 0; i < 3; i++)
    {
        LCD_Clear(Cyan);  // Clear the LCD with Cyan color
        GUI_Text(44, 10, "Touch crosshair to calibrate", Red, White);  // Display calibration instructions
        delay_ms(500);  // Wait for 500 milliseconds
        DrawCross(DisplaySample[i].x, DisplaySample[i].y);  // Draw a crosshair at a specific location on the screen

        do
        {
            Ptr = Read_Ads7846();  // Read raw touch coordinates
        } while (Ptr == (void *)0);  // Repeat until valid touch data is obtained

        ScreenSample[i].x = Ptr->x;  // Store the raw touch x-coordinate
        ScreenSample[i].y = Ptr->y;  // Store the raw touch y-coordinate
    }

    // Calculate the calibration matrix using the collected samples
    setCalibrationMatrix(&DisplaySample[0], &ScreenSample[0], &matrix);

    // Clear the LCD with a custom color (RGB565CONVERT(200, 200, 120))
    LCD_Clear(RGB565CONVERT(200, 200, 120));
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
