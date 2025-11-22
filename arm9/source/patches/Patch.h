#pragma once
#include "PatchContext.h"

/// @brief Abstract class representing a patch intended for the arm9 or arm7 binary.
class Patch
{
public:
    /// @brief Next patch in the singly linked list.
    Patch* next = nullptr;

    /// @brief Tries to find the patch target in the region corresponding
    ///        to the given patchContext.
    /// @param patchContext The patch context to use.
    /// @return True if the patch target was found, or false otherwise.
    virtual bool FindPatchTarget(PatchContext& patchContext) = 0;

    /// @brief Applies the patch to the target that was previously found
    ///        with FindPatchTarget.
    /// @param patchContext The patch context to use.
    virtual void ApplyPatch(PatchContext& patchContext) = 0;
};
