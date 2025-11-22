#include "common.h"
#include <bit>
#include "Blowfish.h"

void Blowfish::Encrypt(const void* src, void* dst, u32 length) const
{
    const u32* src32 = (const u32*)src;
    u32* dst32 = (u32*)dst;
    for (u32 i = 0; i < (length / 4); i += 2)
    {
        u64 value = src32[i] | ((u64)src32[i + 1] << 32);
        value = Encrypt(value);
        dst32[i] = value & 0xFFFFFFFFu;
        dst32[i + 1] = value >> 32;
    }
}

u64 Blowfish::Encrypt(u64 value) const
{
    u32 y = value & 0xFFFFFFFFu;
    u32 x = value >> 32;
    for (u32 i = 0; i < 16; i++)
    {
        u32 z = _keyTable.pTable[i] ^ x;
        u32 a = _keyTable.sBoxes[0][(z >> 24) & 0xFF];
        u32 b = _keyTable.sBoxes[1][(z >> 16) & 0xFF];
        u32 c = _keyTable.sBoxes[2][(z >> 8) & 0xFF];
        u32 d = _keyTable.sBoxes[3][z & 0xFF];
        x = (d + (c ^ (b + a))) ^ y;
        y = z;
    }

    return (x ^ _keyTable.pTable[16]) | ((u64)(y ^ _keyTable.pTable[17]) << 32);
}

void Blowfish::Decrypt(const void* src, void* dst, u32 length) const
{
    const u32* src32 = (const u32*)src;
    u32* dst32 = (u32*)dst;
    for (u32 i = 0; i < (length / 4); i += 2)
    {
        u64 value = src32[i] | ((u64)src32[i + 1] << 32);
        value = Decrypt(value);
        dst32[i] = value & 0xFFFFFFFFu;
        dst32[i + 1] = value >> 32;
    }
}

u64 Blowfish::Decrypt(u64 value) const
{
    u32 y = value & 0xFFFFFFFFu;
    u32 x = value >> 32;
    for (u32 i = 17; i >= 2; i--)
    {
        u32 z = _keyTable.pTable[i] ^ x;
        u32 a = _keyTable.sBoxes[0][(z >> 24) & 0xFF];
        u32 b = _keyTable.sBoxes[1][(z >> 16) & 0xFF];
        u32 c = _keyTable.sBoxes[2][(z >> 8) & 0xFF];
        u32 d = _keyTable.sBoxes[3][z & 0xFF];
        x = (d + (c ^ (b + a))) ^ y;
        y = z;
    }

    return (x ^ _keyTable.pTable[1]) | ((u64)(y ^ _keyTable.pTable[0]) << 32);
}

void Blowfish::TransformTable(u32 idCode, int level, int modulo)
{
    u32 keyCode[3] = { idCode, idCode >> 1, idCode << 1 };
    if (level >= 1)
    {
        ApplyKeyCode(&keyCode[0], modulo);
    }
    if (level >= 2)
    {
        ApplyKeyCode(&keyCode[0], modulo);
    }
    keyCode[1] <<= 1;
    keyCode[2] >>= 1;
    if (level >= 3)
    {
        ApplyKeyCode(&keyCode[0], modulo);
    }
}

void Blowfish::ApplyKeyCode(u32* keyCode, int modulo)
{
    Encrypt(&keyCode[1], &keyCode[1], 8);
    Encrypt(&keyCode[0], &keyCode[0], 8);
    const u32 reversedKeyCode[3] =
    {
        std::byteswap(keyCode[0]),
        std::byteswap(keyCode[1]),
        std::byteswap(keyCode[2])
    };
    int keyCodeIndex = 0;
    for (u32 i = 0; i < BLOWFISH_PTABLE_ENTRY_COUNT; i++)
    {
        _keyTable.pTable[i] ^= reversedKeyCode[keyCodeIndex];
        if (++keyCodeIndex == (modulo >> 2))
        {
            keyCodeIndex = 0;
        }
    }

    u64 scratch = 0;
    u64* keyTable = (u64*)&_keyTable;
    for (u32 i = 0; i < (sizeof(KeyTable) / 8); i++)
    {
        scratch = Encrypt(scratch);
        keyTable[i] = (scratch >> 32) | ((scratch & 0xFFFFFFFFu) << 32);
    }
}
