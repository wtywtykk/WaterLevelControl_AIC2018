#include "Global.h"
#include "Clock.h"

ushort PWMGenMax = 0;

ushort PWMGenGetMax(void)
{
	return PWMGenMax + 1;
}

void PWMGenSetOutputDutyx10000(ushort Duty)
{
	if (Duty > 10000)
	{
		Duty = 10000;
	}
	TA2CCR1 = (ulong)Duty * (PWMGenMax + 1) / 10000;
}

void InitPWMGen(void)
{
	PWMGenMax = Clock_Query(SMCLK) / 10000 - 1;//10kHz
	TA2EX0 = 0;
	TA2CTL = TASSEL__SMCLK | ID__1 | MC__STOP;
	TA2CCTL0 = 0;
	TA2CCTL1 = CM_0 | CCIS_2 | OUTMOD_7;
	TA2CCR0 = PWMGenMax;
	TA2CCR1 = 0;

	TA2CTL |= TACLR;
	TA2CTL |= MC__UP;
	//P2.4
	P2DIR |= BIT4;
	P2DS |= BIT4;
	P2OUT &= ~BIT4;
	P2SEL |= BIT4;
}
