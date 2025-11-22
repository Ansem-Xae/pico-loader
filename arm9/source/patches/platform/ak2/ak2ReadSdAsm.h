#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "ak2SdReadSectorAsm.h"
#include "../SdReadPatchCode.h"
#include "../acekard-common/iorpgSendSdioCommandAsm.h"
#include "../acekard-common/iorpgSdWaitForStateAsm.h"

DEFINE_SECTION_SYMBOLS(ak2_readsd);

extern "C" void ak2_readSd(u32 srcSector, void* dst, u32 sectorCount);

extern u32 ak2_readSd_sendSdioCommand_address;
extern u32 ak2_readSd_sdReadSector_address;
extern u32 ak2_readSd_sdWaitForState_address;
extern u16 ak2_readSd_sdsc_shift;

class AK2ReadSdPatchCode : public SdReadPatchCode
{
public:
    explicit AK2ReadSdPatchCode(PatchHeap& patchHeap,
        const ioRPGSendSDIOCommandPatchCode* iorpgSendSdioCommandPatchCode,
        const AK2SDReadSectorPatchCode* ak2SdReadSectorPatchCode,
        const ioRPGSDWaitForStatePatchCode* iorpgSdWaitForStatePatchCode)
        : SdReadPatchCode(SECTION_START(ak2_readsd), SECTION_SIZE(ak2_readsd), patchHeap)
    {
        ak2_readSd_sendSdioCommand_address = (u32)iorpgSendSdioCommandPatchCode->GetSendSdioCommandFunction();
        ak2_readSd_sdReadSector_address = (u32)ak2SdReadSectorPatchCode->GetSDReadSectorFunction();
        ak2_readSd_sdWaitForState_address = (u32)iorpgSdWaitForStatePatchCode->GetSDWaitForStateFunction();
    }

    const SdReadFunc GetSdReadFunction() const override
    {
        return (const SdReadFunc)GetAddressAtTarget((void*)ak2_readSd);
    }
};
