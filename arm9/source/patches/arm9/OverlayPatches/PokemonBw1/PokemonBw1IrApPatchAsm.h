#pragma once
#include "sections.h"
#include "../../../PatchCode.h"

DEFINE_SECTION_SYMBOLS(pokemonbw1irappatch);

extern "C" void pokemonbw1irappatch_entry();

extern u32 pokemonbw1irappatch_nextAddress;

class PokemonBw1IrApPatchCode : public PatchCode
{
public:
    PokemonBw1IrApPatchCode(PatchHeap& patchHeap, const void* nextPatch)
        : PatchCode(SECTION_START(pokemonbw1irappatch), SECTION_SIZE(pokemonbw1irappatch), patchHeap)
    {
        pokemonbw1irappatch_nextAddress = (u32)nextPatch;
    }

    const void* GetPatchFunction() const
    {
        return GetAddressAtTarget((void*)pokemonbw1irappatch_entry);
    }
};
