#include "common.h"
#include "../../../PatchContext.h"
#include "GoldenSunDarkDawnOverlayHookPatchAsm.h"
#include "GoldenSunDarkDawnOverlayHookPatch.h"

static const u32 sStartOverlayPattern[] = { 0xE92D4038u, 0xE1A05000u, 0xEBFFFF92u, 0xE595101Cu };

bool GoldenSunDarkDawnOverlayHookPatch::FindPatchTarget(PatchContext& patchContext)
{
    _overlayStartFunc = patchContext.FindPattern32(sStartOverlayPattern, sizeof(sStartOverlayPattern));
    return _overlayStartFunc != nullptr;
}

void GoldenSunDarkDawnOverlayHookPatch::ApplyPatch(PatchContext& patchContext)
{
    if (!_overlayStartFunc)
        return;

    if (!_patchHead) // no patches
        return;

    gsddoverlayhookpatch_hookFuncAddress = (u32)_patchHead->InsertPatch(patchContext);
    gsddoverlayhookpatch_returnAddress = (u32)_overlayStartFunc + 0x30;

    u32 patch1Size = SECTION_SIZE(gsddoverlayhookpatch);
    void* patch1Address = patchContext.GetPatchHeap().Alloc(patch1Size);

    u32 entryAddress = (u32)&gsddoverlayhookpatch_entry - (u32)SECTION_START(gsddoverlayhookpatch) + (u32)patch1Address;

    *(u32*)((u8*)_overlayStartFunc + 0x28) = 0xE51FF004; // ldr pc,= entryAddress
    *(u32*)((u8*)_overlayStartFunc + 0x2C) = entryAddress;

    memcpy(patch1Address, SECTION_START(gsddoverlayhookpatch), patch1Size);
}
