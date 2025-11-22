#pragma once
#include <memory>
#include "common.h"
#include "DSProtectVersion.h"

#define AP_LIST_OVERLAY_ID_STATIC_ARM9  0xFFFE
#define AP_LIST_OVERLAY_ID_INVALID      0xFFFF

/// @brief Class representing an AP list entry.
class ApListEntry
{
    u32 gameCode;
    u16 gameVersion : 5;
    u16 dsProtectVersion : 5; // see DSProtectVersion
    u16 dsProtectFunctionMask : 6; // for versions < 1.23
    u16 regularOverlayId; // 0xFFFE = static ARM9, 0xFFFF = invalid
    u16 sOverlayId; // 0xFFFE = static ARM9, 0xFFFF = invalid
    u8 regularOffset[3]; // 24 bit
    u8 sOffset[3]; // 24 bit

public:
    u32 GetGameCode() const { return gameCode; }
    u8 GetGameVersion() const { return gameVersion; }
    DSProtectVersion GetDSProtectVersion() const { return static_cast<DSProtectVersion>(dsProtectVersion); }
    u8 GetDSProtectFunctionMask() const { return dsProtectFunctionMask; }
    u16 GetRegularOverlayId() const { return regularOverlayId; }
    u16 GetSOverlayId() const { return sOverlayId; }
    u32 GetRegularOffset() const { return regularOffset[0] | (regularOffset[1] << 8) | (regularOffset[2] << 16); }
    u32 GetSOffset() const { return sOffset[0] | (sOffset[1] << 8) | (sOffset[2] << 16); }

    void Dump() const;
};

static_assert(sizeof(ApListEntry) == 0x10, "Invalid sizeof(ApListEntry)");

/// @brief Class representing an AP list.
class ApList
{
public:
    ApList(std::unique_ptr<const ApListEntry[]> entries, u32 count)
        : _entries(std::move(entries)), _count(count) { }

    const ApListEntry* FindEntry(u32 gameCode, u8 gameVersion);

private:
    std::unique_ptr<const ApListEntry[]> _entries;
    u32 _count;
};
