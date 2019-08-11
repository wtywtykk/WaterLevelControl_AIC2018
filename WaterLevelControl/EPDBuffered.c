#include "Global.h"
#include "EPDHal.h"
#include <string.h>

static uchar ImageBuffer[2][EPD_Height][EPD_Width / 8];

#define PixelXAddrFromX(X) ((X)/8)
#define PixelBitMaskFromX(X) (0x80>>(X)%8)

uchar EPDBuffered_BufferRead(uchar Red, uchar XByte, uchar Y)
{
	assert(Red <= 1 && XByte < EPD_Width / 8 && Y < EPD_Height);
	return ImageBuffer[Red][Y][XByte];
}

void EPDBuffered_BufferWrite(uchar XByte, uchar Y, uchar ValBlack, uchar ValRed)
{
	assert(XByte < EPD_Width / 8 && Y < EPD_Height);
	ImageBuffer[0][Y][XByte] = ValBlack;
	ImageBuffer[1][Y][XByte] = ValRed;
}

uchar EPDBuffered_BufferReadBit(uchar X, uchar Y)
{
	uchar Res = 0;
	assert(X < EPD_Width && Y < EPD_Height);
	if (ImageBuffer[0][Y][PixelXAddrFromX(X)] & PixelBitMaskFromX(X))
	{
		Res |= 0x1;
	}
	if (ImageBuffer[1][Y][PixelXAddrFromX(X)] & PixelBitMaskFromX(X))
	{
		Res |= 0x2;
	}
	return Res;
}

void EPDBuffered_BufferWriteBit(uchar X, uchar Y, uchar Val)
{
	uchar Mask = PixelBitMaskFromX(X);
	assert(X < EPD_Width && Y < EPD_Height);
	if (Val & 0x1)
	{
		ImageBuffer[0][Y][PixelXAddrFromX(X)] = ImageBuffer[0][Y][PixelXAddrFromX(X)] | Mask;
	}
	else
	{
		ImageBuffer[0][Y][PixelXAddrFromX(X)] = ImageBuffer[0][Y][PixelXAddrFromX(X)] & ~Mask;
	}
	if (Val & 0x2)
	{
		ImageBuffer[1][Y][PixelXAddrFromX(X)] = ImageBuffer[1][Y][PixelXAddrFromX(X)] | Mask;
	}
	else
	{
		ImageBuffer[1][Y][PixelXAddrFromX(X)] = ImageBuffer[1][Y][PixelXAddrFromX(X)] & ~Mask;
	}
}

void EPDBuffered_BufferFulsh(void)
{
	EPDHal_WriteBlack(&ImageBuffer[0][0][0]);
	EPDHal_WriteRed(&ImageBuffer[1][0][0]);
	EPDHal_Refresh();
}

void EPDBuffered_Init(void)
{
	EPDHal_Init();

	memset(ImageBuffer, 0, sizeof(ImageBuffer));
}

void EPDBuffered_Clear(void)
{
	memset(ImageBuffer, 0, sizeof(ImageBuffer));
}

