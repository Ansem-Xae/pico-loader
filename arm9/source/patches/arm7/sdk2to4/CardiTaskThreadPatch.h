#pragma once
#include "patches/Patch.h"

class FunctionSignature;

/// @brief Arm7 patch for redirecting save reads and writes on SDK 2-4.
class CardiTaskThreadPatch : public Patch
{
public:
    enum class PatchVariant : u16
    {
        None,
        A,
        B,
        C,
        D,
        E,
        F
    };

    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u32* _cardiTaskThread = nullptr;
    u16 _thumb = false;
    PatchVariant _patchVariant = PatchVariant::None;

    bool CheckSignature(const PatchContext& patchContext, const FunctionSignature& signature);
};