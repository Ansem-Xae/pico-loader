#include "common.h"
#include "../PatchContext.h"
#include "PokemonDownloaderArm7PatchAsm.h"
#include "PokemonDownloaderArm7Patch.h"

static const u32 sBootFunctionPattern[] = { 0xE92D4070u, 0xE59F0098u, 0xE5904004u, 0xE3540000u };

bool PokemonDownloaderArm7Patch::FindPatchTarget(PatchContext& patchContext)
{
    _bootFunction = patchContext.FindPattern32(sBootFunctionPattern, sizeof(sBootFunctionPattern));
    return _bootFunction != nullptr;
}

void PokemonDownloaderArm7Patch::ApplyPatch(PatchContext& patchContext)
{
    if (!_bootFunction)
    {
        return;
    }

    auto patchCode = patchContext.GetPatchCodeCollection().AddUniquePatchCode<PokemonDownloaderArm7PatchCode>(
        patchContext.GetPatchHeap());

    _bootFunction[0] = 0xE59F0000; // ldr r0,= address
    _bootFunction[1] = 0xE12FFF10; // bx r0
    _bootFunction[2] = (u32)patchCode->GetBoot7Function(); // address
}
