#pragma once
#include "sections.h"
#include "LoaderInfo.h"
#include "patches/PatchCode.h"

DEFINE_SECTION_SYMBOLS(patch_bootstub);

extern IReadSectorsPatchCode::ReadSectorsFunc patch_bootstub_arm9reboot_readSdSectors_address;
extern const void* patch_bootstub_arm9reboot_dldi_address;
extern const char* patch_bootstub_arm9reboot_launcher_path;
extern loader_info_t patch_bootstub_arm9reboot_loader_info;

extern "C" void patch_bootstub_arm9reboot(void);
extern "C" void patch_bootstub_arm7reboot(void);

class BootstubPatchCode : public PatchCode
{
public:
    BootstubPatchCode(PatchHeap& patchHeap, void* dldi, const char* launcherPath, const loader_info_t* loaderInfo,
        const IReadSectorsPatchCode* readSectorsPatchCode)
        : PatchCode(SECTION_START(patch_bootstub), SECTION_SIZE(patch_bootstub), patchHeap)
    {
        patch_bootstub_arm9reboot_dldi_address = dldi;
        patch_bootstub_arm9reboot_readSdSectors_address = readSectorsPatchCode->GetReadSectorsFunction();
        patch_bootstub_arm9reboot_launcher_path = launcherPath;
        patch_bootstub_arm9reboot_loader_info = *loaderInfo;
    }

    const void* GetArm9RebootFunction() const
    {
        return GetAddressAtTarget((void*)patch_bootstub_arm9reboot);
    }

    const void* GetArm7RebootFunction() const
    {
        return GetAddressAtTarget((void*)patch_bootstub_arm7reboot);
    }
};
