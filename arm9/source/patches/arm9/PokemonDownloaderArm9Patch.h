#pragma once
#include "../Patch.h"
#include "LoaderInfo.h"

/// @brief Arm9 patch to make the Pokemon downloader reboot into Pico Loader.
class PokemonDownloaderArm9Patch : public Patch
{
public:
    explicit PokemonDownloaderArm9Patch(const loader_info_t* loaderInfo)
        : _loaderInfo(loaderInfo) { }

    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u32* _bootFunction = nullptr;
    const loader_info_t* _loaderInfo;
};
