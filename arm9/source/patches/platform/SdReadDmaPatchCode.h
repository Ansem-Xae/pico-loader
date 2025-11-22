#pragma once
#include "../PatchCode.h"

/// @brief Abstract base class for patch code implementing dma SD reads
class SdReadDmaPatchCode : public PatchCode
{
protected:
    SdReadDmaPatchCode(const void* code, u32 size, PatchHeap& patchHeap)
        : PatchCode(code, size, patchHeap) { }

public:
    /// @brief Pointer to a function for reading a single SD sector using the given dmaChannel.
    ///        In first sector of batch read, previousSrcSector will be 0.
    ///        On further reads previousSrcSector will contains the previous srcSector
    ///        which can be used to continue a sequential read.
    typedef void (*SdReadDmaFunc)(u32 srcSector, u32 previousSrcSector, u32 dmaChannel, void* dst);

    /// @brief Pointer to a function for finishing a batch of dma reads.
    typedef void (*SdReadDmaFinishFunc)(void);

    /// @brief Gets a pointer to the dma SD read function in the patch code.
    /// @return The pointer to the dma SD read function.
    virtual const SdReadDmaFunc GetSdReadDmaFunction() const = 0;

    /// @brief Gets a pointer to the dma SD read finish function in the patch code.
    /// @return The pointer to the dma SD read finish function.
    virtual const SdReadDmaFinishFunc GetSdReadDmaFinishFunction() const = 0;
};
