#pragma once
#include "common.h"
#include "../acekard-common/IoRPGLoaderPlatform.h"
#include "r4idsnReadSdAsm.h"
#include "r4idsnSdReadSectorAsm.h"
#include "r4idsnWriteSdAsm.h"

/// @brief Implementation of LoaderPlatform for the r4idsn.com flashcard.
class R4iDSNLoaderPlatform : public IoRPGLoaderPlatform
{
private:
    enum
    {
        IORPG_CMD_SDIO_BYTE = 0xAB
    };

public:
    R4iDSNLoaderPlatform() : IoRPGLoaderPlatform(IORPG_CMD_SDIO_BYTE) { }

    const SdReadPatchCode* CreateSdReadPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            auto* waitForStatePatchCode = patchCodeCollection.GetOrAddSharedPatchCode([&]
            {
                return new ioRPGSDWaitForStatePatchCode(patchHeap);
            });
            return new R4iDSNReadSdPatchCode(patchHeap,
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ioRPGSendSDIOCommandPatchCode(patchHeap);
                }),
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new R4iDSNSDReadSectorPatchCode(patchHeap, waitForStatePatchCode);
                }),
                waitForStatePatchCode);
        });
    }

    const SdWritePatchCode* CreateSdWritePatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new R4iDSNWriteSdPatchCode(patchHeap,
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
        r4idsn_readSd_sdsc_shift = THUMB_MOVS_REG(THUMB_R4, THUMB_R0);
        r4idsn_writeSd_sdsc_shift = THUMB_MOVS_REG(THUMB_R7, THUMB_R0);
    }
};
