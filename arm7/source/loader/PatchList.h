#pragma once
#include <memory>

enum class PatchListPatchType : u8
{
    Replace,
    Metafortress
};

struct PatchListHeaderEntry
{
    u32 gameCode;
    u32 gameVersion : 8;
    u32 offset : 24;
};

struct PatchListHeader
{
    u32 entryCount;
    PatchListHeaderEntry headerEntries[1];
};

union PatchListEntryPatch
{
    struct
    {
        PatchListPatchType patchType;
        u8 reserved;
    } common;
    struct
    {
        PatchListPatchType patchType;
        u8 reserved;
        u16 dataLength;
        u32 address;
        u8 data[1];
    } replacePatch;
    struct
    {
        PatchListPatchType patchType;
        u8 reserved;
        u16 addressCount;
        u32 addresses[1];
    } metafortressPatch;
};

static_assert(offsetof(PatchListEntryPatch, replacePatch.dataLength) == 2);
static_assert(offsetof(PatchListEntryPatch, replacePatch.address) == 4);
static_assert(offsetof(PatchListEntryPatch, replacePatch.data) == 8);
static_assert(offsetof(PatchListEntryPatch, metafortressPatch.addressCount) == 2);
static_assert(offsetof(PatchListEntryPatch, metafortressPatch.addresses) == 4);

struct PatchListEntry
{
    u16 length;
    u16 patchCount;
    PatchListEntryPatch firstPatch;
};

/// @brief Class representing a patch list.
class PatchList
{
public:
    PatchList(std::unique_ptr<const u8[]> fileContents)
        : _fileContents(std::move(fileContents)) { }

    const PatchListEntry* FindEntry(u32 gameCode, u8 gameVersion);

private:
    std::unique_ptr<const u8[]> _fileContents;
};
