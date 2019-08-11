#include "Global.h"
#include "Timer.h"
#include "OLED.h"
#include "BoardKey.h"
#include "PWMGen.h"
#include "FDC2214.h"
#include <stdio.h>
#include <math.h>
#include "InfoStore.h"

//#define FDC_Debug 1

#define PulseDuty 8000
#define PulseDutyCompensate 9000
#define ChannelMask (0x0F)
#define TankCalibPoints 20
#define TankCalibMaxHeight 15

float SetLevel = 5;
float Level = 0;
ulong TankCalibVal[TankCalibPoints] = { 0 };
uchar UseCalibration = 0;
char ForceMode = 0;
float Duty = 0;

void Control_DoMotorControl(void)
{
	static float DutyCount = 0;
	if (Duty > PulseDuty)
	{
		PWMGenSetOutputDutyx10000(Duty);
		DutyCount = 0;
	}
	else
	{
		if (Duty < 3800)
		{
			Duty = 3800;
		}
		DutyCount += Duty;
		if (DutyCount > PulseDuty)
		{
			PWMGenSetOutputDutyx10000(PulseDutyCompensate);
			DutyCount -= PulseDuty;
		}
		else
		{
			PWMGenSetOutputDutyx10000(0);
		}
	}
}

uchar Control_GetMeasureValue(void)
{
	if (FDC2214_GetNewDataMask() & ChannelMask == ChannelMask)
	{
		float CapAir;
		float CapSensor;
		float CapWater;
		float CapWire;
		FDC2214_ClearNewDataMask(0xFF);
		FDC2214_GetChannelCapacitance(0, &CapSensor);
		FDC2214_GetChannelCapacitance(1, &CapWater);
		FDC2214_GetChannelCapacitance(2, &CapAir);
		FDC2214_GetChannelCapacitance(3, &CapWire);
		CapAir -= CapWire;
		CapSensor -= CapWire;
		CapWater -= CapWire;
		if (CapWater > 25)
		{
			Level = 10.0 * (CapSensor / 10 - CapAir) / (CapWater - CapAir);

			Level *= 1000;
			float p1 = 0;
			float p2 = 0;
			float p3 = 7.606e-7;
			float p4 = 0.007282;
			float p5 = 19.68;
			//float p1 = 1.02e-13;
			//float p2 = 1.698e-09;
			//float p3 = 1.473e-05;
			//float p4 = 0.08269;
			//float p5 = 199.6;
			Level = (((p1 * Level + p2) * Level + p3) * Level + p4) * Level + p5;
			Level *= 10;
		}
		else
		{
			Level = -30;
		}

		if (UseCalibration)
		{
			if (Level <= CalibrationLower)
			{
				Level = Level + CalibrationLower - Info.LevelCalibration[0];
			}
			else if (Level >= CalibrationUpper)
			{
				Level = Level + CalibrationUpper - Info.LevelCalibration[CalibrationCount - 1];
			}
			else
			{
				ushort i = floor((CalibrationCount - 1) * (Level - CalibrationLower) / (CalibrationUpper - CalibrationLower));
				Level = (i + (Level - Info.LevelCalibration[i]) / (Info.LevelCalibration[i + 1] - Info.LevelCalibration[i])) * ((CalibrationUpper - CalibrationLower) / (CalibrationCount - 1)) + CalibrationLower;
			}
		}
		return 1;
	}
	return 0;
}

