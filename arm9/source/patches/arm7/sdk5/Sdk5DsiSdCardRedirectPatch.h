#pragma once
#include "patches/Patch.h"

/// @brief Arm7 patch for redirecting access to the DSi sd card to the flashcard sd card.
class Sdk5DsiSdCardRedirectPatch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u32* _attachFunction = nullptr;
    s16 _blToGetDriveStructOffset = 0;
    u16 _thumb = false;
};
