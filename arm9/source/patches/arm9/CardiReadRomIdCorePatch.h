#pragma once
#include "../Patch.h"

class FunctionSignature;

/// @brief Arm9 patch to avoid card id reads.
class CardiReadRomIdCorePatch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    bool CheckSignature(const PatchContext& patchContext, const FunctionSignature& signature);

    u32* _cardiReadRomIdCore = nullptr;
    u16 _thumb = false;
};
