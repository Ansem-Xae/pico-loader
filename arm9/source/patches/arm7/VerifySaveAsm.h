#pragma once
#include "sections.h"
#include "../PatchCode.h"
#include "../SectorRemapPatchCode.h"
#include "../platform/SdReadPatchCode.h"

DEFINE_SECTION_SYMBOLS(verifysave);

extern "C" void verifysave_asm(u32, u32 saveSrc, void* memoryDst, u32 byteLength);

extern u32 verifysave_tmpBufferPtr;
extern u32 verifysave_save_offset_to_sd_sector_asm_address;
extern u32 verifysave_sdread_asm_address;

class VerifySavePatchCode : public PatchCode
{
public:
    VerifySavePatchCode(PatchHeap& patchHeap, const SectorRemapPatchCode* sectorRemapPatchCode,
        const SdReadPatchCode* sdReadPatchCode, void* tmpBuffer)
        : PatchCode(SECTION_START(verifysave), SECTION_SIZE(verifysave), patchHeap)
    {
        verifysave_save_offset_to_sd_sector_asm_address = (u32)sectorRemapPatchCode->GetRemapFunction();
        verifysave_sdread_asm_address = (u32)sdReadPatchCode->GetSdReadFunction();
        verifysave_tmpBufferPtr = (u32)tmpBuffer;
    }

    const void* GetVerifySaveFunction() const
    {
        return GetAddressAtTarget((void*)verifysave_asm);
    }
};
