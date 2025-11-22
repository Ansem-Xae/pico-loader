#pragma once
#include "../PatchCode.h"
#include "sections.h"

DEFINE_SECTION_SYMBOLS(patch_pokemondownloader7);

extern "C" void patch_pokemondownloader7_entry(void);

class PokemonDownloaderArm7PatchCode : public PatchCode
{
public:
    explicit PokemonDownloaderArm7PatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(patch_pokemondownloader7), SECTION_SIZE(patch_pokemondownloader7), patchHeap) { }

    const void* GetBoot7Function() const
    {
        return GetAddressAtTarget((void*)patch_pokemondownloader7_entry);
    }
};
