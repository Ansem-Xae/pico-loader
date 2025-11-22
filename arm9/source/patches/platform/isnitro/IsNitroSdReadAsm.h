#pragma once
#include "sections.h"
#include "../SdReadPatchCode.h"

DEFINE_SECTION_SYMBOLS(sdread);

extern "C" void sdread_asm(u32 srcSector, void* dst, u32 sectorCount);

extern u32 sdread_asm_agbRamPtr;

class IsNitroSdReadPatchCode : public SdReadPatchCode
{
public:
    IsNitroSdReadPatchCode(PatchHeap& patchHeap, u32 agbRamPtr)
        : SdReadPatchCode(SECTION_START(sdread), SECTION_SIZE(sdread), patchHeap)
    {
        sdread_asm_agbRamPtr = agbRamPtr;
    }

    const SdReadFunc GetSdReadFunction() const override
    {
        return (const SdReadFunc)GetAddressAtTarget((void*)sdread_asm);
    }
};
