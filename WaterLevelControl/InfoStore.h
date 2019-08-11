#pragma once

#include "Global.h"

#define CalibrationCount 14
#define CalibrationLower 10
#define CalibrationUpper 140

typedef struct
{
	float LevelCalibration[CalibrationCount];
	ushort Key;
}InfoBlock;

extern InfoBlock Info;

void LoadInfo(void); 
void EraseInfo(void);
void SaveInfo();
