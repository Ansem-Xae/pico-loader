#include "common.h"
#include "DSProtectArm9Patch.h"
#include "OverlayPatches/DSProtectPatches/DSProtectOverlayPatch.h"
#include "ApList.h"
#include "sharedMemory.h"
#include "ndsHeader.h"

bool DSProtectArm9Patch::FindPatchTarget(PatchContext& patchContext)
{
    return true; // target already specified by _arm9Offset
}

void DSProtectArm9Patch::ApplyPatch(PatchContext& patchContext)
{
    auto patch = new DSProtectOverlayPatch(
        AP_LIST_OVERLAY_ID_STATIC_ARM9, _arm9Offset, _version, _functionMask);
    auto romHeader = (const nds_header_ntr_t*)TWL_SHARED_MEMORY->ntrSharedMem.romHeader;
    patch->ApplyPatchForStaticArm9(romHeader->arm9LoadAddress);
}
