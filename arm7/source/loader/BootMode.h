#pragma once

/// @brief The Pico Loader boot mode.
enum class BootMode
{
    /// @brief Boot a retail or homebrew rom.
    Normal,

    /// @brief Reboot a retail rom that used OS_ResetSystem.
    SdkResetSystem,

    /// @brief Boot a multiboot rom that is already loaded into memory.
    Multiboot
};
