#pragma once
#include "PatchList.h"

/// @brief Factory for creating \see ApList instances.
class PatchListFactory
{
public:
    /// @brief Creates an \see PatchList instance from the file at the given \p path.
    /// @param path The patch list file path.
    /// @return A pointer to the constructed \see PatchList instance, or \c nullptr if construction failed.
    std::unique_ptr<PatchList> CreateFromFile(const TCHAR* path);
};
