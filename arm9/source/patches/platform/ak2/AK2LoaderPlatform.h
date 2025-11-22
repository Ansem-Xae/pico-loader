#pragma once
#include "common.h"
#include "../acekard-common/IoRPGLoaderPlatform.h"
#include "ak2ReadSdAsm.h"
#include "ak2SdReadSectorAsm.h"
#include "ak2WriteSdAsm.h"

/// @brief Implementation of LoaderPlatform for the Acekard 2 flashcard
class AK2LoaderPlatform : public IoRPGLoaderPlatform
{
private:
    enum
    {
        IORPG_CMD_SDIO_BYTE = 0xD5
    };

public:
    AK2LoaderPlatform() : IoRPGLoaderPlatform(IORPG_CMD_SDIO_BYTE) { }

    const SdReadPatchCode* CreateSdReadPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            auto* waitForStatePatchCode = patchCodeCollection.GetOrAddSharedPatchCode([&]
            {
                return new ioRPGSDWaitForStatePatchCode(patchHeap);
            });
            return new AK2ReadSdPatchCode(patchHeap,
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ioRPGSendSDIOCommandPatchCode(patchHeap);
                }),
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new AK2SDReadSectorPatchCode(patchHeap, waitForStatePatchCode);
                }),
                waitForStatePatchCode);
        });
    }

    const SdWritePatchCode* CreateSdWritePatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new AK2WriteSdPatchCode(patchHeap,
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
        ak2_readSd_sdsc_shift = THUMB_MOVS_REG(THUMB_R4, THUMB_R0);
        ak2_writeSd_sdsc_shift = THUMB_MOVS_REG(THUMB_R7, THUMB_R0);
    }
};
