#include "common.h"
#include "../../../PatchContext.h"
#include "PokemonIrApPatchAsm.h"
#include "PokemonIrApPatch.h"

const void* PokemonIrApPatch::InsertPatch(PatchContext& patchContext)
{
    ConfigurePatch();
    return patchContext.GetPatchCodeCollection().AddUniquePatchCode<PokemonIrApPatchCode>
    (
        patchContext.GetPatchHeap(),
        next ? next->InsertPatch(patchContext) : nullptr
    )->GetPatchFunction();
}

void PokemonIrApPatch::ConfigurePatch() const {
    switch (_version)
    {
        // All of these offsets are luckily region-independent
        case PokemonIrVersion::Hgss:
        {
            pokemonirappatch_overlayId = 1;
            pokemonirappatch_offset = 0xD2C + 0x12;
            break;
        }
        case PokemonIrVersion::Bw1:
        {
            pokemonirappatch_overlayId = 231;
            pokemonirappatch_offset = 0x1B8 + 0x12;
            break;
        }
        case PokemonIrVersion::Bw2:
        {
            pokemonirappatch_overlayId = 338;
            pokemonirappatch_offset = 0x1B8 + 0x12;
            break;
        }
        default:
        {
            LOG_WARNING("Unsupported Pokemon IR version\n");
            pokemonirappatch_overlayId = 0xFFFFFFFF;
            break;
        }
    }
}
