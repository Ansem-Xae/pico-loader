#pragma once
#include "../Patch.h"

/// @brief Arm7 patch to disabling clearing of wram.
class DisableArm7WramClearPatch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u32* _clearEndPool = nullptr;
};
