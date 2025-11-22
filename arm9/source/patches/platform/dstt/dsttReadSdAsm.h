#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "../SdReadPatchCode.h"

DEFINE_SECTION_SYMBOLS(dstt_readsd);
DEFINE_SECTION_SYMBOLS(dstt_readsd_stopTransmission);

extern "C" void dstt_readSd(u32 srcSector, void* dst, u32 sectorCount);
extern "C" void dstt_stopTransmission();

extern u32 dstt_stopTransmission_address;
extern u16 dstt_readSd_sdsc_shift;

class DSTTReadSdStopTransmissionPatchCode : public PatchCode
{
public:
    explicit DSTTReadSdStopTransmissionPatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(dstt_readsd_stopTransmission), SECTION_SIZE(dstt_readsd_stopTransmission), patchHeap) { }

    const void* GetStopTransmissionFunction() const
    {
        return GetAddressAtTarget((void*)dstt_stopTransmission);
    }
};

class DSTTReadSdPatchCode : public SdReadPatchCode
{
public:
    explicit DSTTReadSdPatchCode(PatchHeap& patchHeap,
        const DSTTReadSdStopTransmissionPatchCode* dsttReadSdStopTransmissionPatchCode)
        : SdReadPatchCode(SECTION_START(dstt_readsd), SECTION_SIZE(dstt_readsd), patchHeap)
        {
            dstt_stopTransmission_address = (u32)dsttReadSdStopTransmissionPatchCode->GetStopTransmissionFunction();
        }

    const SdReadFunc GetSdReadFunction() const override
    {
        return (const SdReadFunc)GetAddressAtTarget((void*)dstt_readSd);
    }
};
