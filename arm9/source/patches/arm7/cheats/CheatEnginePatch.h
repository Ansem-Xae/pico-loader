#pragma once
#include "patches/Patch.h"

/// @brief Arm7 patch for injecting the cheat engine in the vblank interrupt handler.
class CheatEnginePatch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

    void SetCheats(const void* cheats)
    {
        _cheats = cheats;
    }

private:
    const void* _cheats = nullptr;
    u32* _vblankIrqHandler = nullptr;
    const u32* _foundPattern = nullptr;
};
