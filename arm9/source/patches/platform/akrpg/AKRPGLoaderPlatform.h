#pragma once
#include "common.h"
#include "../acekard-common/IoRPGLoaderPlatform.h"
#include "akrpgReadSdAsm.h"
#include "akrpgSdReadSectorAsm.h"
#include "akrpgWriteSdAsm.h"

/// @brief Implementation of LoaderPlatform for the Acekard RPG SD card.
class AKRPGLoaderPlatform : public IoRPGLoaderPlatform
{
private:
    enum
    {
        IORPG_CMD_SDIO_BYTE = 0xD5
    };

public:
    AKRPGLoaderPlatform() : IoRPGLoaderPlatform(IORPG_CMD_SDIO_BYTE) { }

    const SdReadPatchCode* CreateSdReadPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            auto* waitForStatePatchCode = patchCodeCollection.GetOrAddSharedPatchCode([&]
            {
                return new ioRPGSDWaitForStatePatchCode(patchHeap);
            });
            return new AKRPGReadSdPatchCode(patchHeap,
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ioRPGSendSDIOCommandPatchCode(patchHeap);
                }),
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new AKRPGSDReadSectorPatchCode(patchHeap, waitForStatePatchCode);
                }),
                waitForStatePatchCode);
        });
    }

    const SdWritePatchCode* CreateSdWritePatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new AKRPGWriteSdPatchCode(patchHeap,
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ioRPGSendSDIOCommandPatchCode(patchHeap);
                }),
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ioRPGSDWaitForStatePatchCode(patchHeap);
                }));
        });
    }

    void PatchSdscShift(void) const override
    {
        akrpg_readSd_sdsc_shift = THUMB_MOVS_REG(THUMB_R4, THUMB_R0);
        akrpg_writeSd_sdsc_shift = THUMB_MOVS_REG(THUMB_R7, THUMB_R0);
    }
};
