#pragma once
#include "sections.h"
#include "../SdReadPatchCode.h"

DEFINE_SECTION_SYMBOLS(r4_readsd);

extern "C" void r4_readSd(u32 srcSector, void* dst, u32 sectorCount);

class R4ReadSdPatchCode : public SdReadPatchCode
{
public:
    explicit R4ReadSdPatchCode(PatchHeap& patchHeap)
        : SdReadPatchCode(SECTION_START(r4_readsd), SECTION_SIZE(r4_readsd), patchHeap) { }

    const SdReadFunc GetSdReadFunction() const override
    {
        return (const SdReadFunc)GetAddressAtTarget((void*)r4_readSd);
    }
};
