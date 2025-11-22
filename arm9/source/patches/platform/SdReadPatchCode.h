#pragma once
#include "../PatchCode.h"

/// @brief Abstract base class for patch code implementing SD reads
class SdReadPatchCode : public PatchCode
{
protected:
    SdReadPatchCode(const void* code, u32 size, PatchHeap& patchHeap)
        : PatchCode(code, size, patchHeap) { }

public:
    /// @brief Pointer to a function for reading sectorCount SD sectors
    ///        from srcSector to the given dst buffer.
    typedef void (*SdReadFunc)(u32 srcSector, void* dst, u32 sectorCount);

    /// @brief Gets a pointer to the SD read function in the patch code.
    /// @return The pointer to the SD read function.
    virtual const SdReadFunc GetSdReadFunction() const = 0;
};
