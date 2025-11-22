#pragma once
#include "sections.h"
#include "../SdWritePatchCode.h"

DEFINE_SECTION_SYMBOLS(dspico_writesdsectors);

extern "C" void dspico_writeSdSectors(u32 dstSector, const void* src, u32 sectorCount);

class DSPicoWriteSdSectorsPatchCode : public SdWritePatchCode
{
public:
    explicit DSPicoWriteSdSectorsPatchCode(PatchHeap& patchHeap)
        : SdWritePatchCode(SECTION_START(dspico_writesdsectors), SECTION_SIZE(dspico_writesdsectors), patchHeap) { }

    const SdWriteFunc GetSdWriteFunction() const override
    {
        return (const SdWriteFunc)GetAddressAtTarget((void*)dspico_writeSdSectors);
    }
};
