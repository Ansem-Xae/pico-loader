#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "../SdReadPatchCode.h"

DEFINE_SECTION_SYMBOLS(scsd_common);

extern "C" void sccmn_changeMode();
extern "C" void sccmn_sdSendClock10();
extern "C" void sccmn_sdio4BitCrc16();

class SuperCardCommonPatchCode : public PatchCode
{
public:
    explicit SuperCardCommonPatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(scsd_common), SECTION_SIZE(scsd_common), patchHeap) { }

    const void* GetScChangeModeFunction() const
    {
        return GetAddressAtTarget((void*)sccmn_changeMode);
    }

    const void* GetSdSendClock10Function() const
    {
        return GetAddressAtTarget((void*)sccmn_sdSendClock10);
    }

    const void* GetCrc16ChecksumFunction() const
    {
        return GetAddressAtTarget((void*)sccmn_sdio4BitCrc16);
    }
};
