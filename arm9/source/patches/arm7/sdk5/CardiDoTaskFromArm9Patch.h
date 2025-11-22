#pragma once
#include "patches/Patch.h"

/// @brief Arm7 patch for redirecting save reads and writes on SDK 5.
class CardiDoTaskFromArm9Patch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u32* _cardiDoTaskFromArm9 = nullptr;
    const u32* _foundPattern = nullptr;
    u16 _thumb = false;

    void TryPattern(PatchContext& patchContext, const u32* pattern);
};