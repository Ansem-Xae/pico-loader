#pragma once

/// @brief Class for displaying a critical error message.
class ErrorDisplay
{
public:
    /// @brief Displays the given \p errorString and loops.
    /// @note This function does not return.
    /// @param errorString The error string to display.
    void PrintError(const char* errorString);
};
