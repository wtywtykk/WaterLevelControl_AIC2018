#include "Global.h"
#include "Timer.h"
#include "EPDHal.h"

const uchar lut_vcom0[] = {
	0x06,0x06,0x06,0x0A,
	0x0A,0x14,0x06,0x06,
	0x06,0x00,0x00,0x00,
	0x00,0x00,0x00
};
const uchar lut_w[] = {
	0x06,0x46,0x06,0x8A,
	0x4A,0x14,0x86,0x06,
	0x06,0x00,0x00,0x00,
	0x00,0x00,0x00
};
const uchar lut_b[] = {
	0x86,0x06,0x06,0x8A,
	0x4A,0x14,0x06,0x46,
	0x06,0x00,0x00,0x00,
	0x00,0x00,0x00
};
const uchar lut_g1[] = {
	0x86,0x06,0x06,0x8A,
	0x4A,0x14,0x06,0x46,
	0x06,0x00,0x00,0x00,
	0x00,0x00,0x00
};
const uchar lut_g2[] = {
	0x86,0x06,0x06,0x8A,
	0x4A,0x14,0x06,0x46,
	0x06,0x00,0x00,0x00,
	0x00,0x00,0x00
};
const uchar lut_vcom1[] = {
	0x18,0x18,0x02,0x1E,
	0x1E,0x02,0x04,0x28,
	0x05,0x05,0x1E,0x02,
	0x04,0x02,0x01
};
const uchar EPDHal_WriteRedLUT0[] = {
	0x98,0x98,0x02,0x5E,
	0x5E,0x02,0x84,0x68,
	0x05,0x45,0x5E,0x02,
	0x44,0x42,0x01
};
const uchar EPDHal_WriteRedLUT1[] = {
	0x18,0x18,0x02,0x1E,
	0x1E,0x02,0x04,0x28,
	0x05,0x05,0x1E,0x02,
	0x04,0x02,0x01
};

#define CLK_HIGH()    P6OUT|=BIT1
#define CLK_LOW()     P6OUT&=~BIT1
#define CS_HIGH()    P6OUT|=BIT2
#define CS_LOW()     P6OUT&=~BIT2
#define SDI_HIGH()    P6OUT|=BIT3
#define SDI_LOW()     P6OUT&=~BIT3
#define DC_HIGH()     P6OUT|=BIT4
#define DC_LOW()      P6OUT&=~BIT4

void EPDHal_InitIO(void)
{
	P6DIR |= BIT1 | BIT2 | BIT3 | BIT4;
	CS_HIGH();
	CLK_HIGH();
}

void EPDHal_SPI_Write(uchar value)
{
	uchar i;
	for (i = 0; i < 8; i++)
	{
		CLK_LOW();
		if (value & 0x80)
		{
			SDI_HIGH();
		}
		else
		{
			SDI_LOW();
		}
		value <<= 1;
		CLK_HIGH();
	}
}

void EPDHal_WriteCMD(uchar command)
{
	CS_LOW();
	DC_LOW();
	EPDHal_SPI_Write(command);
	CS_HIGH();
}

void EPDHal_WriteDAT(uchar dat)
{
	CS_LOW();
	DC_HIGH();
	EPDHal_SPI_Write(dat);
	CS_HIGH();
}

void EPDHal_WriteBlackLUT(void)
{
	uchar count;
	EPDHal_WriteCMD(0x20);
	for (count = 0; count < 15; count++)
	{
		EPDHal_WriteDAT(lut_vcom0[count]);
	}

	EPDHal_WriteCMD(0x21);
	for (count = 0; count < 15; count++)
	{
		EPDHal_WriteDAT(lut_w[count]);
	}

	EPDHal_WriteCMD(0x22);
	for (count = 0; count < 15; count++)
	{
		EPDHal_WriteDAT(lut_b[count]);
	}

	EPDHal_WriteCMD(0x23);
	for (count = 0; count < 15; count++)
	{
		EPDHal_WriteDAT(lut_g1[count]);
	}

	EPDHal_WriteCMD(0x24);
	for (count = 0; count < 15; count++)
	{
		EPDHal_WriteDAT(lut_g2[count]);
	}
}

void EPDHal_WriteRedLUT(void)
{
	uchar count;
	EPDHal_WriteCMD(0x25);
	for (count = 0; count < 15; count++)
	{
		EPDHal_WriteDAT(lut_vcom1[count]);
	}

	EPDHal_WriteCMD(0x26);
	for (count = 0; count < 15; count++)
	{
		EPDHal_WriteDAT(EPDHal_WriteRedLUT0[count]);
	}

	EPDHal_WriteCMD(0x27);
	for (count = 0; count < 15; count++)
	{
		EPDHal_WriteDAT(EPDHal_WriteRedLUT1[count]);
	}
}

void EPDHal_Init(void)
{
	EPDHal_InitIO();
	EPDHal_WriteCMD(0x01);
	EPDHal_WriteDAT(0x07);//set high/low voltage
	EPDHal_WriteDAT(0x00);
	EPDHal_WriteDAT(0x0a);
	EPDHal_WriteDAT(0x00);
	EPDHal_WriteCMD(0x06);//boost config
	EPDHal_WriteDAT(0x07);
	EPDHal_WriteDAT(0x07);
	EPDHal_WriteDAT(0x07);
	EPDHal_WriteCMD(0x04);//power up
	Delay(100);
	EPDHal_WriteCMD(0X00);
	EPDHal_WriteDAT(0xcf);//select max resolution
	EPDHal_WriteCMD(0X50);
	EPDHal_WriteDAT(0x37);
	EPDHal_WriteCMD(0x30);//PLL config
	EPDHal_WriteDAT(0x29);
	EPDHal_WriteCMD(0x61);//pixel setting
	EPDHal_WriteDAT(0x68);
	EPDHal_WriteDAT(0x00);
	EPDHal_WriteDAT(0xd4);
	EPDHal_WriteCMD(0x82);//vcom setting
	EPDHal_WriteDAT(0x0a);
	EPDHal_WriteBlackLUT();
	EPDHal_WriteRedLUT();
}

void EPDHal_Refresh(void)
{
	EPDHal_WriteCMD(0x12);
}

void EPDHal_DeInit(void)
{
	EPDHal_WriteCMD(0x82);//to solve Vcom drop
	EPDHal_WriteDAT(0x00);
	EPDHal_WriteCMD(0x01);//power setting
	EPDHal_WriteDAT(0x02);//gate switch to external
	EPDHal_WriteDAT(0x00);
	EPDHal_WriteDAT(0x00);
	EPDHal_WriteDAT(0x00);
	EPDHal_WriteCMD(0X02);//power off
}

void EPDHal_WriteBlack(uchar* Buf)
{
	ushort i;
	EPDHal_WriteCMD(0x10);
	for (i = 0; i < EPD_DataCount; i++)
	{
		EPDHal_WriteDAT(~Buf[i]);
	}
}

void EPDHal_WriteRed(uchar* Buf)
{
	ushort i;
	EPDHal_WriteCMD(0x13);
	for (i = 0; i < EPD_DataCount; i++)
	{
		EPDHal_WriteDAT(~Buf[i]);
	}
}
