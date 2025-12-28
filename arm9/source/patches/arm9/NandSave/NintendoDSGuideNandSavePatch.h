#pragma once
#include "patches/Patch.h"

class FunctionSignature;

/// @brief Arm9 patch to redirect Nintendo DS Guide nand saving to the SD card.
class NintendoDSGuideNandSavePatch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;
};
