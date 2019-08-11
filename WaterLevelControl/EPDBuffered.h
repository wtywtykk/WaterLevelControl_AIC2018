#pragma once

#include "Global.h"
#include "EPDHal.h"

uchar EPDBuffered_BufferRead(uchar Red, uchar XByte, uchar Y);
void EPDBuffered_BufferWrite(uchar XByte, uchar Y, uchar ValBlack, uchar ValRed);
uchar EPDBuffered_BufferReadBit(uchar X, uchar Y);
void EPDBuffered_BufferWriteBit(uchar X, uchar Y, uchar Val);
void EPDBuffered_BufferFulsh(void);
void EPDBuffered_Init(void);
void EPDBuffered_Clear(void);
