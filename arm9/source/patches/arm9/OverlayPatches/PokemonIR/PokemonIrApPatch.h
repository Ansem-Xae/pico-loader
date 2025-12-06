#pragma once
#include "../OverlayPatch.h"
#include "PokemonIrVersion.h"

/// @brief Arm9 overlay patch to disable the IR-sensor anti-piracy in Pokemon HeartGold, SoulSilver, Black, White, Black 2, and White 2.
class PokemonIrApPatch : public OverlayPatch
{
public:
    PokemonIrApPatch(PokemonIrVersion version) : _version(version) { }
    const void* InsertPatch(PatchContext& patchContext) override;

private:
    PokemonIrVersion _version;

    void ConfigurePatch() const;
};
