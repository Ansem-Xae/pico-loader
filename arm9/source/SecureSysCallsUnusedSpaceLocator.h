#pragma once
#include "ndsHeader.h"

class PatchHeap;

/// @brief Class for finding the unused space between the system calls in the arm9 secure area.
class SecureSysCallsUnusedSpaceLocator
{
public:
    /// @brief Searches for the unused space between the system calls in the arm9 secure area of the rom
    ///        with the given \p romHeader and adds the unused space to the given \p patchHeap.
    /// @param romHeader The rom header.
    /// @param patchHeap The patch heap to add the unused space to.
    void FindUnusedSpace(const nds_header_ntr_t* romHeader, PatchHeap& patchHeap) const;

private:
    const u16* FindPattern(const u16* data, u32 length, const u16* pattern, u32 patternLength) const;
};
