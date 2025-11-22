#pragma once
#include "../PatchCode.h"

/// @brief Abstract base class for patch code implementing SD writes
class SdWritePatchCode : public PatchCode
{
protected:
    SdWritePatchCode(const void* code, u32 size, PatchHeap& patchHeap)
        : PatchCode(code, size, patchHeap) { }

public:
    /// @brief Pointer to a function for writing sectorCount SD sectors
    ///        from the given src buffer to dstSector.
    typedef void (*SdWriteFunc)(u32 dstSector, const void* src, u32 sectorCount);

    /// @brief Gets a pointer to the SD write function in the patch code.
    /// @return The pointer to the SD write function.
    virtual const SdWriteFunc GetSdWriteFunction() const = 0;
};
