#pragma once
#include "sections.h"
#include "SectorRemapPatchCode.h"

DEFINE_SECTION_SYMBOLS(offsettosectorremap);

extern "C" u32 offset_to_sector_remap(u32 romOffset);

class OffsetToSectorRemapPatchCode : public SectorRemapPatchCode
{
public:
    explicit OffsetToSectorRemapPatchCode(PatchHeap& patchHeap)
        : SectorRemapPatchCode(SECTION_START(offsettosectorremap), SECTION_SIZE(offsettosectorremap), patchHeap)
    { }

    const void* GetRemapFunction() const override
    {
        return GetAddressAtTarget((void*)offset_to_sector_remap);
    }
};
