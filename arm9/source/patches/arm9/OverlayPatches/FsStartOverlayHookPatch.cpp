#include "common.h"
#include "../../PatchContext.h"
#include "thumbInstructions.h"
#include "FsStartOverlayHookPatch.h"

static const u32 sFSStartOverlayPatternSdk4[] = { 0xE59F10D0u, 0xE1A04000u, 0xE1D100B0u, 0xE3500002u }; // -0xC
static const u32 sFSStartOverlayPatternSdk4Thumb[] = { 0x481F1C06u, 0x28028800u, 0x69E8D121u, 0x0E012700u }; // -8
static const u32 sFSStartOverlayPattern[] = { 0xE3500001u, 0x0A00001Cu, 0xE595001Cu, 0xE3A03000u }; // -0x14
static const u32 sFSStartOverlayPatternThumb[] = { 0x69E8D023u, 0x0E012600u, 0x42082002u, 0x491BD013u }; // -0x10
static const u32 sFSStartOverlayPatternThumbHybrid[] = { 0x68691C07u, 0x42814828u, 0x4828D30Cu, 0xD2094281u }; // -8

extern "C" void fsstartoverlayhook_entry();

extern u8 __fsstartoverlayhook_start[];
extern u8 __fsstartoverlayhook_end[];

extern u32 fsstartoverlayhook_dcFlushRangeOffset;
extern u32 fsstartoverlayhook_hookFuncAddress;

bool FsStartOverlayHookPatch::FindPatchTarget(PatchContext& patchContext)
{
    if (!_patchHead) // no patches
        return true;

    if (patchContext.GetSdkVersion().GetMajor() <= 4)
    {
        _fsStartOverlay = patchContext.FindPattern32(sFSStartOverlayPatternSdk4, sizeof(sFSStartOverlayPatternSdk4));
        if (_fsStartOverlay)
        {
            _fsStartOverlay -= 3;
        }
        else
        {
            _fsStartOverlay = patchContext.FindPattern32(sFSStartOverlayPatternSdk4Thumb, sizeof(sFSStartOverlayPatternSdk4Thumb));
            if (_fsStartOverlay)
            {
                _fsStartOverlay -= 2;
                _thumb = true;
            }
        }
    }
    else
    {
        _fsStartOverlay = patchContext.FindPattern32(sFSStartOverlayPattern, sizeof(sFSStartOverlayPattern));
        if (_fsStartOverlay)
        {
            _fsStartOverlay -= 5;
        }
        else
        {
            _fsStartOverlay = patchContext.FindPattern32(sFSStartOverlayPatternThumb, sizeof(sFSStartOverlayPatternThumb));
            if (_fsStartOverlay)
            {
                _fsStartOverlay -= 4;
                _thumb = true;
            }
            else
            {
                _fsStartOverlay = patchContext.FindPattern32(sFSStartOverlayPatternThumbHybrid, sizeof(sFSStartOverlayPatternThumbHybrid));
                if (_fsStartOverlay)
                {
                    _fsStartOverlay -= 2;
                    _thumb = true;
                    _hybrid = true;
                }
            }
        }
    }

    if (_fsStartOverlay)
    {
        LOG_DEBUG("FS_StartOverlay found at 0x%p\n", _fsStartOverlay);
    }
    else
    {
        LOG_WARNING("FS_StartOverlay not found\n");
    }

    return _fsStartOverlay != nullptr;
}

void FsStartOverlayHookPatch::ApplyPatch(PatchContext& patchContext)
{
    if (!_fsStartOverlay)
        return;

    if (!_patchHead) // no patches
        return;

    const void* firstPatch = _patchHead->InsertPatch(patchContext);

    if (!firstPatch)
        return;

    fsstartoverlayhook_hookFuncAddress = (u32)firstPatch;

    u32 patchOffset;
    if (_thumb)
    {
        u32 blDcFlushRange1;
        u32 blDcFlushRange2;
        if (_hybrid)
        {
            patchOffset = 0x8E;
            blDcFlushRange1 = *(u16*)((u8*)_fsStartOverlay + 0x92);
            blDcFlushRange2 = *(u16*)((u8*)_fsStartOverlay + 0x94);
        }
        else
        {
            if (patchContext.GetSdkVersion().GetMajor() <= 4)
            {
                patchOffset = 0x68;
                blDcFlushRange1 = *(u16*)((u8*)_fsStartOverlay + 0x6C);
                blDcFlushRange2 = *(u16*)((u8*)_fsStartOverlay + 0x6E);
            }
            else
            {
                patchOffset = 0x6C;
                blDcFlushRange1 = *(u16*)((u8*)_fsStartOverlay + 0x70);
                blDcFlushRange2 = *(u16*)((u8*)_fsStartOverlay + 0x72);
            }
        }
        fsstartoverlayhook_dcFlushRangeOffset = ((int)((((blDcFlushRange1 & 0x7FF) << 11) | (blDcFlushRange2 & 0x7FF)) << 10) >> 9) - (_hybrid ? 5 : 1);
    }
    else
    {
        patchOffset = 0xAC;
        u32 blDcFlushRange = *(u32*)((u8*)_fsStartOverlay + 0xB0);
        fsstartoverlayhook_dcFlushRangeOffset = (int)((blDcFlushRange & 0xFFFFFF) << 8) >> 6;
    }

    u32 patch1Size = (u32)__fsstartoverlayhook_end - (u32)__fsstartoverlayhook_start;
    void* patch1Address = patchContext.GetPatchHeap().Alloc(patch1Size);

    u32 entryAddress = (u32)&fsstartoverlayhook_entry - (u32)__fsstartoverlayhook_start + (u32)patch1Address;
    memcpy(patch1Address, __fsstartoverlayhook_start, patch1Size);

    if (_thumb)
    {
        if (_hybrid)
        {
            *(u16*)((u8*)_fsStartOverlay + patchOffset + 0) = THUMB_LDR_PC_IMM(THUMB_R0, 4);
            *(u16*)((u8*)_fsStartOverlay + patchOffset + 2) = THUMB_NOP;
            *(u16*)((u8*)_fsStartOverlay + patchOffset + 4) = THUMB_BLX(THUMB_R0);
            *(u32*)((u8*)_fsStartOverlay + patchOffset + 6) = entryAddress;
        }
        else
        {
            *(u16*)((u8*)_fsStartOverlay + patchOffset + 0) = THUMB_LDR_PC_IMM(THUMB_R0, 0);
            *(u16*)((u8*)_fsStartOverlay + patchOffset + 2) = THUMB_BLX(THUMB_R0);
            *(u32*)((u8*)_fsStartOverlay + patchOffset + 4) = entryAddress;
        }
    }
    else
    {
        *(u32*)((u8*)_fsStartOverlay + patchOffset + 0) = 0xE59F0000; // ldr r0,= entryAddress
        *(u32*)((u8*)_fsStartOverlay + patchOffset + 4) = 0xE12FFF30; // blx r0
        *(u32*)((u8*)_fsStartOverlay + patchOffset + 8) = entryAddress;
    }
}
