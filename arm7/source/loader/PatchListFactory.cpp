#include "common.h"
#include "PatchListFactory.h"

std::unique_ptr<PatchList> PatchListFactory::CreateFromFile(const TCHAR* path)
{
    FIL file;
    if (f_open(&file, path, FA_OPEN_EXISTING | FA_READ) != FR_OK)
    {
        LOG_FATAL("Failed to open patch list file\n");
        return nullptr;
    }
    u32 fileSize = f_size(&file);
    auto fileContents = std::make_unique_for_overwrite<u8[]>(fileSize);
    UINT bytesRead = 0;
    FRESULT result = f_read(&file, fileContents.get(), fileSize, &bytesRead);
    if (result != FR_OK || bytesRead != fileSize)
    {
        LOG_FATAL("Failed to read patch list file\n");
        return nullptr;
    }
    f_close(&file);

    return std::make_unique<PatchList>(std::move(fileContents));
}
