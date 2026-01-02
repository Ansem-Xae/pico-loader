#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "../IReadSectorsDmaPatchCode.h"

DEFINE_SECTION_SYMBOLS(g003_readsddma);

extern "C" void g003_readSdDma(u32 srcSector, u32 previousSrcSector, u32 dmaChannel, void* dst);
extern "C" void g003_finishReadSdDma(void);

extern u32 g003_readSdDma_miiCardDmaCopy32Ptr;

class G003ReadSdDmaPatchCode : public PatchCode, public IReadSectorsDmaPatchCode
{
public:
    G003ReadSdDmaPatchCode(PatchHeap& patchHeap, const void* miiCardDmaCopy32Ptr)
        : PatchCode(SECTION_START(g003_readsddma), SECTION_SIZE(g003_readsddma), patchHeap)
    {
        g003_readSdDma_miiCardDmaCopy32Ptr = (u32)miiCardDmaCopy32Ptr;
    }

    const ReadSectorsDmaFunc GetSdReadDmaFunction() const override
    {
        return (const ReadSectorsDmaFunc)GetAddressAtTarget((void*)g003_readSdDma);
    }

    const ReadSectorsDmaFinishFunc GetSdReadDmaFinishFunction() const override
    {
        return (const ReadSectorsDmaFinishFunc)GetAddressAtTarget((void*)g003_finishReadSdDma);
    }
};
