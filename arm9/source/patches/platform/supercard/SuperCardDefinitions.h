#pragma once
#include "common.h"

#define NUM_STARTUP_CLOCKS  30000
#define MAX_STARTUP_TRIES   5000
#define SD_OCR_VALUE        0x00030000
#define RESPONSE_TIMEOUT    256
#define SD_STATE_STBY       3
#define SD_STATE_TRAN       4
#define READY_FOR_DATA      1

#define SD_RESET_ADDR       (*(vu16*)0x09440000)

#define SC_MODE_REG         (*(vu16*)0x09FFFFFE)
#define SC_MODE_MAGIC       0xA55A

#define SC_MODE_SDRAM       0x0001
#define SC_MODE_SDCARD      0x0002

#define REG_SCSD_CMD16      (*(vu16*)0x09800000)
#define REG_SCSD_CMD32      (*(vu32*)0x09800000)
