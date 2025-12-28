#pragma once
#include "patches/Patch.h"

class FunctionSignature;

/// @brief Arm9 patch to redirect WarioWare D.I.Y. nand saving to the SD card.
class WarioWareDiyNandSavePatch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u8* _sdPatchEntry = nullptr;
};
