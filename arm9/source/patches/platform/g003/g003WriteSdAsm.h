#pragma once
#include "sections.h"
#include "../SdWritePatchCode.h"

DEFINE_SECTION_SYMBOLS(g003_writesd);

extern "C" void g003_writeSd(u32 dstSector, const void* src, u32 sectorCount);

class G003WriteSdPatchCode : public SdWritePatchCode
{
public:
    explicit G003WriteSdPatchCode(PatchHeap& patchHeap)
        : SdWritePatchCode(SECTION_START(g003_writesd), SECTION_SIZE(g003_writesd), patchHeap) { }

    const SdWriteFunc GetSdWriteFunction() const override
    {
        return (const SdWriteFunc)GetAddressAtTarget((void*)g003_writeSd);
    }
};
