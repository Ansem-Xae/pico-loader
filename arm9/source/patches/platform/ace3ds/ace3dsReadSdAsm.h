#pragma once
#include "sections.h"
#include "../SdReadPatchCode.h"

DEFINE_SECTION_SYMBOLS(ace3ds_readsd);

extern "C" void ace3ds_readSd(u32 srcSector, void* dst, u32 sectorCount);

class Ace3DSReadSdPatchCode : public SdReadPatchCode
{
public:
    explicit Ace3DSReadSdPatchCode(PatchHeap& patchHeap)
        : SdReadPatchCode(SECTION_START(ace3ds_readsd), SECTION_SIZE(ace3ds_readsd), patchHeap) { }

    const SdReadFunc GetSdReadFunction() const override
    {
        return (const SdReadFunc)GetAddressAtTarget((void*)ace3ds_readSd);
    }
};
