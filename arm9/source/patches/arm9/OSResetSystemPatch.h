#pragma once
#include "../Patch.h"
#include "LoaderInfo.h"

/// @brief Arm9 patch to make OS_ResetSystem reboot into Pico Loader.
class OSResetSystemPatch : public Patch
{
public:
    explicit OSResetSystemPatch(const loader_info_t* loaderInfo)
        : _loaderInfo(loaderInfo) { }

    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

    void** GetCheatsPointerAtTarget() const
    {
        return _cheatsPointer;
    }

private:
    u32* _osResetSystem = nullptr;
    u32 _hybrid = false;
    const loader_info_t* _loaderInfo;
    void** _cheatsPointer = nullptr;
};
