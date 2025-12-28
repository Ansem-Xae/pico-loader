#pragma once
#include "sections.h"
#include "SectorRemapPatchCode.h"
#include "fileInfo.h"

DEFINE_SECTION_SYMBOLS(saveoffsettosdsector);

extern "C" u32 save_offset_to_sd_sector_asm(u32 saveOffset);

extern u32 saveoffsettosdsector_fatDataPtr;

class SaveOffsetToSdSectorPatchCode : public SectorRemapPatchCode
{
public:
    SaveOffsetToSdSectorPatchCode(PatchHeap& patchHeap, const save_file_info_t* fatDataPtr)
        : SectorRemapPatchCode(SECTION_START(saveoffsettosdsector), SECTION_SIZE(saveoffsettosdsector), patchHeap)
    {
        saveoffsettosdsector_fatDataPtr = (u32)fatDataPtr;
    }

    const void* GetRemapFunction() const override
    {
        return GetAddressAtTarget((void*)save_offset_to_sd_sector_asm);
    }
};
