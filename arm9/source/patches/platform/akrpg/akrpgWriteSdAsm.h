#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "../SdWritePatchCode.h"
#include "../acekard-common/iorpgSendSdioCommandAsm.h"
#include "../acekard-common/iorpgSdWaitForStateAsm.h"

DEFINE_SECTION_SYMBOLS(akrpg_writesd);

extern "C" void akrpg_writeSd(u32 srcSector, void* dst, u32 sectorCount);

extern u32 akrpg_writeSd_sendSdioCommand_address;
extern u32 akrpg_writeSd_sdWaitForState_address;
extern u16 akrpg_writeSd_sdsc_shift;

class AKRPGWriteSdPatchCode : public SdWritePatchCode
{
public:
    explicit AKRPGWriteSdPatchCode(PatchHeap& patchHeap,
        const ioRPGSendSDIOCommandPatchCode* iorpgSendSdioCommandPatchCode,
        const ioRPGSDWaitForStatePatchCode* iorpgSdWaitForStatePatchCode)
        : SdWritePatchCode(SECTION_START(akrpg_writesd), SECTION_SIZE(akrpg_writesd), patchHeap)
    {
        akrpg_writeSd_sendSdioCommand_address = (u32)iorpgSendSdioCommandPatchCode->GetSendSdioCommandFunction();
        akrpg_writeSd_sdWaitForState_address = (u32)iorpgSdWaitForStatePatchCode->GetSDWaitForStateFunction();
    }

    const SdWriteFunc GetSdWriteFunction() const override
    {
        return (const SdWriteFunc)GetAddressAtTarget((void*)akrpg_writeSd);
    }
};
