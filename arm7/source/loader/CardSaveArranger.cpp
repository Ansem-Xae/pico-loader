#include "common.h"
#include <string.h>
#include "SaveList.h"
#include "SaveListFactory.h"
#include "fileInfo.h"
#include "CardSaveArranger.h"

#define SAVE_LIST_PATH      "/_pico/savelist.bin"
#define DEFAULT_SAVE_SIZE   (512 * 1024)
#define SAVE_FILL_VALUE     0xFF

bool CardSaveArranger::SetupCardSave(u32 gameCode, const TCHAR* savePath) const
{
    SaveList* saveList = SaveListFactory().CreateFromFile(SAVE_LIST_PATH);
    u32 saveSize = DEFAULT_SAVE_SIZE;
    if (saveList)
    {
        const auto saveListEntry = saveList->FindEntry(gameCode);
        if (!saveListEntry)
        {
            LOG_WARNING("Game code %c%c%c%c not found in save list\n",
                gameCode & 0xFF, (gameCode >> 8) & 0xFF, (gameCode >> 16) & 0xFF, gameCode >> 24);
        }
        else
        {
            saveSize = saveListEntry->GetSaveSize();
            saveListEntry->Dump();
        }
        delete saveList;
    }
    if (saveSize == 0)
    {
        return true;
    }

    auto file = std::make_unique<FIL>();
    LOG_DEBUG("Save file: %s\n", savePath);
    if (f_open(file.get(), savePath, FA_OPEN_ALWAYS | FA_READ | FA_WRITE) != FR_OK)
    {
        LOG_FATAL("Failed to open or create save file\n");
        return false;
    }
    u32 initialSize = f_size(file.get());
    if (initialSize < saveSize)
    {
        if (f_lseek(file.get(), saveSize) != FR_OK ||
            f_lseek(file.get(), initialSize) != FR_OK)
        {
            LOG_FATAL("Failed to expand save file\n");
            return false;
        }

        auto ffBuffer = std::make_unique<u8[]>(512);
        memset(ffBuffer.get(), SAVE_FILL_VALUE, 512);

        u32 offset = initialSize;
        // Align to 512 bytes
        if ((offset & 511) != 0)
        {
            u32 remainingTo512 = 512 - (offset & 511);
            UINT bytesWritten = 0;
            if (f_write(file.get(), ffBuffer.get(), remainingTo512, &bytesWritten) != FR_OK ||
                bytesWritten != remainingTo512)
            {
                LOG_FATAL("Failed to expand save file\n");
                return false;
            }
            offset += remainingTo512;
        }

        // Write in 512-byte blocks
        while (offset < saveSize)
        {
            UINT bytesWritten = 0;
            if (f_write(file.get(), ffBuffer.get(), 512, &bytesWritten) != FR_OK ||
                bytesWritten != 512)
            {
                LOG_FATAL("Failed to expand save file\n");
                return false;
            }
            offset += 512;
        }
    }

    DWORD* clusterTab = (DWORD*)SHARED_SAVE_FILE_INFO->clusterMap;
    clusterTab[0] = sizeof(SHARED_SAVE_FILE_INFO->clusterMap) / sizeof(u32);
    file->cltbl = clusterTab;
    FRESULT seekResult = f_lseek(file.get(), CREATE_LINKMAP);
    if (seekResult != FR_OK)
    {
        LOG_FATAL("Failed to make save cluster table. Result: %d\n", seekResult);
        return false;
    }
    SHARED_SAVE_FILE_INFO->clusterShift = __builtin_ctz(file->obj.fs->csize);
    SHARED_SAVE_FILE_INFO->database = file->obj.fs->database;
    SHARED_SAVE_FILE_INFO->clusterMask = file->obj.fs->csize - 1;

    LOG_DEBUG("Made save cluster table\n");

    if (f_close(file.get()) != FR_OK)
    {
        LOG_FATAL("Failed to close save file\n");
        return false;
    }

    return true;
}
