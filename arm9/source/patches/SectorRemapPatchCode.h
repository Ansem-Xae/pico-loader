#pragma once
#include "PatchCode.h"

class SectorRemapPatchCode : public PatchCode
{
public:
    SectorRemapPatchCode(const void* code, u32 size, PatchHeap& patchHeap)
        : PatchCode(code, size, patchHeap) { }

    virtual const void* GetRemapFunction() const = 0;
};