static float Integ = 0;
void ControlTick(void)
{
	if (Control_GetMeasureValue())
	{
		switch (ForceMode)
		{
		case 0:
			if (Level > 150)
			{
				PWMGenSetOutputDutyx10000(0);
				return;
			}
			else
			{
				float PRatio = 3000;
				float IRatio = 2;
				float DRatio = 10000;
				float DDRatio = 10000;
				float IBounds = 50;
				float IScale = 1;

				float Error = Level - SetLevel;
				static float LastError = 0;
				static float LastD = 0;

				if (Level > 100)
				{
					PRatio = 3000;
					IScale = 3;
				}
				else if (Level > 80)
				{
					PRatio = 3000;
					IScale = 4;
				}
				else if (Level > 40)
				{
					PRatio = 5000;
					IScale = 6;
				}
				else
				{
					PRatio = 6000;
					IScale = 9;
				}

				if (fabs(Error) > 2)
				{
					if (PRatio < 6000)
					{
						PRatio = 6000;
					}
					Integ *= 0.9;
				}
				if (fabs(Error) > 3)
				{
					PRatio = 1000000;
				}

				Duty = 5000;
				Duty -= PRatio * Error + DRatio * (Error - LastError) + DDRatio * (Error - LastError - LastD);
				if (Integ > 0)
				{
					Duty -= IRatio * Integ * Integ;
				}
				else
				{
					Duty -= -IRatio * Integ * Integ;
				}
				LastError = Error;
				LastD = Error - LastError;
				Integ += Error / IScale;
				if (Integ < -IBounds)
				{
					Integ = -IBounds;
				}
				if (Integ > IBounds)
				{
					Integ = IBounds;
				}
				if (Duty > 10000)
				{
					Duty = 10000;
				}
			}
			break;
		case 1:
			Duty = 10000;
			break;
		case -1:
			Duty = 0;
			break;
		}
		Control_DoMotorControl();
	}
}

void Control_DebugDisp(void)
{
	uchar i = 0;
#ifdef FDC_Debug
	FDC2214_GetData();
#endif

	for (i = 0; i < 4; i++)
	{
		float vF;
		float vC;
		uchar Ret = FDC2214_GetChannelFreq(i, &vF);
		FDC2214_GetChannelCapacitance(i, &vC);
		printf("%d %dF %ld C %ld\r\n", i, Ret, (long)(vF), (long)(1000 * vC));
	}
}

void Control_Init(void)
{
	FDC2214_SetDeglitch(DEGLITCH_BW_10M);
	FDC2214_SetClockSource(1, 40000000);
	FDC2214_SetINTB(0);
	FDC2214_SetCurrentMode(0, 0);
	FDC2214_SetChannelConfig(0, 0x1000, 0x00A0, 1, MULTI_CH_0M01_8M75, DRIVE_1p571mA);
	FDC2214_SetChannelConfig(1, 0x1000, 0x00A0, 1, MULTI_CH_0M01_8M75, DRIVE_1p571mA);
	FDC2214_SetChannelConfig(2, 0x1000, 0x00A0, 1, MULTI_CH_0M01_8M75, DRIVE_1p571mA);
	FDC2214_SetChannelConfig(3, 0x1000, 0x00A0, 1, MULTI_CH_0M01_8M75, DRIVE_1p571mA);
	FDC2214_SetConvertChannel(1, 3);
	FDC2214_SetSleepMode(0);

#ifndef FDC_Debug
	Timer_Register(Timer1ms, ControlTick);
#endif
}

void Control_Process(void)
{
	FDC2214_GetData();
}

void lftoa(char* Buf, double Val)
{
	long IntPart;
	long FloatPart;
	if (Val < 0)
	{
		Buf[0] = '-';
		Val = -Val;
	}
	else
	{
		Buf[0] = ' ';
	}
	Buf++;
	IntPart = Val;
	FloatPart = 10.0 * (Val - IntPart);
	sprintf(Buf, "%3ld.%01ld", IntPart, FloatPart);
}

void Control_Disp(void)
{
	char sb[32];
	char nsb[32];
	//Control_DebugDisp();
	lftoa(nsb, Level);
	sprintf(sb, "Level: %s mm  ", nsb);
	OLED_WriteStr(0, 40, sb, 1);
	//printf("%ld %ld\r\n", (long)Duty, (long)Integ);
	printf("%ld\r\n", (long)(Level * 1000));
}

void Control_Set(float NewLevel)
{
	SetLevel = NewLevel;
}

float Control_Get(void)
{
	return Level;
}

void Control_EnableCalibration(uchar Val)
{
	UseCalibration = Val;
}

void Control_SetForceMode(char Val)
{
	ForceMode = Val;
}
