#pragma once
#include "patches/PatchCode.h"
#include "sections.h"
#include "patches/SaveOffsetToSdSectorAsm.h"
#include "patches/platform/SdReadPatchCode.h"

DEFINE_SECTION_SYMBOLS(patch_readnandsave);

extern "C" bool patch_readNandSave(void* dst, u32 nandByteOffset, u32 byteLength, u32 dmaChannel);

extern u32 patch_readNandSave_save_offset_to_sd_sector_asm_address;
extern u32 patch_readNandSave_sdread_asm_address;

class ReadNandSavePatchCode : public PatchCode
{
public:
    ReadNandSavePatchCode(PatchHeap& patchHeap, const SaveOffsetToSdSectorPatchCode* saveOffsetToSdSectorPatchCode,
        const SdReadPatchCode* sdReadPatchCode)
        : PatchCode(SECTION_START(patch_readnandsave), SECTION_SIZE(patch_readnandsave), patchHeap)
    {
        patch_readNandSave_save_offset_to_sd_sector_asm_address = (u32)saveOffsetToSdSectorPatchCode->GetRemapFunction();
        patch_readNandSave_sdread_asm_address = (u32)sdReadPatchCode->GetSdReadFunction();
    }

    const void* GetReadNandSaveFunction() const
    {
        return GetAddressAtTarget((void*)patch_readNandSave);
    }
};
