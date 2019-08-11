#pragma once

#define FDC2214_DATA_CH0 0x00
#define FDC2214_DATA_LSB_CH0 0x01
#define FDC2214_DATA_CH1 0x02
#define FDC2214_DATA_LSB_CH1 0x03
#define FDC2214_DATA_CH2 0x04
#define FDC2214_DATA_LSB_CH2 0x05
#define FDC2214_DATA_CH3 0x06
#define FDC2214_DATA_LSB_CH3 0x07
#define FDC2214_RCOUNT_CH0 0x08
#define FDC2214_RCOUNT_CH1 0x09
#define FDC2214_RCOUNT_CH2 0x0A
#define FDC2214_RCOUNT_CH3 0x0B
#define FDC2214_OFFSET_CH0 0x0C
#define FDC2214_OFFSET_CH1 0x0D
#define FDC2214_OFFSET_CH2 0x0E
#define FDC2214_OFFSET_CH3 0x0F
#define FDC2214_SETTLECOUNT_CH0 0x10
#define FDC2214_SETTLECOUNT_CH1 0x11
#define FDC2214_SETTLECOUNT_CH2 0x12
#define FDC2214_SETTLECOUNT_CH3 0x13
#define FDC2214_CLOCK_DIVIDERS_CH0 0x14
#define FDC2214_CLOCK_DIVIDERS_CH1 0x15
#define FDC2214_CLOCK_DIVIDERS_CH2 0x16
#define FDC2214_CLOCK_DIVIDERS_CH3 0x17
#define FDC2214_STATUS 0x18
#define FDC2214_ERROR_CONFIG 0x19
#define FDC2214_CONFIG 0x1A
#define FDC2214_MUX_CONFIG 0x1B
#define FDC2214_RESET_DEV 0x1C
#define FDC2214_DRIVE_CURRENT_CH0 0x1E
#define FDC2214_DRIVE_CURRENT_CH1 0x1F
#define FDC2214_DRIVE_CURRENT_CH2 0x20
#define FDC2214_DRIVE_CURRENT_CH3 0x21
#define FDC2214_MANUFACTURER_ID 0x7E
#define FDC2214_DEVICE_ID 0x7F

#define FDC2214_DATA_ERR_WD_SHIFT 13
#define FDC2214_DATA_ERR_WD_MASK (0x1 << FDC2214_DATA_ERR_WD_SHIFT)
#define FDC2214_DATA_ERR_AW_SHIFT 12
#define FDC2214_DATA_ERR_AW_MASK (0x1 << FDC2214_DATA_ERR_AW_SHIFT)
#define FDC2214_DATA_MSB_SHIFT 0
#define FDC2214_DATA_MSB_MASK (0xFFF << FDC2214_DATA_MSB_SHIFT)

#define FDC2214_CLOCK_DIVIDERS_FIN_SEL_SHIFT 12
#define FDC2214_CLOCK_DIVIDERS_FIN_SEL_MASK (0x3 << FDC2214_CLOCK_DIVIDERS_FIN_SEL_SHIFT)
#define FDC2214_CLOCK_DIVIDERS_FREF_DIVIDER_SHIFT 0
#define FDC2214_CLOCK_DIVIDERS_FREF_DIVIDER_MASK (0x3FF << FDC2214_CLOCK_DIVIDERS_FREF_DIVIDER_SHIFT)

#define FDC2214_STATUS_ERR_CHAN_SHIFT 14
#define FDC2214_STATUS_ERR_CHAN_MASK (0x3 << FDC2214_STATUS_ERR_CHAN_SHIFT)
#define FDC2214_STATUS_ERR_WD_SHIFT 11
#define FDC2214_STATUS_ERR_WD_MASK (0x1 << FDC2214_STATUS_ERR_WD_SHIFT)
#define FDC2214_STATUS_ERR_AHW_SHIFT 10
#define FDC2214_STATUS_ERR_AHW_MASK (0x1 << FDC2214_STATUS_ERR_AHW_SHIFT)
#define FDC2214_STATUS_ERR_ALW_SHIFT 9
#define FDC2214_STATUS_ERR_ALW_MASK (0x1 << FDC2214_STATUS_ERR_ALW_SHIFT)
#define FDC2214_STATUS_DRDY_SHIFT 6
#define FDC2214_STATUS_DRDY_MASK (0x1 << FDC2214_STATUS_DRDY_SHIFT)
#define FDC2214_STATUS_CH0_UNREADCONV_SHIFT 3
#define FDC2214_STATUS_CH0_UNREADCONV_MASK (0x1 << FDC2214_STATUS_CH0_UNREADCONV_SHIFT)
#define FDC2214_STATUS_CH1_UNREADCONV_SHIFT 2
#define FDC2214_STATUS_CH1_UNREADCONV_MASK (0x1 << FDC2214_STATUS_CH1_UNREADCONV_SHIFT)
#define FDC2214_STATUS_CH2_UNREADCONV_SHIFT 1
#define FDC2214_STATUS_CH2_UNREADCONV_MASK (0x1 << FDC2214_STATUS_CH2_UNREADCONV_SHIFT)
#define FDC2214_STATUS_CH3_UNREADCONV_SHIFT 0
#define FDC2214_STATUS_CH3_UNREADCONV_MASK (0x1 << FDC2214_STATUS_CH3_UNREADCONV_SHIFT)

