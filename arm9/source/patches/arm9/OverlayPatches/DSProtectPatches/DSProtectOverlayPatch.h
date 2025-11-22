#pragma once
#include "../OverlayPatch.h"
#include "DSProtectVersion.h"

/// @brief Arm9 overlay patch for DS Protect.
class DSProtectOverlayPatch : public OverlayPatch
{
public:
    DSProtectOverlayPatch(u32 overlayId, u32 overlayOffset, DSProtectVersion version, u32 functionMask)
        : _overlayId(overlayId), _overlayOffset(overlayOffset), _version(version), _functionMask(functionMask) { }

    const void* InsertPatch(PatchContext& patchContext) override;

private:
    u32 _overlayId;
    u32 _overlayOffset;
    DSProtectVersion _version;
    u32 _functionMask;

    void SetOldV1Offsets(const u16* offsets) const;
    const void* InsertPatchV1(PatchContext& patchContext) const;
    const void* InsertPatchV2(PatchContext& patchContext) const;
    const void* InsertPatchV2s(PatchContext& patchContext) const;
};
