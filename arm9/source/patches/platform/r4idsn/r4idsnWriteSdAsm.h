#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "../SdWritePatchCode.h"
#include "../acekard-common/iorpgSendSdioCommandAsm.h"
#include "../acekard-common/iorpgSdWaitForStateAsm.h"

DEFINE_SECTION_SYMBOLS(r4idsn_writesd);

extern "C" void r4idsn_writeSd(u32 srcSector, void* dst, u32 sectorCount);

extern u32 r4idsn_writeSd_sendSdioCommand_address;
extern u32 r4idsn_writeSd_sdWaitForState_address;
extern u16 r4idsn_writeSd_sdsc_shift;

class R4iDSNWriteSdPatchCode : public SdWritePatchCode
{
public:
    explicit R4iDSNWriteSdPatchCode(PatchHeap& patchHeap,
        const ioRPGSendSDIOCommandPatchCode* iorpgSendSdioCommandPatchCode,
        const ioRPGSDWaitForStatePatchCode* iorpgSdWaitForStatePatchCode)
        : SdWritePatchCode(SECTION_START(r4idsn_writesd), SECTION_SIZE(r4idsn_writesd), patchHeap)
    {
        r4idsn_writeSd_sendSdioCommand_address = (u32)iorpgSendSdioCommandPatchCode->GetSendSdioCommandFunction();
        r4idsn_writeSd_sdWaitForState_address = (u32)iorpgSdWaitForStatePatchCode->GetSDWaitForStateFunction();
    }

    const SdWriteFunc GetSdWriteFunction() const override
    {
        return (const SdWriteFunc)GetAddressAtTarget((void*)r4idsn_writeSd);
    }
};