#define FDC2214_ERROR_CONFIG_WD_ERR2OUT_SHIFT 13
#define FDC2214_ERROR_CONFIG_WD_ERR2OUT_MASK (0x1 << FDC2214_ERROR_CONFIG_WD_ERR2OUT_SHIFT)
#define FDC2214_ERROR_CONFIG_AH_WARN2OUT_SHIFT 12
#define FDC2214_ERROR_CONFIG_AH_WARN2OUT_MASK (0x1 << FDC2214_ERROR_CONFIG_AH_WARN2OUT_SHIFT)
#define FDC2214_ERROR_CONFIG_AL_WARN2OUT_SHIFT 11
#define FDC2214_ERROR_CONFIG_AL_WARN2OUT_MASK (0x1 << FDC2214_ERROR_CONFIG_AL_WARN2OUT_SHIFT)
#define FDC2214_ERROR_CONFIG_WD_ERR2INT_SHIFT 5
#define FDC2214_ERROR_CONFIG_WD_ERR2INT_MASK (0x1 << FDC2214_ERROR_CONFIG_WD_ERR2INT_SHIFT)
#define FDC2214_ERROR_CONFIG_DRDY_2INT_SHIFT 0
#define FDC2214_ERROR_CONFIG_DRDY_2INT_MASK (0x1 << FDC2214_ERROR_CONFIG_DRDY_2INT_SHIFT)

#define FDC2214_CONFIG_ACTIVE_CHAN_SHIFT 14
#define FDC2214_CONFIG_ACTIVE_CHAN_MASK (0x3 << FDC2214_CONFIG_ACTIVE_CHAN_SHIFT)
#define FDC2214_CONFIG_SLEEP_MODE_EN_SHIFT 13
#define FDC2214_CONFIG_SLEEP_MODE_EN_MASK (0x1 << FDC2214_CONFIG_SLEEP_MODE_EN_SHIFT)
#define FDC2214_CONFIG_SENSOR_ACTIVATE_SEL_SHIFT 11
#define FDC2214_CONFIG_SENSOR_ACTIVATE_SEL_MASK (0x1 << FDC2214_CONFIG_SENSOR_ACTIVATE_SEL_SHIFT)
#define FDC2214_CONFIG_REF_CLK_SRC_SHIFT 9
#define FDC2214_CONFIG_REF_CLK_SRC_MASK (0x1 << FDC2214_CONFIG_REF_CLK_SRC_SHIFT)
#define FDC2214_CONFIG_INTB_DIS_SHIFT 7
#define FDC2214_CONFIG_INTB_DIS_MASK (0x1 << FDC2214_CONFIG_INTB_DIS_SHIFT)
#define FDC2214_CONFIG_HIGH_CURRENT_DRV_SHIFT 6
#define FDC2214_CONFIG_HIGH_CURRENT_DRV_MASK (0x1 << FDC2214_CONFIG_HIGH_CURRENT_DRV_SHIFT)

#define FDC2214_MUX_CONFIG_AUTOSCAN_EN_SHIFT 15
#define FDC2214_MUX_CONFIG_AUTOSCAN_EN_MASK (0x1 << FDC2214_MUX_CONFIG_AUTOSCAN_EN_SHIFT)
#define FDC2214_MUX_CONFIG_RR_SEQUENCE_SHIFT 13
#define FDC2214_MUX_CONFIG_RR_SEQUENCE_MASK (0x3 << FDC2214_MUX_CONFIG_RR_SEQUENCE_SHIFT)
#define FDC2214_MUX_CONFIG_DEGLITCH_SHIFT 0
#define FDC2214_MUX_CONFIG_DEGLITCH_MASK (0x7 << FDC2214_MUX_CONFIG_DEGLITCH_SHIFT)

#define FDC2214_RESET_DEV_RESET_DEV_SHIFT 15
#define FDC2214_RESET_DEV_RESET_DEV_MASK (0x1 << FDC2214_RESET_DEV_RESET_DEV_SHIFT)
#define FDC2214_RESET_DEV_OUTPUT_GAIN_SHIFT 9
#define FDC2214_RESET_DEV_OUTPUT_GAIN_MASK (0x3 << FDC2214_RESET_DEV_OUTPUT_GAIN_SHIFT)

#define FDC2214_DRIVE_CURRENT_IDRIVE_SHIFT 11
#define FDC2214_DRIVE_CURRENT_IDRIVE_MASK (0x1F << FDC2214_DRIVE_CURRENT_IDRIVE_SHIFT)

#define FDC2214_ID 0x3055
