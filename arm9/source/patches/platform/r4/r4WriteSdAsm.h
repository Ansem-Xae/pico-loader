#pragma once
#include "sections.h"
#include "../SdWritePatchCode.h"

DEFINE_SECTION_SYMBOLS(r4_writesd);

extern "C" void r4_writeSd(u32 dstSector, const void* src, u32 sectorCount);

class R4WriteSdPatchCode : public SdWritePatchCode
{
public:
    explicit R4WriteSdPatchCode(PatchHeap& patchHeap)
        : SdWritePatchCode(SECTION_START(r4_writesd), SECTION_SIZE(r4_writesd), patchHeap) { }

    const SdWriteFunc GetSdWriteFunction() const override
    {
        return (const SdWriteFunc)GetAddressAtTarget((void*)r4_writeSd);
    }
};
