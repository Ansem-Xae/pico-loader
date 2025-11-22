#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "../SdWritePatchCode.h"
#include "../acekard-common/iorpgSendSdioCommandAsm.h"
#include "../acekard-common/iorpgSdWaitForStateAsm.h"

DEFINE_SECTION_SYMBOLS(ak2_writesd);

extern "C" void ak2_writeSd(u32 srcSector, void* dst, u32 sectorCount);

extern u32 ak2_writeSd_sendSdioCommand_address;
extern u32 ak2_writeSd_sdWaitForState_address;
extern u16 ak2_writeSd_sdsc_shift;

class AK2WriteSdPatchCode : public SdWritePatchCode
{
public:
    explicit AK2WriteSdPatchCode(PatchHeap& patchHeap,
        const ioRPGSendSDIOCommandPatchCode* iorpgSendSdioCommandPatchCode,
        const ioRPGSDWaitForStatePatchCode* iorpgSdWaitForStatePatchCode)
        : SdWritePatchCode(SECTION_START(ak2_writesd), SECTION_SIZE(ak2_writesd), patchHeap)
    {
        ak2_writeSd_sendSdioCommand_address = (u32)iorpgSendSdioCommandPatchCode->GetSendSdioCommandFunction();
        ak2_writeSd_sdWaitForState_address = (u32)iorpgSdWaitForStatePatchCode->GetSDWaitForStateFunction();
    }

    const SdWriteFunc GetSdWriteFunction() const override
    {
        return (const SdWriteFunc)GetAddressAtTarget((void*)ak2_writeSd);
    }
};
