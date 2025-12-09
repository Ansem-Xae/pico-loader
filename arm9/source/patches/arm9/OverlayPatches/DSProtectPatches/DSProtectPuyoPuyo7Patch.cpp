#include "common.h"
#include "../../../PatchContext.h"
#include "DSProtectPuyoPuyo7PatchAsm.h"
#include "DSProtectPuyoPuyo7Patch.h"

const void* DSProtectPuyoPuyo7Patch::InsertPatch(PatchContext& patchContext)
{
    // Next patch
    puyopuyo7patch_nextAddress = next ? (const void*)next->InsertPatch(patchContext) : nullptr;

    u32 patchSize = SECTION_SIZE(puyopuyo7patch);
    void* patchAddress = patchContext.GetPatchHeap().Alloc(patchSize);
    u32 entryAddress = (u32)&puyopuyo7patch_entry - (u32)SECTION_START(puyopuyo7patch) + (u32)patchAddress;
    memcpy(patchAddress, SECTION_START(puyopuyo7patch), patchSize);

    return (const void*)entryAddress;
}
