#pragma once
#include "Patch.h"

/// @brief Class containing a linked list of patches.
class PatchCollection
{
public:
    /// @brief Adds the given \p patch to the collection.
    /// @param patch The patch to add.
    void AddPatch(Patch* patch)
    {
        LOG_DEBUG("PatchCollection::AddPatch\n");
        if (!_head)
        {
            _head = patch;
        }
        if (_tail)
        {
            _tail->next = patch;
        }
        _tail = patch;
    }

    /// @brief Tries to perform all patches in this collection.
    /// @param patchContext The patch context to use.
    /// @return \c true when all patches were successfully performed, or \c false otherwise.
    bool TryPerformPatches(PatchContext& patchContext);

private:
    Patch* _head = nullptr;
    Patch* _tail = nullptr;
};
