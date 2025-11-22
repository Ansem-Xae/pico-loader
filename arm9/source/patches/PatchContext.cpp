#include "common.h"
#include "fastSearch.h"
#include "platform/LoaderPlatform.h"
#include "PatchContext.h"

u32* PatchContext::FindPattern32(const u32* pattern, u32 byteLength) const
{
#ifdef LIBTWL_ARM9
    if (byteLength == 16)
    {
        return (u32*)fastSearch16((const u32*)_data, _dataSize, pattern);
    }
#endif
    return nullptr;
}

u32* PatchContext::FindPattern32Twl(const u32* pattern, u32 byteLength) const
{
    if (!_twlData || _twlDataSize == 0)
    {
        return nullptr;
    }

#ifdef LIBTWL_ARM9
    if (byteLength == 16)
    {
        return (u32*)fastSearch16((const u32*)_twlData, _twlDataSize, pattern);
    }
#endif
    return nullptr;
}