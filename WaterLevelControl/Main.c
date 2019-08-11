#include "Global.h"
#include "Clock.h"
#include "PortInterrupts.h"
#include "DMA.h"
#include "Timer.h"
#include "OLED.h"
#include "BoardKey.h"
#include "PWMGen.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "FDC2214.h"
#include "Control.h"
#include "Graphic.h"
#include "UART.h"
#include "DMAADC.h"
#include "InfoStore.h"

typedef enum
{
	EmptyTank,
	TankCalib,
	Ready,
	_90Test,
	_110Test,
	_120Test,
	FreeMode,
}ScreenEnum;
ScreenEnum ScreenStatus = TankCalib;
uchar ScreenClearRequired = 1;

float TargetLevel = 50;
ushort CalibIndex = CalibrationCount - 1;
ulong _90StartTime = 0;

void InitSysClock(void)
{
	Clock clk;
	clk.ACLKDivide = 1;
	clk.ACLKSource = DCODIV;
	clk.DCOFreq = 50000000;
	clk.DCODivider = 2;
	clk.MCLKDivide = 1;
	clk.MCLKSource = DCODIV;
	clk.SMCLKDivide = 1;
	clk.SMCLKSource = DCODIV;
	clk.XT1Freq = 32767;
	clk.XT2Freq = 4000000;
	Clock_Init(&clk);
}

void InitLED(void)
{
	P1DIR |= 1 << 0;
	P4DIR |= 1 << 7;
	P4OUT &= ~(1 << 7);
	P1OUT &= ~(1 << 0);
}

void UpdateLED(void)
{
	if ((GetTickSafe() / 500) % 2)
	{
		P1OUT |= 1 << 0;
		P4OUT &= ~(1 << 7);
	}
	else
	{
		P1OUT &= ~(1 << 0);
		P4OUT |= 1 << 7;
	}
}

void JumpToStatus(ScreenEnum NewStatus)
{
	ScreenStatus = NewStatus;
	switch (ScreenStatus)
	{
	case EmptyTank:
		Control_SetForceMode(-1);
		Control_EnableCalibration(0);
		Control_Set(-300000);
		break;
	case TankCalib:
		Control_SetForceMode(-1);
		Control_EnableCalibration(0);
		CalibIndex = CalibrationCount - 1;
		Control_Set(-300000);
		break;
	case Ready:
		Control_SetForceMode(-1);
		Control_EnableCalibration(1);
		Control_Set(-300000);
		break;
	case _90Test:
		Control_SetForceMode(0);
		Control_EnableCalibration(1);
		TargetLevel = 89;//!!!!!!!!!!!!!!!!!!!!!!hard code to deal with overshoot
		//set target to 89,then 90 after 10 sec.
		Control_Set(TargetLevel);
		_90StartTime = GetTickSafe();
		break;
	case _110Test:
		Control_SetForceMode(0);
		Control_EnableCalibration(1);
		TargetLevel = 110;
		Control_Set(TargetLevel);
		break;
	case _120Test:
		Control_SetForceMode(0);
		Control_EnableCalibration(1);
		TargetLevel = 120;
		Control_Set(TargetLevel);
		break;
	case FreeMode:
		Control_SetForceMode(0);
		Control_EnableCalibration(1);
		Control_Set(TargetLevel);
		break;
	}
	ScreenClearRequired = 1;
}

void CheckKey(void)
{
	//switch (BoardKey_GetLastKey())
	//{
	//case BOARDKEY_RIGHT:
	//	FDC2214_Init();
	//	Control_Init();
	//	return;
	//	break;
	//}

	switch (ScreenStatus)
	{
	case EmptyTank:
		switch (BoardKey_GetLastKey())
		{
		case BOARDKEY_LEFT:
			EraseInfo();
			LoadInfo();
			break;
		case BOARDKEY_RIGHT:
			JumpToStatus(TankCalib);
			break;
		}
		break;
	case TankCalib:
		switch (BoardKey_GetLastKey())
		{
		case BOARDKEY_LEFT:
		{
			Info.LevelCalibration[CalibIndex] = Control_Get();
			if (CalibIndex == 0)
			{
				SaveInfo();
				JumpToStatus(Ready);
			}
			else
			{
				CalibIndex--;
			}
		}
		break;
		case BOARDKEY_RIGHT:
			JumpToStatus(Ready);
			break;
		}
		break;
	case Ready:
		switch (BoardKey_GetLastKey())
		{
		case BOARDKEY_LEFT:
		{
			static uchar PumpOn = 0;
			PumpOn = !PumpOn;
			Control_SetForceMode(PumpOn ? 1 : -1);
		}
		break;
		case BOARDKEY_RIGHT:
			JumpToStatus(_90Test);
			break;
		}
		break;
	case _90Test:
		switch (BoardKey_GetLastKey())
		{
		case BOARDKEY_LEFT:
			break;
		case BOARDKEY_RIGHT:
			JumpToStatus(_110Test);
			break;
		}
		break;
	case _110Test:
		switch (BoardKey_GetLastKey())
		{
		case BOARDKEY_LEFT:
			break;
		case BOARDKEY_RIGHT:
			JumpToStatus(_120Test);
			break;
		}
		break;
	case _120Test:
		switch (BoardKey_GetLastKey())
		{
		case BOARDKEY_LEFT:
			break;
		case BOARDKEY_RIGHT:
			JumpToStatus(FreeMode);
			break;
		}
		break;
	case FreeMode:
		switch (BoardKey_GetLastKey())
		{
		case BOARDKEY_LEFT:
			TargetLevel -= 10;
			break;
		case BOARDKEY_RIGHT:
			TargetLevel += 10;
			break;
		}
		Control_Set(TargetLevel);
		break;
	}
}

