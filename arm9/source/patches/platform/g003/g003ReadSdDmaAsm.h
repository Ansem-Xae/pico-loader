#pragma once
#include "sections.h"
#include "../SdReadDmaPatchCode.h"

DEFINE_SECTION_SYMBOLS(g003_readsddma);

extern "C" void g003_readSdDma(u32 srcSector, u32 previousSrcSector, u32 dmaChannel, void* dst);
extern "C" void g003_finishReadSdDma(void);

extern u32 g003_readSdDma_miiCardDmaCopy32Ptr;

class G003ReadSdDmaPatchCode : public SdReadDmaPatchCode
{
public:
    explicit G003ReadSdDmaPatchCode(PatchHeap& patchHeap, const void* miiCardDmaCopy32Ptr)
        : SdReadDmaPatchCode(SECTION_START(g003_readsddma), SECTION_SIZE(g003_readsddma), patchHeap)
    {
        g003_readSdDma_miiCardDmaCopy32Ptr = (u32)miiCardDmaCopy32Ptr;
    }

    const SdReadDmaFunc GetSdReadDmaFunction() const override
    {
        return (const SdReadDmaFunc)GetAddressAtTarget((void*)g003_readSdDma);
    }

    const SdReadDmaFinishFunc GetSdReadDmaFinishFunction() const override
    {
        return (const SdReadDmaFinishFunc)GetAddressAtTarget((void*)g003_finishReadSdDma);
    }
};
