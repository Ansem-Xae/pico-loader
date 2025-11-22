#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "r4idsnSdReadSectorAsm.h"
#include "../SdReadPatchCode.h"
#include "../acekard-common/iorpgSendSdioCommandAsm.h"
#include "../acekard-common/iorpgSdWaitForStateAsm.h"

DEFINE_SECTION_SYMBOLS(r4idsn_readsd);

extern "C" void r4idsn_readSd(u32 srcSector, void* dst, u32 sectorCount);

extern u32 r4idsn_readSd_sendSdioCommand_address;
extern u32 r4idsn_readSd_sdReadSector_address;
extern u32 r4idsn_readSd_sdWaitForState_address;
extern u16 r4idsn_readSd_sdsc_shift;

class R4iDSNReadSdPatchCode : public SdReadPatchCode
{
public:
    explicit R4iDSNReadSdPatchCode(PatchHeap& patchHeap,
        const ioRPGSendSDIOCommandPatchCode* iorpgSendSdioCommandPatchCode,
        const R4iDSNSDReadSectorPatchCode* r4idsnSdReadSectorPatchCode,
        const ioRPGSDWaitForStatePatchCode* iorpgSdWaitForStatePatchCode)
        : SdReadPatchCode(SECTION_START(r4idsn_readsd), SECTION_SIZE(r4idsn_readsd), patchHeap)
    {
        r4idsn_readSd_sendSdioCommand_address = (u32)iorpgSendSdioCommandPatchCode->GetSendSdioCommandFunction();
        r4idsn_readSd_sdReadSector_address = (u32)r4idsnSdReadSectorPatchCode->GetSDReadSectorFunction();
        r4idsn_readSd_sdWaitForState_address = (u32)iorpgSdWaitForStatePatchCode->GetSDWaitForStateFunction();
    }

    const SdReadFunc GetSdReadFunction() const override
    {
        return (const SdReadFunc)GetAddressAtTarget((void*)r4idsn_readSd);
    }
};
