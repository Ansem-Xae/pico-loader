#pragma once
#include "sections.h"
#include "../../../PatchCode.h"

DEFINE_SECTION_SYMBOLS(pokemonirappatch);

extern "C" void pokemonirappatch_entry();

extern u32 pokemonirappatch_overlayId;
extern u32 pokemonirappatch_offset;
extern u32 pokemonirappatch_nextAddress;

class PokemonIrApPatchCode : public PatchCode
{
public:
    PokemonIrApPatchCode(PatchHeap& patchHeap, const void* nextPatch)
        : PatchCode(SECTION_START(pokemonirappatch), SECTION_SIZE(pokemonirappatch), patchHeap)
    {
        pokemonirappatch_nextAddress = (u32)nextPatch;
    }

    const void* GetPatchFunction() const
    {
        return GetAddressAtTarget((void*)pokemonirappatch_entry);
    }
};
