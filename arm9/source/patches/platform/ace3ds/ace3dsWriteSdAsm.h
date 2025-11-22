#pragma once
#include "sections.h"
#include "../SdWritePatchCode.h"

DEFINE_SECTION_SYMBOLS(ace3ds_writesd);

extern "C" void ace3ds_writeSd(u32 dstSector, const void* src, u32 sectorCount);

class Ace3DSWriteSdPatchCode : public SdWritePatchCode
{
public:
    explicit Ace3DSWriteSdPatchCode(PatchHeap& patchHeap)
        : SdWritePatchCode(SECTION_START(ace3ds_writesd), SECTION_SIZE(ace3ds_writesd), patchHeap) { }

    const SdWriteFunc GetSdWriteFunction() const override
    {
        return (const SdWriteFunc)GetAddressAtTarget((void*)ace3ds_writeSd);
    }
};
