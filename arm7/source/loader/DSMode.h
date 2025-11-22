#pragma once

/// @brief Class handling switching from DSi to DS mode.
class DSMode
{
public:
    /// @brief Switches to DS mode for the game with the given \p gameCode.
    /// @param gameCode The game code of the game to switch to DS mode for.
    void SwitchToDSMode(u32 gameCode) const;

    /// @brief Switches CODEC to DS mode for the game with the given \p gameCode.
    /// @param gameCode The game code of the game to switch CODEC to DS mode for.
    void SwitchToDSTouchAndSoundMode(u32 gameCode) const;

private:
    void SwitchCodecToDSMode(u32 gameCode) const;
    bool ShouldUseVolumeFix(u32 gameCode) const;
};
