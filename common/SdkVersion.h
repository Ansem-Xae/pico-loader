#pragma once

#define SDK_VERSION_MAJOR_SHIFT         24
#define SDK_VERSION_MINOR_SHIFT         16
#define SDK_VERSION_RELEASE_STEP_SHIFT   0

#define SDK_VERSION_MAJOR_MASK          0x00FF
#define SDK_VERSION_MINOR_MASK          0x00FF
#define SDK_VERSION_RELEASE_STEP_MASK   0xFFFF

#define SDK_VERSION_MAJOR_NITRO_2       2
#define SDK_VERSION_MAJOR_NITRO_3       3
#define SDK_VERSION_MAJOR_NITRO_4       4
#define SDK_VERSION_MAJOR_TWL           5

#define SDK_VERSION_RELEASE_STEP_PREVIEW            10000
#define SDK_VERSION_RELEASE_STEP_RELEASE_CANDIDATE  20000
#define SDK_VERSION_RELEASE_STEP_RELEASE            30000

/// @brief Class representing an exact SDK version.
class SdkVersion
{
    u32 _version;
public:
    constexpr SdkVersion(u32 version)
        : _version(version) { }

    constexpr SdkVersion(u32 major, u32 minor, u32 releaseStep)
        : _version((major << SDK_VERSION_MAJOR_SHIFT) | (minor << SDK_VERSION_MINOR_SHIFT) | releaseStep) { }

    constexpr u32 GetMajor() const { return _version >> SDK_VERSION_MAJOR_SHIFT; }
    constexpr u32 GetMinor() const { return (_version >> SDK_VERSION_MINOR_SHIFT) & SDK_VERSION_MINOR_MASK; }
    constexpr u32 GetReleaseClass() const { return GetReleaseStep() / 10000; }
    constexpr u32 GetReleaseMajor() const { return (GetReleaseStep() % 10000) / 100; }
    constexpr u32 GetReleaseMinor() const { return (GetReleaseStep() % 10000) % 100; }
    constexpr u32 GetReleaseStep() const { return _version & SDK_VERSION_RELEASE_STEP_MASK; }

    constexpr bool IsTwlSdk() const { return GetMajor() == SDK_VERSION_MAJOR_TWL; }

    constexpr friend auto operator<=>(const SdkVersion&, const SdkVersion&) = default;
};
