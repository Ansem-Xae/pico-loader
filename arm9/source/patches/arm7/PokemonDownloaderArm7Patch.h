#pragma once
#include "../Patch.h"
#include "LoaderInfo.h"

/// @brief Arm7 patch to make the Pokemon downloader reboot into Pico Loader.
class PokemonDownloaderArm7Patch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u32* _bootFunction = nullptr;
};