void DrawKeyHint(void)
{
	const char* LeftTip = "--";
	const char* RightTip = "--";
	uchar TipWidth = 0;
	switch (ScreenStatus)
	{
	case EmptyTank:
		LeftTip = "Reset";
		RightTip = "Skip";
		break;
	case TankCalib:
		LeftTip = "Conf";
		RightTip = "Skip";
		break;
	case Ready:
		LeftTip = "MAN";
		RightTip = "90mm";
		break;
	case _90Test:
		RightTip = "110mm";
		break;
	case _110Test:
		RightTip = "120mm";
		break;
	case _120Test:
		RightTip = "Free";
		break;
	case FreeMode:
		LeftTip = "-";
		RightTip = "+";
		break;
	}
	OLED_Fill(0, 7 * 8, OLED_Width, 8, 0);
	OLED_WriteStr(0, 7 * 8, LeftTip, 0);
	OLED_EstimateStr(RightTip, &TipWidth, NULL);
	OLED_WriteStr(OLED_Width - TipWidth, 7 * 8, RightTip, 0);
}

void DrawInterface(void)
{
	char sb[32];

	switch (ScreenStatus)
	{
	case EmptyTank:
		OLED_WriteStr(0, 0, "Empty the Tank and wait...", 1);
		break;
	case TankCalib:
		sprintf(sb, "Calibrate level: %d mm", CalibrationLower + (CalibrationUpper - CalibrationLower) * CalibIndex / (CalibrationCount - 1));
		OLED_WriteStr(0, 0, "Calibration", 1);
		OLED_WriteStr(0, 16, sb, 1);
		break;
	case Ready:
		OLED_WriteStr(0, 0, "Ready...", 1);
		break;
	case _90Test:
		OLED_WriteStr(0, 0, "Now testing 90mm", 1);
		break;
	case _110Test:
		OLED_WriteStr(0, 0, "Now testing 110mm", 1);
		break;
	case _120Test:
		OLED_WriteStr(0, 0, "Now testing 120mm", 1);
		break;
	case FreeMode:
		sprintf(sb, "%ldmm   ", (long)(TargetLevel));
		OLED_WriteStr(0, 0, "Free Mode", 1);
		OLED_WriteStr(8, 16, sb, 1);
		break;
	}
}

void UpdateScreen(void)
{
	if (ScreenClearRequired)
	{
		OLED_Clear();
		ScreenClearRequired = 0;
	}
	DrawInterface();
	DrawKeyHint();
	OLED_BufferFulsh();
}

void DoEvents(void)
{
	Control_Process();
	switch (ScreenStatus)
	{
	case EmptyTank:
		if (Control_Get() < -10)
		{
			static uchar i = 0;
			i++;
			if (i >= 30)
			{
				FDC2214_Init();
				Control_Init();
				FDC2214_Init();
				Control_Init();
				JumpToStatus(TankCalib);
			}
		}
		Control_Disp();
		break;
	case TankCalib:
	case Ready:
		Control_Disp();
		break;
	case _90Test:
		Control_Disp();
		if ((_90StartTime != 0) && (_90StartTime + 20000 < GetTickSafe()))
		{
			TargetLevel = 90;
			Control_Set(TargetLevel);
			_90StartTime = 0;
		}
		break;
	case _110Test:
	case _120Test:
	case FreeMode:
		Control_Disp();
		break;
	}
}

ushort Buf[1024];

int main()
{
	WDTCTL = WDTPW | WDTHOLD;
	__enable_interrupt();
	InitSysClock();
	PortInterrupts_Init();
	LoadInfo();
	DMA_Init();
	InitTimer();
	StartTimer();
	InitLED();
	OLED_Init();
	//Graphic_Init();
	BoardKey_Init();
	UART1_Init(115200, NULL);
	InitPWMGen();
	FDC2214_Init();
	Control_Init();

	JumpToStatus(EmptyTank);
	//JumpToStatus(FreeMode);

	while (1)
	{
		UpdateScreen();
		CheckKey();
		DoEvents();
		UpdateLED();
		//FDC2214_MonitorDebug();
		//Delay(500);
	}
}
