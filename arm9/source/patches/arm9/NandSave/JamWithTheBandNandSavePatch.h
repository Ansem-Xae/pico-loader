#pragma once
#include "patches/Patch.h"

class FunctionSignature;

/// @brief Arm9 patch to redirect Jam with the Band nand saving to the SD card.
class JamWithTheBandNandSavePatch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;
};
