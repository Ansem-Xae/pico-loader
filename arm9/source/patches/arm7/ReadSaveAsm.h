#pragma once
#include "sections.h"
#include "../PatchCode.h"
#include "../SectorRemapPatchCode.h"
#include "../platform/SdReadPatchCode.h"

DEFINE_SECTION_SYMBOLS(readsave);

extern "C" void readsave_asm(u32, u32 saveSrc, void* memoryDst, u32 byteLength);

extern u32 readsave_tmpBufferPtr;
extern u32 readsave_save_offset_to_sd_sector_asm_address;
extern u32 readsave_sdread_asm_address;

class ReadSavePatchCode : public PatchCode
{
public:
    ReadSavePatchCode(PatchHeap& patchHeap, const SectorRemapPatchCode* sectorRemapPatchCode,
        const SdReadPatchCode* sdReadPatchCode, void* tmpBuffer)
        : PatchCode(SECTION_START(readsave), SECTION_SIZE(readsave), patchHeap)
    {
        readsave_save_offset_to_sd_sector_asm_address = (u32)sectorRemapPatchCode->GetRemapFunction();
        readsave_sdread_asm_address = (u32)sdReadPatchCode->GetSdReadFunction();
        readsave_tmpBufferPtr = (u32)tmpBuffer;
    }

    const void* GetReadSaveFunction() const
    {
        return GetAddressAtTarget((void*)readsave_asm);
    }
};
