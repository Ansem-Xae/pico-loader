#include "common.h"
#include "../PatchContext.h"
#include "sharedMemory.h"
#include "ndsHeader.h"
#include "DisableArm7WramClearPatch.h"

bool DisableArm7WramClearPatch::FindPatchTarget(PatchContext& patchContext)
{
    auto romHeader = (const nds_header_ntr_t*)TWL_SHARED_MEMORY->ntrSharedMem.romHeader;
    _clearEndPool = nullptr;
    for (u32 i = 0; i < 0x100; i += 4)
    {
        u32* ptr = (u32*)(romHeader->arm7LoadAddress + i);
        if (*ptr == 0x380FF00)
        {
            _clearEndPool = ptr;
            break;
        }
    }

    if (_clearEndPool)
    {
        LOG_DEBUG("Arm7 wram clear end pool value found at 0x%p\n", _clearEndPool);
    }
    else
    {
        LOG_WARNING("Arm7 wram clear end pool value not found\n");
    }

    return _clearEndPool != nullptr;
}

void DisableArm7WramClearPatch::ApplyPatch(PatchContext& patchContext)
{
    if (!_clearEndPool)
        return;

    *_clearEndPool = 0;
}
