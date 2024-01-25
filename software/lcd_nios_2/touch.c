#include "Touch.h"
#include "LCD32.h"

/* Private variables ---------------------------------------------------------*/
Matrix matrix ;
Coordinate  display ;


Coordinate ScreenSample[3];

Coordinate DisplaySample[3] = {
                                { 50,  45 },
                                                      { 220, 45},
                                { 160,210}
                                };

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
  static Coordinate  screen;
  int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
  alt_u8 count=0;
  int buffer[2][3]={{0},{0}};
  
  do
  {        
    TP_GetAdXY(TP_X,TP_Y);  
    buffer[0][count]=TP_X[0];  
    buffer[1][count]=TP_Y[0];
    count++;  
  }
  while(!TP_INT_IN&& count<3);  /* TP_INT_IN  */
  if(count==3)   /* Average X Y  */ 
  {
    /* Average X  */
  temp[0]=buffer[0][0];
    temp[1]=buffer[0][1];
    temp[2]=buffer[0][2];

    m0=temp[0]-temp[1];
    m1=temp[1]-temp[2];
    m2=temp[2]-temp[0];

    m0=m0>0?m0:(-m0);
  m1=m1>0?m1:(-m1);
    m2=m2>0?m2:(-m2);

  //  if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;

    if(m0<m1)
    {
      if(m2<m0) 
        screen.x=(temp[0]+temp[2])/2;
      else 
        screen.x=(temp[0]+temp[1])/2;   
    }
    else if(m2<m1) 
      screen.x=(temp[0]+temp[2])/2;
    else 
      screen.x=(temp[1]+temp[2])/2;

    /* Average Y  */
  temp[0]=buffer[1][0];
    temp[1]=buffer[1][1];
    temp[2]=buffer[1][2];
    m0=temp[0]-temp[1];
    m1=temp[1]-temp[2];
    m2=temp[2]-temp[0];
    m0=m0>0?m0:(-m0);
    m1=m1>0?m1:(-m1);
    m2=m2>0?m2:(-m2);
    //if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) return 0;

    if(m0<m1)
    {
      if(m2<m0) 
        screen.y=(temp[0]+temp[2])/2;
      else 
        screen.y=(temp[0]+temp[1])/2;   
    }
    else if(m2<m1) 
       screen.y=(temp[0]+temp[2])/2;
    else
       screen.y=(temp[1]+temp[2])/2;

    return &screen;
  }  
  return 0; 
}
     

/*******************************************************************************
* Function Name  : setCalibrationMatrix
* Description    : Calculate K A B C D E F
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
alt_u8 setCalibrationMatrix( Coordinate * displayPtr,
                          Coordinate * screenPtr,
                          Matrix * matrixPtr)
{

  alt_u8 retTHRESHOLD = 1 ;
  /* K£½(X0£­X2) (Y1£­Y2)£­(X1£­X2) (Y0£­Y2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = 0;
  }
  else
  {
    /* A£½((XD0£­XD2) (Y1£­Y2)£­(XD1£­XD2) (Y0£­Y2))£¯K    */
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* B£½((X0£­X2) (XD1£­XD2)£­(XD0£­XD2) (X1£­X2))£¯K    */
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* C£½(Y0(X2XD1£­X1XD2)+Y1(X0XD2£­X2XD0)+Y2(X1XD0£­X0XD1))£¯K */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* D£½((YD0£­YD2) (Y1£­Y2)£­(YD1£­YD2) (Y0£­Y2))£¯K    */
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* E£½((X0£­X2) (YD1£­YD2)£­(YD0£­YD2) (X1£­X2))£¯K    */
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* F£½(Y0(X2YD1£­X1YD2)+Y1(X0YD2£­X2YD0)+Y2(X1YD0£­X0YD1))£¯K */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
  }
  return( retTHRESHOLD ) ;
}

/*******************************************************************************
* Function Name  : getDisplayPoint
* Description    : Touch panel X Y to display X Y
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
alt_u8 getDisplayPoint(Coordinate * displayPtr,
                     Coordinate * screenPtr,
                     Matrix * matrixPtr )
{
  alt_u8 retTHRESHOLD =1 ;

  if( matrixPtr->Divider != 0 )
  {
    /* XD = AX+BY+C */        
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;
    /* YD = DX+EY+F */        
    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;
  }
  else
  {
    retTHRESHOLD = 0;
  }
  return(retTHRESHOLD);
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
  Coordinate * Ptr;

  for(i=0;i<3;i++)
  {
   LCD_Clear(Cyan);
   GUI_Text(44,10,"Touch crosshair to calibrate",Red,White);
   delay_ms(500);
   DrawCross(DisplaySample[i].x,DisplaySample[i].y);
   do
   {
     Ptr=Read_Ads7846();
   }
   while( Ptr == (void*)0 );
   ScreenSample[i].x= Ptr->x; ScreenSample[i].y= Ptr->y;
  }
  setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix );
  LCD_Clear(RGB565CONVERT(200,200,120));
} 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
