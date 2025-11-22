#pragma once
#include "sections.h"
#include "../SdReadPatchCode.h"

DEFINE_SECTION_SYMBOLS(r4_readrom);

extern "C" void r4_readRom(u32 srcSector, void* dst);

class R4ReadRomPatchCode : public SdReadPatchCode
{
public:
    explicit R4ReadRomPatchCode(PatchHeap& patchHeap)
        : SdReadPatchCode(SECTION_START(r4_readrom), SECTION_SIZE(r4_readrom), patchHeap)
    { }

    const SdReadFunc GetSdReadFunction() const override
    {
        return (const SdReadFunc)GetAddressAtTarget((void*)r4_readRom);
    }
};
