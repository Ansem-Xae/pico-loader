#pragma once
#include <string.h>

#define BLOWFISH_PTABLE_ENTRY_COUNT     18
#define BLOWFISH_SBOX_COUNT             4
#define BLOWFISH_SBOX_ENTRY_COUNT       256

/// @brief Class for blowfish encryption and decryption.
class Blowfish
{
public:
    /// @brief Struct representing a blowfish key table.
    struct KeyTable
    {
        u32 pTable[BLOWFISH_PTABLE_ENTRY_COUNT];
        u32 sBoxes[BLOWFISH_SBOX_COUNT][BLOWFISH_SBOX_ENTRY_COUNT];
    };

    static_assert(sizeof(KeyTable) == 0x1048, "Invalid size of Blowfish::KeyTable.");

    /// @brief Constructs an instance of \see Blowfish using the given \p keyTable.
    /// @param keyTable The key table to use. A copy will be made into the constructed class.
    explicit Blowfish(const KeyTable* keyTable)
        : Blowfish(keyTable->pTable, keyTable->sBoxes) { }

    /// @brief Constructs an instance of \see Blowfish using the given \p pTable and \p sBoxes.
    /// @param pTable The p table to use. A copy will be made into the constructed class.
    /// @param sBoxes The s boxes to use. A copy will be made into the constructed class.
    Blowfish(const void* pTable, const void* sBoxes)
    {
        memcpy(_keyTable.pTable, pTable, sizeof(_keyTable.pTable));
        memcpy(_keyTable.sBoxes, sBoxes, sizeof(_keyTable.sBoxes));
    }

    /// @brief Encrypts the given \p length from \p src to \p dst.
    /// @param src The source buffer.
    /// @param dst The encrypted destination buffer.
    /// @param length The length of the data to encrypt. Must be a multiple of 8.
    void Encrypt(const void* src, void* dst, u32 length) const;

    /// @brief Encrypts the given 64-bit \p value and returns the result.
    /// @param value The 64-bit value to encrypt.
    /// @return The encrypted result.
    u64 Encrypt(u64 value) const;

    /// @brief Drcrypts the given \p length from \p src to \p dst.
    /// @param src The encrypted source buffer.
    /// @param dst The decrypted destination buffer.
    /// @param length The length of the data to encrypt. Must be a multiple of 8.
    void Decrypt(const void* src, void* dst, u32 length) const;

    /// @brief Decrypts the given 64-bit \p value and returns the result.
    /// @param value The 64-bit value to decrypt.
    /// @return The decrypted result.
    u64 Decrypt(u64 value) const;

    /// @brief Transforms the key table by the given \p idCode, \p level and \p modulo.
    /// @param idCode The id code to use.
    /// @param level The transform level to use.
    /// @param modulo The modulo to use.
    void TransformTable(u32 idCode, int level, int modulo);

private:
    void ApplyKeyCode(u32* keyCode, int modulo);

    KeyTable _keyTable;
};
