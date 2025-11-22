#include "common.h"
#include "../../../PatchContext.h"
#include "PokemonBw1IrApPatchAsm.h"
#include "PokemonBw1IrApPatch.h"

const void* PokemonBw1IrApPatch::InsertPatch(PatchContext& patchContext)
{
    return patchContext.GetPatchCodeCollection().AddUniquePatchCode<PokemonBw1IrApPatchCode>
    (
        patchContext.GetPatchHeap(),
        next ? next->InsertPatch(patchContext) : nullptr
    )->GetPatchFunction();
}