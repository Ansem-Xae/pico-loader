#pragma once
class PatchContext;

/// @brief Patch to be applied when an overlay is loaded.
class OverlayPatch
{
public:
    /// @brief Pointer to the next overlay patch, or \c nullptr when none.
    OverlayPatch* next = nullptr;

    /// @brief Inserts the patch using the given \p patchContext.
    /// @param patchContext The patch context to use.
    /// @return A pointer to the patch function.
    virtual const void* InsertPatch(PatchContext& patchContext) = 0;
};
