#pragma once
#include "sections.h"
#include "thumbInstructions.h"

DEFINE_SECTION_SYMBOLS(iorpg_sdwaitforstate);

extern "C" void iorpg_sdWaitForState(u32 status_shift, u8 state);

class ioRPGSDWaitForStatePatchCode : public PatchCode
{
public:
    explicit ioRPGSDWaitForStatePatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(iorpg_sdwaitforstate), SECTION_SIZE(iorpg_sdwaitforstate), patchHeap) { }

    const void* GetSDWaitForStateFunction() const
    {
        return GetAddressAtTarget((void*)iorpg_sdWaitForState);
    }
};
