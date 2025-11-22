#pragma once
#include "../OverlayPatch.h"

/// @brief Arm9 overlay patch to disable the Pokemon Black & White IR-sensor anti-piracy.
class PokemonBw1IrApPatch : public OverlayPatch
{
public:
    const void* InsertPatch(PatchContext& patchContext) override;
};