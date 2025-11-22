#pragma once
#include "../../Patch.h"
#include "OverlayPatch.h"

/// @brief Abstract base class for a patch that applies patches to overlays when they are loaded.
class OverlayHookPatch : public Patch
{
public:
    /// @brief Adds an overlay patch to the list.
    /// @param patch The overlay pach to add.
    void AddOverlayPatch(OverlayPatch* patch)
    {
        LOG_DEBUG("OverlayHookPatch::AddOverlayPatch\n");
        if (!_patchHead)
            _patchHead = patch;
        if (_patchTail)
            _patchTail->next = patch;
        _patchTail = patch;
    }

protected:
    OverlayPatch* _patchHead = nullptr;
    OverlayPatch* _patchTail = nullptr;
};
