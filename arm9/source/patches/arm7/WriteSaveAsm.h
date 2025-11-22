#pragma once
#include "sections.h"
#include "../PatchCode.h"
#include "../SectorRemapPatchCode.h"
#include "../platform/SdReadPatchCode.h"
#include "../platform/SdWritePatchCode.h"

DEFINE_SECTION_SYMBOLS(writesave);

extern "C" void writesave_asm(u32, const void* memorySrc, u32 saveDst, u32 byteLength);

extern u32 writesave_tmpBufferPtr;
extern u32 writesave_save_offset_to_sd_sector_asm_address;
extern u32 writesave_sdread_asm_address;
extern u32 writesave_sdwrite_asm_address;

class WriteSavePatchCode : public PatchCode
{
public:
    WriteSavePatchCode(PatchHeap& patchHeap, const SectorRemapPatchCode* sectorRemapPatchCode,
        const SdReadPatchCode* sdReadPatchCode, const SdWritePatchCode* sdWritePatchCode, void* tmpBuffer)
        : PatchCode(SECTION_START(writesave), SECTION_SIZE(writesave), patchHeap)
    {
        writesave_save_offset_to_sd_sector_asm_address = (u32)sectorRemapPatchCode->GetRemapFunction();
        writesave_sdread_asm_address = (u32)sdReadPatchCode->GetSdReadFunction();
        writesave_sdwrite_asm_address = (u32)sdWritePatchCode->GetSdWriteFunction();
        writesave_tmpBufferPtr = (u32)tmpBuffer;
    }

    const void* GetWriteSaveFunction() const
    {
        return GetAddressAtTarget((void*)writesave_asm);
    }
};
