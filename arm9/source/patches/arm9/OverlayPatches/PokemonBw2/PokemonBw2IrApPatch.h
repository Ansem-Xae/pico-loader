#pragma once
#include "../OverlayPatch.h"

/// @brief Arm9 overlay patch to disable the Pokemon Black & White 2 IR-sensor anti-piracy.
class PokemonBw2IrApPatch : public OverlayPatch
{
public:
    const void* InsertPatch(PatchContext& patchContext) override;
};