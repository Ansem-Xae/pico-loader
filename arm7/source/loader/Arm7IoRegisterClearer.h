#pragma once

/// @brief Class for clearing the arm9 IO registers.
class Arm7IoRegisterClearer
{
public:
    /// @brief Clears the arm7 IO registers.
    void ClearIoRegisters() const;

private:
    void ClearNtrIoRegisters() const;
    void ClearTwlIoRegisters() const;
};
