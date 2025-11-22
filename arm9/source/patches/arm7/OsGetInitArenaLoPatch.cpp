#include "common.h"
#include "../PatchContext.h"
#include "OsGetInitArenaLoPatch.h"

static const u32 sOSGetInitArenaLoPatternOld[] = { 0xE3A0050Eu, 0xE59F1014u, 0xE351050Eu, 0x81A00001u }; // -0x34
static const u32 sOSGetInitArenaLoPattern[] = { 0xE59F1018u, 0xE3A0050Eu, 0xE351050Eu, 0x81A00001u }; // -0x34
static const u32 sOSGetInitArenaLoPattern2[] = { 0xE59F101Cu, 0xE3A0050Eu, 0xE351050Eu, 0x81A00001u }; // -0x3C
static const u32 sOSGetInitArenaLoPatternThumb[] = { 0xD0042801u, 0xD0042807u, 0xD0092808u, 0x4809E00Fu };
static const u32 sOSGetInitArenaLoPatternThumbSdk50[] = { 0x2801B508u, 0x2807D004u, 0x2808D006u, 0xE010D00Au };

bool OsGetInitArenaLoPatch::FindPatchTarget(PatchContext& patchContext)
{
    if (_osGetInitArenaLo)
        return true;
    _osGetInitArenaLo = patchContext.FindPattern32(sOSGetInitArenaLoPattern, sizeof(sOSGetInitArenaLoPattern));
    if (!_osGetInitArenaLo)
        _osGetInitArenaLo = patchContext.FindPattern32(sOSGetInitArenaLoPatternOld, sizeof(sOSGetInitArenaLoPatternOld));
    if (_osGetInitArenaLo)
    {
        _osGetInitArenaLo -= 13;
        _wramBssOffset = 0x54;
    }
    else
    {
        _osGetInitArenaLo = patchContext.FindPattern32(sOSGetInitArenaLoPattern2, sizeof(sOSGetInitArenaLoPattern2));
        if (_osGetInitArenaLo)
        {
            _osGetInitArenaLo -= 15;
            _wramBssOffset = 0x60;
        }
        else
        {
            _osGetInitArenaLo = patchContext.FindPattern32(sOSGetInitArenaLoPatternThumb, sizeof(sOSGetInitArenaLoPatternThumb));
            if (_osGetInitArenaLo)
            {
                _wramBssOffset = 0x38;
            }
            else
            {
                _osGetInitArenaLo = patchContext.FindPattern32(sOSGetInitArenaLoPatternThumbSdk50, sizeof(sOSGetInitArenaLoPatternThumbSdk50));
                if (_osGetInitArenaLo)
                {
                    _wramBssOffset = 0x40;
                }
                else
                {
                    _osGetInitArenaLo = patchContext.FindPattern32Twl(sOSGetInitArenaLoPattern, sizeof(sOSGetInitArenaLoPattern));
                    if (!_osGetInitArenaLo)
                        _osGetInitArenaLo = patchContext.FindPattern32Twl(sOSGetInitArenaLoPatternOld, sizeof(sOSGetInitArenaLoPatternOld));
                    if (_osGetInitArenaLo)
                    {
                        _osGetInitArenaLo -= 13;
                        _wramBssOffset = 0x54;
                    }
                    else
                    {
                        _osGetInitArenaLo = patchContext.FindPattern32Twl(sOSGetInitArenaLoPattern2, sizeof(sOSGetInitArenaLoPattern2));
                        if (_osGetInitArenaLo)
                        {
                            _osGetInitArenaLo -= 15;
                            _wramBssOffset = 0x60;
                        }
                        else
                        {
                            _osGetInitArenaLo = patchContext.FindPattern32Twl(sOSGetInitArenaLoPatternThumb, sizeof(sOSGetInitArenaLoPatternThumb));
                            if (_osGetInitArenaLo)
                            {
                                _wramBssOffset = 0x38;
                            }
                        }
                    }
                }
            }
        }
    }
    if (_osGetInitArenaLo)
    {
        LOG_DEBUG("Found OS_GetInitArenaLo at %p\n", _osGetInitArenaLo);
        _wramBssEnd = (void*)*(u32*)((u8*)_osGetInitArenaLo + _wramBssOffset);
        LOG_DEBUG("Bss end: 0x%p\n", _wramBssEnd);
        _mainMemoryArenaLo = (void*)*(u32*)((u8*)_osGetInitArenaLo + _wramBssOffset - 4);
        LOG_DEBUG("Main memory arena lo: 0x%p\n", _mainMemoryArenaLo);
    }
    else
    {
        LOG_WARNING("OS_GetInitArenaLo not found\n");
    }
    return _osGetInitArenaLo != nullptr;
}

void OsGetInitArenaLoPatch::ApplyPatch(PatchContext& patchContext)
{
    if (!_osGetInitArenaLo)
        return;
    *(u32*)((u8*)_osGetInitArenaLo + _wramBssOffset) = (u32)_wramBssEnd;
    *(u32*)((u8*)_osGetInitArenaLo + _wramBssOffset - 4) = (u32)_mainMemoryArenaLo;
}
