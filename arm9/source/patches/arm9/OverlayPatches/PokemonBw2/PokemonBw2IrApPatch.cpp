#include "common.h"
#include "../../../PatchContext.h"
#include "PokemonBw2IrApPatchAsm.h"
#include "PokemonBw2IrApPatch.h"

const void* PokemonBw2IrApPatch::InsertPatch(PatchContext& patchContext)
{
    return patchContext.GetPatchCodeCollection().AddUniquePatchCode<PokemonBw2IrApPatchCode>
    (
        patchContext.GetPatchHeap(),
        next ? next->InsertPatch(patchContext) : nullptr
    )->GetPatchFunction();
}