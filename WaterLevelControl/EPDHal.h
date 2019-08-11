#pragma once

#define EPD_Height 212
#define EPD_Width 104
#define EPD_DataCount (EPD_Height * EPD_Width / 8)

void EPDHal_Init(void);
void EPDHal_Refresh(void);
void EPDHal_DeInit(void);
void EPDHal_WriteBlack(uchar* Buf);
void EPDHal_WriteRed(uchar* Buf);
