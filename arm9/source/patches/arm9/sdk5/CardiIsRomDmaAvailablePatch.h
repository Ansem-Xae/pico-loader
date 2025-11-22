#pragma once
#include "patches/Patch.h"

/// @brief Arm9 patch to disable DMA card reads on SDK 5.
class CardiIsRomDmaAvailablePatch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u32* _cardiIsRomDmaAvailable = nullptr;
    u16 _thumb = false;
};
