#pragma once
#include "patches/PatchCode.h"
#include "sections.h"
#include "patches/platform/SdReadPatchCode.h"
#include "patches/platform/SdWritePatchCode.h"

DEFINE_SECTION_SYMBOLS(patch_dsisdredirect);

extern "C" bool __patch_dsisdredirect_io(u32 driveNumber, u32 startSector, void* buffer, u32 sectorCount, bool isRead);
extern "C" u32 __patch_dsisdredirect_control(u32 driveNumber, u32 command, void* argumentBuffer);

extern u32 __patch_dsisdredirect_io_readsd_asm_address;
extern u32 __patch_dsisdredirect_io_writesd_asm_address;
extern u32 __patch_dsisdredirect_control_get_drive_struct_address;

class Sdk5DsiSdCardRedirectPatchCode : public PatchCode
{
public:
    Sdk5DsiSdCardRedirectPatchCode(PatchHeap& patchHeap, const SdReadPatchCode* sdReadPatchCode,
        const SdWritePatchCode* sdWritePatchCode, u32 getDriveStructAddress)
        : PatchCode(SECTION_START(patch_dsisdredirect), SECTION_SIZE(patch_dsisdredirect), patchHeap)
    {
        __patch_dsisdredirect_io_readsd_asm_address = (u32)sdReadPatchCode->GetSdReadFunction();
        __patch_dsisdredirect_io_writesd_asm_address =  (u32)sdWritePatchCode->GetSdWriteFunction();
        __patch_dsisdredirect_control_get_drive_struct_address = getDriveStructAddress;
    }

    const void* GetIoFunction() const
    {
        return GetAddressAtTarget((void*)__patch_dsisdredirect_io);
    }

    const void* GetControlFunction() const
    {
        return GetAddressAtTarget((void*)__patch_dsisdredirect_control);
    }
};