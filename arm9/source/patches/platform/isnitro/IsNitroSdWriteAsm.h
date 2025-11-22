#pragma once
#include "sections.h"
#include "../SdWritePatchCode.h"

DEFINE_SECTION_SYMBOLS(sdwrite);

extern "C" void sdwrite_asm(u32 dstSector, const void* src, u32 sectorCount);

extern u32 sdwrite_asm_agbRamPtr;

class IsNitroSdWritePatchCode : public SdWritePatchCode
{
public:
    IsNitroSdWritePatchCode(PatchHeap& patchHeap, u32 agbRamPtr)
        : SdWritePatchCode(SECTION_START(sdwrite), SECTION_SIZE(sdwrite), patchHeap)
    {
        sdwrite_asm_agbRamPtr = agbRamPtr;
    }

    const SdWriteFunc GetSdWriteFunction() const override
    {
        return (const SdWriteFunc)GetAddressAtTarget((void*)sdwrite_asm);
    }
};
