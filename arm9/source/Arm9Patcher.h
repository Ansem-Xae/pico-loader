#pragma once
#include "LoaderInfo.h"

class ApListEntry;
class LoaderPlatform;
class PatchContext;

/// @brief Class for patching the arm9 of retail roms.
class Arm9Patcher
{
public:
    /// @brief Applies arm9 patches using the given \p loaderPlatform.
    /// @param loaderPlatform The loader platform to use.
    /// @param apListEntry The AP list entry for the rom being loaded, or \c nullptr if there is none.
    /// @param isCloneBootRom \c true if the rom being loaded is a clone boot rom, or \c false otherwise.
    /// @param loaderInfo The loader info to use.
    void ApplyPatches(const LoaderPlatform* loaderPlatform, const ApListEntry* apListEntry,
        bool isCloneBootRom, const loader_info_t* loaderInfo) const;

private:
    void AddRestoreCompressedEndPatch(PatchContext& patchContext,
        u32 arm9AutoLoadDoneHookAddress, u32* moduleParamsCompressedEnd, u32 originalCompressedEndValue) const;
    u32 GetAvailableParentSectionSpace() const;
};
