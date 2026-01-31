#include "common.h"
#include "ApListFactory.h"

std::unique_ptr<ApList> ApListFactory::CreateFromFile(const TCHAR* path)
{
    FIL file;
    if (f_open(&file, path, FA_OPEN_EXISTING | FA_READ) != FR_OK)
    {
        LOG_FATAL("Failed to open ap list file\n");
        return nullptr;
    }
    const u32 entryCount = f_size(&file) / sizeof(ApListEntry);
    auto entries = std::make_unique_for_overwrite<ApListEntry[]>(entryCount);
    UINT bytesRead = 0;
    FRESULT result = f_read(&file, entries.get(), entryCount * sizeof(ApListEntry), &bytesRead);
    if (result != FR_OK || bytesRead != entryCount * sizeof(ApListEntry))
    {
        LOG_FATAL("Failed to read ap list file\n");
        return nullptr;
    }
    f_close(&file);

    return std::make_unique<ApList>(std::move(entries), entryCount);
}