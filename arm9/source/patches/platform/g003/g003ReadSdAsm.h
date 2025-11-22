#pragma once
#include "sections.h"
#include "../SdReadPatchCode.h"

DEFINE_SECTION_SYMBOLS(g003_readsd);

extern "C" void g003_readSd(u32 srcSector, void* dst, u32 sectorCount);

class G003ReadSdPatchCode : public SdReadPatchCode
{
public:
    explicit G003ReadSdPatchCode(PatchHeap& patchHeap)
        : SdReadPatchCode(SECTION_START(g003_readsd), SECTION_SIZE(g003_readsd), patchHeap) { }

    const SdReadFunc GetSdReadFunction() const override
    {
        return (const SdReadFunc)GetAddressAtTarget((void*)g003_readSd);
    }
};
