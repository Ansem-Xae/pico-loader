#pragma once

/// @brief Class for setting up the save file for retail card roms.
class CardSaveArranger
{
public:
    /// @brief Sets up the save file at \p savePath for a retail card rom with the given \p gameCode.
    /// @param gameCode The game code of the retail card rom.
    /// @param savePath The desired save file path.
    /// @return \c true when setting up the save was successful, or \c false otherwise.
    bool SetupCardSave(u32 gameCode, const TCHAR* savePath) const;
};
