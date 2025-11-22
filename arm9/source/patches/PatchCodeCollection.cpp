#include "common.h"
#include "PatchCodeCollection.h"

void PatchCodeCollection::AddUniquePatchCode(PatchCode* patchCode)
{
    for (auto& entry : _patchCodeEntries)
    {
        if (entry.type == 0)
        {
            entry.type = 0xFFFFFFFFu;
            entry.patchCode = patchCode;
            return;
        }
    }

    LOG_FATAL("Out of patch code entries.\n");
    while (true);
}

PatchCodeCollection::PatchCodeEntry& PatchCodeCollection::GetOrAddSharedPatchCode(u32 id)//, create_patch_code_func_t factoryFunction)
{
    PatchCodeEntry* firstFreeEntry = nullptr;
    for (auto& entry : _patchCodeEntries)
    {
        if (entry.type == 0)
        {
            firstFreeEntry = &entry;
            break;
        }
        else if (entry.type == id)
        {
            return entry;
        }
    }

    if (firstFreeEntry == nullptr)
    {
        LOG_FATAL("Out of patch code entries.\n");
        while (true);
    }

    return *firstFreeEntry;
}

void PatchCodeCollection::CopyAllToTarget() const
{
    for (auto& entry : _patchCodeEntries)
    {
        if (entry.type == 0)
        {
            break;
        }

        entry.patchCode->CopyToTarget();
    }
}
