#pragma once
#include "SdkVersion.h"

/// @brief Class representing a function signature with a minimum and maximum SDK version.
class FunctionSignature
{
public:
    consteval FunctionSignature(const u32 pattern[4], SdkVersion minSdkVersion, SdkVersion maxSdkVersion)
        : _pattern { pattern[0], pattern[1], pattern[2], pattern[3] }
        , _minSdkVersion(minSdkVersion), _maxSdkVersion(maxSdkVersion) { }

    /// @brief Returns the pattern of the function signature.
    /// @return The pattern of the function signature.
    constexpr const u32* GetPattern() const { return _pattern; }

    /// @brief Returns the minimum SDK version of this function signature.
    /// @return The minimum SDK version of this function signature.
    constexpr SdkVersion GetMinimumSdkVersion() const { return _minSdkVersion; }

    /// @brief Returns the maximum SDK version of this function signature.
    /// @return The maximum SDK version of this function signature.
    constexpr SdkVersion GetMaximumSdkVersion() const { return _maxSdkVersion; }

private:
    const u32 _pattern[4];
    const SdkVersion _minSdkVersion;
    const SdkVersion _maxSdkVersion;
};
