#pragma once

class LoaderPlatform;

/// @brief Class for patching the arm7 of retail roms.
class Arm7Patcher
{
public:
    /// @brief Applies arm7 patches using the given \p loaderPlatform.
    /// @param loaderPlatform The loader platform to use.
    /// @return A pointer to the patch space in IWRAM, or \c nullptr if the patches have been placed in main memory.
    void* ApplyPatches(const LoaderPlatform* loaderPlatform) const;
};
