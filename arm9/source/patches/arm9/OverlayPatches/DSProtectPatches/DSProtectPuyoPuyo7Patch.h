#pragma once
#include "../OverlayPatch.h"

/// @brief Arm9 overlay patch for DS Protect specifically for Puyo Puyo 7.
class DSProtectPuyoPuyo7Patch : public OverlayPatch
{
public:
    const void* InsertPatch(PatchContext& patchContext) override;
};
