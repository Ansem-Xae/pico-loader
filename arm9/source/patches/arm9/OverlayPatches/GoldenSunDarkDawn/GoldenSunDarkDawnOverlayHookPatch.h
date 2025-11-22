#pragma once
#include "../OverlayHookPatch.h"

/// @brief Arm9 patch to apply patches to overlays when they are loaded in Golden Sun Dark Dawn.
class GoldenSunDarkDawnOverlayHookPatch : public OverlayHookPatch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u32* _overlayStartFunc;
};
