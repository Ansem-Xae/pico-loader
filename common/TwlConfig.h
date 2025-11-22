#pragma once
#include <nds/ndstypes.h>

struct twl_config_t
{
    u32 configFlags;
    u8 field4;
    u8 country;
    u8 language;
    u8 rtcYear;
    s64 rtcOffset;
    u8 eulaAgreeVersion[8];
    u8 field18[2];
    u8 alarmHour;
    u8 alarmMinute;
    u8 field1C[2];
    u8 alarmEnable;
    u8 field1F[2];
    u8 systemMenuUsedTitleSlots;
    u8 systemMenuFreeTitleSlots;
    u8 field23;
    u8 field24;
    u8 field25[3];
    u64 systemMenuLastSelectedTitleId;
    u16 touchCalibrationX1Adc;
    u16 touchCalibrationY1Adc;
    u8 touchCalibrationX1Pixel;
    u8 touchCalibrationY1Pixel;
    u16 touchCalibrationX2Adc;
    u16 touchCalibrationY2Adc;
    u8 touchCalibrationX2Pixel;
    u8 touchCalibrationY2Pixel;
    u32 field3C;
    u8 field40[4];
    u8 favoriteColor;
    u8 field45;
    u8 birthdayMonth;
    u8 birthdayDay;
    u16 nickname[11];
    u16 message[27];
    u8 parentalControlsFlags;
    u8 field95[6];
    u8 parentalControlsRegion;
    u8 parentalControlsAge;
    u8 parentalControlsSecretQuestion;
    u8 parentalControlsUnknown;
    u8 field9F[2];
    u8 parentalControlsPin[5];
    u16 parentalControlsSecretAnswer[65];
};

static_assert(sizeof(twl_config_t) == 0x128);
