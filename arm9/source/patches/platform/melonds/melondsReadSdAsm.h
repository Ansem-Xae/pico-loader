#pragma once
#include "sections.h"
#include "../SdReadPatchCode.h"

DEFINE_SECTION_SYMBOLS(melonds_readsd);

extern "C" void melonds_readSd(u32 srcSector, void* dst, u32 sectorCount);

class MelonDSReadSdPatchCode : public SdReadPatchCode
{
public:
    explicit MelonDSReadSdPatchCode(PatchHeap& patchHeap)
        : SdReadPatchCode(SECTION_START(melonds_readsd), SECTION_SIZE(melonds_readsd), patchHeap) { }

    const SdReadFunc GetSdReadFunction() const override
    {
        return (const SdReadFunc)GetAddressAtTarget((void*)melonds_readSd);
    }
};
