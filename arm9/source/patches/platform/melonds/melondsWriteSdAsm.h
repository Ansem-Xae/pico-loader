#pragma once
#include "sections.h"
#include "../SdWritePatchCode.h"

DEFINE_SECTION_SYMBOLS(melonds_writesd);

extern "C" void melonds_writeSd(u32 dstSector, const void* src, u32 sectorCount);

class MelonDSWriteSdPatchCode : public SdWritePatchCode
{
public:
    explicit MelonDSWriteSdPatchCode(PatchHeap& patchHeap)
        : SdWritePatchCode(SECTION_START(melonds_writesd), SECTION_SIZE(melonds_writesd), patchHeap) { }

    const SdWriteFunc GetSdWriteFunction() const override
    {
        return (const SdWriteFunc)GetAddressAtTarget((void*)melonds_writeSd);
    }
};
