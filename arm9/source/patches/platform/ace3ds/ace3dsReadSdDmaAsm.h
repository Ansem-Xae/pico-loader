#pragma once
#include "sections.h"
#include "../SdReadDmaPatchCode.h"

DEFINE_SECTION_SYMBOLS(ace3ds_readsddma);

extern "C" void ace3ds_readSdDma(u32 srcSector, u32 previousSrcSector, u32 dmaChannel, void* dst);
extern "C" void ace3ds_finishReadSdDma(void);

extern u32 ace3ds_readSdDma_miiCardDmaCopy32Ptr;

class Ace3DSReadSdDmaPatchCode : public SdReadDmaPatchCode
{
public:
    explicit Ace3DSReadSdDmaPatchCode(PatchHeap& patchHeap, const void* miiCardDmaCopy32Ptr)
        : SdReadDmaPatchCode(SECTION_START(ace3ds_readsddma), SECTION_SIZE(ace3ds_readsddma), patchHeap)
    {
        ace3ds_readSdDma_miiCardDmaCopy32Ptr = (u32)miiCardDmaCopy32Ptr;
    }

    const SdReadDmaFunc GetSdReadDmaFunction() const override
    {
        return (const SdReadDmaFunc)GetAddressAtTarget((void*)ace3ds_readSdDma);
    }

    const SdReadDmaFinishFunc GetSdReadDmaFinishFunction() const override
    {
        return (const SdReadDmaFinishFunc)GetAddressAtTarget((void*)ace3ds_finishReadSdDma);
    }
};
