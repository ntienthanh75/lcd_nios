/*********************************************************************************************************
* File                : game.c
* Hardware Environment:
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : v0.0.1
* By                  : NGUYEN Tien Thanh
* Created             : Jan 18, 2024
*********************************************************************************************************/

#include "LCD32.h"
#include "touch.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
// Define SDRAM base address
#define MAX_POINTS 20
#define BUF_SIZE (50)

/*******************************************************************************
* Function Name  : write file txt
* Description    :
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void write_txt()
{
	printf("NTT Program 123\n");

	printf("running0\n");
	FILE* fp_ascii = NULL;

	printf("running1\n");
	FILE* fp_bin = NULL;

	printf("running2\n");
	char buffer[BUF_SIZE];

	printf("running3\n");
	int read_size, i;

	printf("before open\n");
	fp_ascii = fopen("/mnt/host/hosts_read_ascii.txt", "r");

	printf("running\n");
	if (fp_ascii == NULL){
		printf("Cannot open file\n");
		exit(1);
	}

	printf(fp_ascii);
	fgets(buffer, sizeof(buffer), fp_ascii);
	printf("%s", buffer);
	fclose(fp_ascii);


	printf("openned file\n");
}
/*******************************************************************************
* Function Name  : saveCoordinatesToCSV
* Description    :
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void saveCoordinatesToCSV(const alt_u32 *pointsBuffer, alt_u32 pointsCount) {
    //FILE *file = fopen(filename, "w");
	FILE *file = fopen("/mnt/host/coordinates.csv", "w");
    alt_u32 i;
    if (file != NULL) {
        // Write header if needed
        // fprintf(file, "X,Y\n");

        // Write coordinates
        for (i = 0; i < pointsCount; i++) {
            alt_u16 x = (alt_u16)(pointsBuffer[i] >> 16);
            alt_u16 y = (alt_u16)(pointsBuffer[i] & 0xFFFF);
            fprintf(file, "%u,%u\n", x, y);
        }

        fclose(file);
        printf("Coordinates saved\n");
    } else {
        printf("Error opening the file for writing.\n");
    }
}
/*******************************************************************************
* Function Name  : DisplayNumber
* Description    : Function to handle touch input, draw on the LCD, and store points in SDRAM
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void DrawOnLCDAndStore() {
    Coordinate* touchPoint;

    alt_u32 pointsCount = 0;
    alt_u32 pointsBuffer[MAX_POINTS];  // Each point is a 32-bit value



    while (1) {
        // Wait for touch input
        //while (!TP_INT_IN);

        // Get touch coordinates
        touchPoint = Read_Ads7846();

        if (touchPoint != 0) {
        	pointsCount++;
        	pointsBuffer[pointsCount] = ((alt_u32)touchPoint->x << 16) | touchPoint->y;
        	// Save coordinates to CSV
            saveCoordinatesToCSV(pointsBuffer, pointsCount);
            printf("Number of touches: %lu\n", pointsCount);
        }

        getDisplayPoint(&display, touchPoint, &matrix ) ;
        TP_DrawPoint(display.x,display.y);

        // Draw a point on the LCD
        //LCD_SetPoint(touchPoint->x, touchPoint->y, Red);

        // Store the touched point in SDRAM
        //alt_u32 address = SDRAM_BASE + pointsCount * 2;  // Each coordinate is 2 bytes
        //IOWR_16DIRECT(address, 0, (touchPoint->x << 8) | touchPoint->y);
        //IOWR_16DIRECT(address, 0, (display.x << 8) | display.y);

        //printf("Limited touches: %i\n", MAX_POINTS);

        if (pointsCount >= MAX_POINTS){
            printf("Breaking out of the loop\n");
            break;
        }
    }
}
/*******************************************************************************
* Function Name  : DisplayNumber
* Description    : Function to display a number on the LCD
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void DisplayNumber(int number) {

	LCD_Clear(RGB565CONVERT(200,200,120));
    // Convert the number to a string (assuming a reasonable range)
    char numberStr[10];
    sprintf(numberStr, "%d", number);

    // Display the number on the LCD
    GUI_Text(50, 50, numberStr, White, Black);
}
/*******************************************************************************
* Function Name  : DisplayNumber
* Description    : Function to retrieve and display the stored image from SDRAM
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void DisplayStoredImage() {
	LCD_Clear(RGB565CONVERT(200,200,120));
    alt_u32 i;
    // Iterate through SDRAM and draw each stored point on the LCD
    for (i = 0; i < MAX_POINTS; i++) {
        alt_u32 address = SDRAM_BASE + i * 2;
        alt_u32 point = IORD_16DIRECT(address, 0);

        alt_u16 x = (point >> 8) & 0xFF;
        alt_u16 y = point & 0xFF;

        LCD_SetPoint(x, y, Red);
    }
}


