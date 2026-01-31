#include "common.h"
#include <algorithm>
#include "PatchList.h"

const PatchListEntry* PatchList::FindEntry(u32 gameCode, u8 gameVersion)
{
    auto header = reinterpret_cast<const PatchListHeader*>(_fileContents.get());
    u32 count = header->entryCount;
    auto entries = header->headerEntries;
    if (count != 0)
    {
        const auto gameEntry = std::lower_bound(entries, entries + count, gameCode,
            [gameVersion] (const PatchListHeaderEntry& entry, u32 value)
            {
                if (entry.gameCode == value)
                {
                    return entry.gameVersion < gameVersion;
                }
                else
                {
                    return entry.gameCode < value;
                }
            });

        if (gameEntry != entries + count && gameEntry->gameCode == gameCode && gameEntry->gameVersion == gameVersion)
        {
            return reinterpret_cast<const PatchListEntry*>(_fileContents.get() + gameEntry->offset);
        }
    }

    return nullptr;
}
