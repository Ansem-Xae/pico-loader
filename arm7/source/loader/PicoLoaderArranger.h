#pragma once
#include "LoaderInfo.h"

/// @brief Class for setting up \see loader_info_t.
class PicoLoaderArranger
{
public:
    /// @brief Sets up the given \p info for being able to reload Pico Loader at a later time.
    /// @param info The \see loader_info_t struct to fill.
    /// @return \c true when setting up was successful, or \c false otherwise.
    bool SetupPicoLoaderInfo(loader_info_t* info) const;
};
