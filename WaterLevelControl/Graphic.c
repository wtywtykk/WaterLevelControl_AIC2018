#include "Global.h"
#include "VisualGDB/Debug/ResourceHeaders/EmbeddedResources.h"
#include "Graphic.h"
#include <string.h>

#if USEOLED
#include "OLEDBuffered.h"
#define Driver_Init OLEDBuffered_Init
#define Driver_Clear OLEDBuffered_Clear
#define Driver_Fulsh OLEDBuffered_BufferFulsh
#define Driver_SetPixel OLEDBuffered_BufferWriteBit
#define Driver_PowerOn()
#define Driver_PowerOff()
#else
#include "EPDBuffered.h"
#define Driver_Init EPDBuffered_Init
#define Driver_Clear EPDBuffered_Clear
#define Driver_Fulsh EPDBuffered_BufferFulsh
#define Driver_SetPixel EPDBuffered_BufferWriteBit
#define Driver_PowerOn() EPDHal_Init()
#define Driver_PowerOff() EPDHal_DeInit()
#endif

#define Font_DataPtr ((uchar*)&_binary_ASC16_start)
#define Font_Width 8
#define Font_Height 16
#define Font_ByteSize (8 * 16 / 8)
#define Font_StartChar 0x0
#define Font_LastChar 0x7F
#define Font_DefaultChar '?'

void Graphic_Init(void)
{
	Driver_Init();
	Driver_Clear();
}

void Graphic_Clear(void)
{
	Driver_Clear();
}

void Graphic_WriteChar(uchar X, uchar Y, char c, uchar CharVal, uchar BgVal)
{
	uchar* FontData = NULL;
	uchar BitCounter = 0;
	uchar ByteVal = 0;
	uchar CharX = 0;
	uchar CharY = 0;
	if (c<Font_StartChar || c>Font_LastChar)
	{
		c = Font_DefaultChar;
	}
	FontData = Font_DataPtr + (c - Font_StartChar) * Font_ByteSize;
	for (CharY = 0; CharY < Font_Height; CharY++)
	{
		BitCounter = 0;
		ByteVal = *FontData;
		for (CharX = 0; CharX < Font_Width; CharX++)
		{
			if (ByteVal & 0x80)
			{
				Driver_SetPixel(X + CharX, Y + CharY, CharVal);
			}
			else
			{
				Driver_SetPixel(X + CharX, Y + CharY, BgVal);
			}
			BitCounter++;
			ByteVal <<= 1;
			if (BitCounter == 8)
			{
				FontData++;
				BitCounter = 0;
				ByteVal = *FontData;
			}
		}
		if (BitCounter)
		{
			FontData++;
		}
	}
}

void Graphic_WriteStr(uchar X, uchar Y, const char *Str, uchar CharVal, uchar BgVal)
{
	assert(X < Graphic_ScreenWidth && Y < Graphic_ScreenHeight);
	while (*Str)
	{
		if (X > Graphic_ScreenWidth - Font_Width)
		{
			Y += Font_Height;
			X = 0;
		}
		if (Y > Graphic_ScreenHeight - Font_Height)
		{
			Y = 0;
		}
		Graphic_WriteChar(X, Y, *Str, CharVal, BgVal);
		Str++;
		X += Font_Width;
	}
}

void Graphic_EstimateStr(const char* Str, uchar* Width, uchar* Height)
{
	if (Width)
	{
		*Width = strlen((char*)Str) * Font_Width;
	}
	if (Height)
	{
		*Height = Font_Height;
	}
}

void Graphic_BufferFulsh(void)
{
	Driver_Fulsh();
}

void Graphic_Fill(uchar X, uchar Y, uchar Width, uchar Height, uchar Val)
{
	uchar CurY, DestY, CurX, DestX;
	DestX = X + Width;
	DestY = Y + Height;
	if (X > Graphic_ScreenWidth || Y > Graphic_ScreenHeight)
	{
		return;
	}
	if (DestX > Graphic_ScreenWidth)
	{
		DestX = Graphic_ScreenWidth;
	}
	if (DestY > Graphic_ScreenHeight)
	{
		DestY = Graphic_ScreenHeight;
	}
	for (CurY = Y; CurY < DestY; CurY++)
	{
		for (CurX = X; CurX < DestX; CurX++)
		{
			Driver_SetPixel(CurX, CurY, Val);
		}
	}
}

void Graphic_Bitblt(uchar* Src, uchar DstX, uchar DstY, uchar Width, uchar Height, uchar CharVal, uchar BgVal)
{
	uint X, Y;
	uchar* SrcPtr = Src;
	for (Y = 0; Y < Height; Y++)
	{
		uint PixelCounter = 0;
		for (X = 0; X < Width; X++)
		{
			ushort PixelX = (ushort)X + DstX;
			ushort PixelY = (ushort)Y + DstY;
			if (PixelX < Graphic_ScreenWidth && PixelY < Graphic_ScreenHeight)
			{
				if (0x80 & (*SrcPtr << PixelCounter))
				{
					Driver_SetPixel(PixelX, PixelY, CharVal);
				}
				else
				{
					Driver_SetPixel(PixelX, PixelY, BgVal);
				}
			}
			PixelCounter++;
			if (PixelCounter == 8)
			{
				PixelCounter = 0;
				SrcPtr++;
			}
		}
		if (PixelCounter)
		{
			SrcPtr++;
		}
	}
}

void Graphic_DrawLine(uchar X1, uchar Y1, uchar X2, uchar Y2, uchar Val)
{
	short xerr = 0, yerr = 0, delta_x, delta_y, distance;
	short incx, incy, uRow, uCol;
	ushort i = 0;
	delta_x = X2 - X1;
	delta_y = Y2 - Y1;
	uRow = X1;
	uCol = Y1;
	if (delta_x > 0)
	{
		incx = 1;
	}
	else if (delta_x == 0)
	{
		incx = 0;
	}
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
	{
		incy = 1;
	}
	else if (delta_y == 0)
	{
		incy = 0;
	}
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
	{
		distance = delta_x;
	}
	else
	{
		distance = delta_y;
	}
	for (i = 0; i <= distance + 1; i++)
	{
		Driver_SetPixel(uRow, uCol, Val);
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

void Graphic_DrawCircle(uchar X0, uchar Y0, uchar Radius, uchar Val)
{
	//Midpoint circle algorithm
	//https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
	short X = Radius;
	short Y = 0;
	short err = 0;

	while (X >= Y)
	{
		Driver_SetPixel(X0 + X, Y0 + Y, Val);
		Driver_SetPixel(X0 + Y, Y0 + X, Val);
		Driver_SetPixel(X0 - Y, Y0 + X, Val);
		Driver_SetPixel(X0 - X, Y0 + Y, Val);
		Driver_SetPixel(X0 - X, Y0 - Y, Val);
		Driver_SetPixel(X0 - Y, Y0 - X, Val);
		Driver_SetPixel(X0 + Y, Y0 - X, Val);
		Driver_SetPixel(X0 + X, Y0 - Y, Val);

		if (err <= 0)
		{
			Y += 1;
			err += 2 * Y + 1;
		}
		if (err > 0)
		{
			X -= 1;
			err -= 2 * X + 1;
		}
	}
}

void Graphic_SetPower(uchar On)
{
	if (On)
	{
		Driver_PowerOn();
	}
	else
	{
		Driver_PowerOff();
	}
}
