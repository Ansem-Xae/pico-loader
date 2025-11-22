#include "common.h"
#include "patches/PatchContext.h"
#include "thumbInstructions.h"
#include "CardiReadCardWithHashInternalAsyncPatch.h"

static const u32 sCARDiReadCardWithHashInternalAsyncPattern[] = { 0xE92D40F8u, 0xE59F405Cu, 0xE3E0C000u, 0xE594E508u };
static const u32 sCARDiReadCardWithHashInternalAsyncPattern54[] = { 0xE92D40F8u, 0xE59FC058u, 0xE1A04000u, 0xE59C0508u };
static const u32 sCARDiReadCardWithHashInternalAsyncPatternThumb[] = { 0x1C06B5F8u, 0x1C0D480Eu, 0x20006881u, 0x1C1743C0u };
// static const u32 sCARDiReadCardWithHashInternalAsyncPatternThumb5004E85[] = { 0x1C1CB538u, 0x689D4B0Bu, 0x43DB2300u, 0xD101429Du };

static const u32 sReturnZeroPatchArm[] = { 0xE3A00000, 0xE12FFF1E }; // mov r0, #0; bx lr

bool CardiReadCardWithHashInternalAsyncPatch::FindPatchTarget(PatchContext& patchContext)
{
    _cardiReadCardWithHashInternalAsync = patchContext.FindPattern32Twl(
        sCARDiReadCardWithHashInternalAsyncPattern, sizeof(sCARDiReadCardWithHashInternalAsyncPattern));
    if (!_cardiReadCardWithHashInternalAsync)
    {
        _cardiReadCardWithHashInternalAsync = patchContext.FindPattern32Twl(
            sCARDiReadCardWithHashInternalAsyncPattern54, sizeof(sCARDiReadCardWithHashInternalAsyncPattern54));
    }
    if (!_cardiReadCardWithHashInternalAsync)
    {
        _cardiReadCardWithHashInternalAsync = patchContext.FindPattern32Twl(
            sCARDiReadCardWithHashInternalAsyncPatternThumb, sizeof(sCARDiReadCardWithHashInternalAsyncPatternThumb));
        if (!_cardiReadCardWithHashInternalAsync)
        {
            _cardiReadCardWithHashInternalAsync = patchContext.FindPattern32(
                sCARDiReadCardWithHashInternalAsyncPatternThumb, sizeof(sCARDiReadCardWithHashInternalAsyncPatternThumb));
        }
        // if (!_cardiReadCardWithHashInternalAsync)
        // {
        //     _cardiReadCardWithHashInternalAsync = patchContext.FindPattern32(
        //         sCARDiReadCardWithHashInternalAsyncPatternThumb5004E85, sizeof(sCARDiReadCardWithHashInternalAsyncPatternThumb5004E85));
        // }
        if (_cardiReadCardWithHashInternalAsync)
        {
            _thumb = true;
        }
    }
    if (!_cardiReadCardWithHashInternalAsync)
    {
        LOG_WARNING("CARDi_ReadCardWithHashInternalAsync not found\n");
    }
    return _cardiReadCardWithHashInternalAsync != nullptr;
}

void CardiReadCardWithHashInternalAsyncPatch::ApplyPatch(PatchContext& patchContext)
{
    if (!_cardiReadCardWithHashInternalAsync)
        return;

    if (_thumb)
    {
        ((u16*)_cardiReadCardWithHashInternalAsync)[0] = THUMB_MOVS_IMM(THUMB_R0, 0);
        ((u16*)_cardiReadCardWithHashInternalAsync)[1] = THUMB_BX_LR;
    }
    else
    {
        _cardiReadCardWithHashInternalAsync[0] = sReturnZeroPatchArm[0];
        _cardiReadCardWithHashInternalAsync[1] = sReturnZeroPatchArm[1];
    }
}
