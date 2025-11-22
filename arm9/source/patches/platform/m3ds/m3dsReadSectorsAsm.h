#pragma once
#include "sections.h"
#include "../SdReadPatchCode.h"

DEFINE_SECTION_SYMBOLS(m3ds_readsdsectors);
DEFINE_SECTION_SYMBOLS(m3ds_readsdsectors_receiveSector);

extern "C" void m3ds_readSdSectors(u32 srcSector, void* dst, u32 sectorCount);
extern "C" void m3ds_receiveSector(void* dst);

extern u32 m3ds_receiveSector_address;

class M3DSReceiveSectorPatchCode : public PatchCode
{
public:
    explicit M3DSReceiveSectorPatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(m3ds_readsdsectors_receiveSector), SECTION_SIZE(m3ds_readsdsectors_receiveSector), patchHeap) { }

    const void* GetReceiveSectorFunction() const
    {
        return GetAddressAtTarget((void*)m3ds_receiveSector);
    }
};

class M3DSReadSdSectorsPatchCode : public SdReadPatchCode
{
public:
    explicit M3DSReadSdSectorsPatchCode(PatchHeap& patchHeap, const M3DSReceiveSectorPatchCode* m3dsReceiveSectorPatchCode)
        : SdReadPatchCode(SECTION_START(m3ds_readsdsectors), SECTION_SIZE(m3ds_readsdsectors), patchHeap)
        {
            m3ds_receiveSector_address = (u32)m3dsReceiveSectorPatchCode->GetReceiveSectorFunction();
        }

    const SdReadFunc GetSdReadFunction() const override
    {
        return (const SdReadFunc)GetAddressAtTarget((void*)m3ds_readSdSectors);
    }
};
