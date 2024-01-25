/*********************************************************************************************************
* File                : game.h
* Hardware Environment:
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : v0.0.1
* By                  : NGUYEN Tien Thanh
* Created             : Jan 18, 2024
*********************************************************************************************************/


#ifndef GAME_H_
#define GAME_H_

void write_txt();
void saveCoordinatesToCSV(const alt_u32 *pointsBuffer, alt_u32 pointsCount);

void DrawOnLCDAndStore();
void DisplayNumber(int number);
void DisplayStoredImage();
#endif /* GAME_H_ */
