#pragma once
#include "ApList.h"

/// @brief Factory for creating \see ApList instances.
class ApListFactory
{
public:
    /// @brief Creates an \see ApList instance from the file at the given \p path.
    /// @param path The ap list file path.
    /// @return A pointer to the constructed \see ApList instance, or \c nullptr if construction failed.
    ApList* CreateFromFile(const TCHAR* path);
};
