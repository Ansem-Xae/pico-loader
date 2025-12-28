#pragma once
#include "ndsHeader.h"

/// @brief Class for setting up the save file for retail card roms.
class CardSaveArranger
{
public:
    /// @brief Sets up the save file at \p savePath for a retail card rom with the given \p gameCode.
    /// @param header The header of the retail card rom.
    /// @param savePath The desired save file path.
    /// @return \c true when setting up the save was successful, or \c false otherwise.
    bool SetupCardSave(const nds_header_ntr_t* header, const TCHAR* savePath) const;
};
