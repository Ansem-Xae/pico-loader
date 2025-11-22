#pragma once

/// @brief Class for clearing the arm9 IO registers.
class Arm9IoRegisterClearer
{
public:
    /// @brief Clears the arm9 ntr IO registers.
    ///        When \p isSdkResetSystem is \c true, some IO registers are not cleared to avoid graphical glitches.
    /// @param isSdkResetSystem \c true if the clear is performed context of OS_ResetSystem, or \c false otherwise.
    void ClearNtrIoRegisters(bool isSdkResetSystem) const;

    /// @brief Clears the arm9 twl IO registers.
    void ClearTwlIoRegisters() const;

private:
    void ClearGraphicsRegisters(bool isSdkResetSystem) const;
    void ClearTimerRegisters() const;
    void ClearNtrDmaRegisters() const;
};
