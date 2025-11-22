#include "common.h"
#include "PatchContext.h"
#include "PatchCollection.h"

bool PatchCollection::TryPerformPatches(PatchContext& patchContext)
{
    LOG_DEBUG("PatchCollection::PerformPatches\n");
    Patch* cur = _head;
    while (cur)
    {
        if (!cur->FindPatchTarget(patchContext))
        {
            LOG_DEBUG("Patch target not found!\n");
            return false;
        }
        cur = cur->next;
    }

    cur = _head;
    while (cur)
    {
        cur->ApplyPatch(patchContext);
        cur = cur->next;
    }

    patchContext.GetPatchCodeCollection().CopyAllToTarget();
    return true;
}
