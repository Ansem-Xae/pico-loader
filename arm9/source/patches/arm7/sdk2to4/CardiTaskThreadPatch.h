#pragma once
#include "patches/Patch.h"

class FunctionSignature;

/// @brief Arm7 patch for redirecting save reads and writes on SDK 2-4.
class CardiTaskThreadPatch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u32* _cardiTaskThread = nullptr;
    u16 _thumb = false;
    u16 _peach = false;
    u16 _pokemonDownloader = false;

    bool CheckSignature(const PatchContext& patchContext, const FunctionSignature& signature);
};