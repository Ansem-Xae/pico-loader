#pragma once

/// @brief Class for displaying critical errors on screen.
class ErrorDisplay
{
public:
    /// @brief Formats and sends a critical error message to the arm9 to display it on screen.
    /// @note This function does not return.
    /// @param errorFormat The error message to format.
    void PrintError(const char* errorFormat, ...);
};
