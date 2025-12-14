#pragma once
#include "../Patch.h"
#include "DSProtectVersion.h"

/// @brief Arm9 static patch for DS Protect.
class DSProtectArm9Patch : public Patch
{
public:
    DSProtectArm9Patch(u32 arm9Offset, DSProtectVersion version, u32 functionMask)
        : _arm9Offset(arm9Offset), _version(version), _functionMask(functionMask) { }

    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u32 _arm9Offset;
    DSProtectVersion _version;
    u32 _functionMask;
};
