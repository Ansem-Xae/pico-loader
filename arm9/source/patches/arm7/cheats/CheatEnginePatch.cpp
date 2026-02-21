#include "common.h"
#include "CheatEnginePatchCode.h"
#include "CheatEnginePatch.h"

// sdk2-4
static const u32 sVBlankIntrPatternArm0[] = { 0xE92D4000u, 0xE24DD004u, 0xE59F0018u, 0xE5900000u };
static const u32 sVBlankIntrPatternArm1[] = { 0xE92D4008u, 0xE59F0014u, 0xE5900000u, 0xE3500000u };
static const u32 sVBlankIntrPatternThumb0[] = { 0x00000000u, 0xB081B500u, 0x68004804u, 0xD0012800u }; // +4
static const u32 sVBlankIntrPatternThumb1[] = { 0x46C04770u, 0x027FFE1Du, 0x4804B508u, 0x28006800u }; // +8

bool CheatEnginePatch::FindPatchTarget(PatchContext& patchContext)
{
    _vblankIrqHandler = patchContext.FindPattern32(sVBlankIntrPatternArm0, sizeof(sVBlankIntrPatternArm0));
    if (_vblankIrqHandler)
    {
        _foundPattern = sVBlankIntrPatternArm0;
    }
    if (!_vblankIrqHandler)
    {
        _vblankIrqHandler = patchContext.FindPattern32(sVBlankIntrPatternArm1, sizeof(sVBlankIntrPatternArm1));
        if (_vblankIrqHandler)
        {
            _foundPattern = sVBlankIntrPatternArm1;
        }
    }
    if (!_vblankIrqHandler)
    {
        _vblankIrqHandler = patchContext.FindPattern32(sVBlankIntrPatternThumb0, sizeof(sVBlankIntrPatternThumb0));
        if (_vblankIrqHandler)
        {
            _foundPattern = sVBlankIntrPatternThumb0;
            _vblankIrqHandler += 1;
        }
    }
    if (!_vblankIrqHandler)
    {
        _vblankIrqHandler = patchContext.FindPattern32(sVBlankIntrPatternThumb1, sizeof(sVBlankIntrPatternThumb1));
        if (_vblankIrqHandler)
        {
            _foundPattern = sVBlankIntrPatternThumb1;
            _vblankIrqHandler += 2;
        }
    }

    if (_vblankIrqHandler)
    {
        LOG_DEBUG("ARM7 VBlankIntr found at 0x%p\n", _vblankIrqHandler);
    }

    return _vblankIrqHandler != nullptr;
}

void CheatEnginePatch::ApplyPatch(PatchContext& patchContext)
{
    if (!_vblankIrqHandler || !_cheats)
        return;

    auto cheatEnginePatchCode = patchContext.GetPatchCodeCollection().AddUniquePatchCode<CheatEnginePatchCode>
    (
        patchContext.GetPatchHeap(),
        _cheats
    );

    if (_foundPattern == sVBlankIntrPatternArm0)
    {
        // push {lr}
        // sub sp, sp, #4
        // ldr r0,=
        // ldr r0, [r0]
        // cmp r0, #0
        // beq 1f
        // bl
        // 1:
        // add sp, sp, #4
        // pop {lr}
        // bx lr
        _vblankIrqHandler[7] = 0xE59F0000; // ldr r0,= address
        _vblankIrqHandler[8] = 0xE12FFF10; // bx r0
        _vblankIrqHandler[9] = (u32)cheatEnginePatchCode->GetCheatEngineFunction(); // address
    }
    else if (_foundPattern == sVBlankIntrPatternArm1)
    {
        // push {r3,lr}
        // ldr r0,=
        // ldr r0, [r0]
        // cmp r0, #0
        // beq 1f
        // bl
        // 1:
        // pop {r3,lr}
        // bx lr
    }
    else if (_foundPattern == sVBlankIntrPatternThumb0)
    {
        // push {lr}
        // sub sp, sp, #4
        // ldr r0,=
        // ldr r0, [r0]
        // cmp r0, #0
        // beq 1f
        // bl
        // 1:
        // add sp, sp, #4
        // pop {r3}
        // bx r3
        // nop
    }
    else if (_foundPattern == sVBlankIntrPatternThumb1)
    {
        // push {r3,lr}
        // ldr r0,=
        // ldr r0, [r0]
        // cmp r0, #0
        // beq 1f
        // bl
        // 1:
        // pop {r3}
        // pop {r3}
        // bx r3
    }
    else
    {
        LOG_ERROR("ARM7 VBlankIntr signature not implemented\n");
    }
}
