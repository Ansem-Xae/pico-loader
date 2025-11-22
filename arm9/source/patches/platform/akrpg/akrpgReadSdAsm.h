#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "akrpgSdReadSectorAsm.h"
#include "../SdReadPatchCode.h"
#include "../acekard-common/iorpgSendSdioCommandAsm.h"
#include "../acekard-common/iorpgSdWaitForStateAsm.h"

DEFINE_SECTION_SYMBOLS(akrpg_readsd);

extern "C" void akrpg_readSd(u32 srcSector, void* dst, u32 sectorCount);

extern u32 akrpg_readSd_sendSdioCommand_address;
extern u32 akrpg_readSd_sdReadSector_address;
extern u32 akrpg_readSd_sdWaitForState_address;
extern u16 akrpg_readSd_sdsc_shift;

class AKRPGReadSdPatchCode : public SdReadPatchCode
{
public:
    explicit AKRPGReadSdPatchCode(PatchHeap& patchHeap,
        const ioRPGSendSDIOCommandPatchCode* iorpgSendSdioCommandPatchCode,
        const AKRPGSDReadSectorPatchCode* akrpgSdReadSectorPatchCode,
        const ioRPGSDWaitForStatePatchCode* iorpgSdWaitForStatePatchCode)
        : SdReadPatchCode(SECTION_START(akrpg_readsd), SECTION_SIZE(akrpg_readsd), patchHeap)
    {
        akrpg_readSd_sendSdioCommand_address = (u32)iorpgSendSdioCommandPatchCode->GetSendSdioCommandFunction();
        akrpg_readSd_sdReadSector_address = (u32)akrpgSdReadSectorPatchCode->GetSDReadSectorFunction();
        akrpg_readSd_sdWaitForState_address = (u32)iorpgSdWaitForStatePatchCode->GetSDWaitForStateFunction();
    }

    const SdReadFunc GetSdReadFunction() const override
    {
        return (const SdReadFunc)GetAddressAtTarget((void*)akrpg_readSd);
    }
};
