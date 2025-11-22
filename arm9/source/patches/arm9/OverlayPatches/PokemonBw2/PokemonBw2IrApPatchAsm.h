#pragma once
#include "sections.h"
#include "../../../PatchCode.h"

DEFINE_SECTION_SYMBOLS(pokemonbw2irappatch);

extern "C" void pokemonbw2irappatch_entry();

extern u32 pokemonbw2irappatch_nextAddress;

class PokemonBw2IrApPatchCode : public PatchCode
{
public:
    PokemonBw2IrApPatchCode(PatchHeap& patchHeap, const void* nextPatch)
        : PatchCode(SECTION_START(pokemonbw2irappatch), SECTION_SIZE(pokemonbw2irappatch), patchHeap)
    {
        pokemonbw2irappatch_nextAddress = (u32)nextPatch;
    }

    const void* GetPatchFunction() const
    {
        return GetAddressAtTarget((void*)pokemonbw2irappatch_entry);
    }
};
