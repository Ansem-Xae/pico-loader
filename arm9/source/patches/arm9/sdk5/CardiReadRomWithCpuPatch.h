#pragma once
#include "patches/Patch.h"

/// @brief Arm9 patch to redirect card reads on SDK 2-4.
class CardiReadRomWithCpuPatch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u32* _cardiReadRomWithCpu = nullptr;
    u16 _thumb = false;
    const u32* _foundPattern = nullptr;

    void TryPattern(PatchContext& patchContext, const u32* pattern);
};
