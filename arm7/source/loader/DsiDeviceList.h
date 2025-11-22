#pragma once

#define DSI_DEVICELIST_ENTRY_FLAGS_DRIVE_SDMC       0
#define DSI_DEVICELIST_ENTRY_FLAGS_DRIVE_NAND       1

#define DSI_DEVICELIST_ENTRY_FLAGS_TYPE_PHYSICAL    (0 << 3)
#define DSI_DEVICELIST_ENTRY_FLAGS_TYPE_FILE        (1 << 3)
#define DSI_DEVICELIST_ENTRY_FLAGS_TYPE_FOLDER      (2 << 3)

#define DSI_DEVICELIST_ENTRY_FLAGS_PARTITION_FIRST  (0 << 5)
#define DSI_DEVICELIST_ENTRY_FLAGS_PARTITION_SECOND (1 << 5)

#define DSI_DEVICELIST_ENTRY_FLAGS_ENCRYPTED        (1 << 7)

#define DSI_DEVICELIST_ENTRY_ACCESS_RIGHTS_READ     (1 << 1)
#define DSI_DEVICELIST_ENTRY_ACCESS_RIGHTS_WRITE    (1 << 2)

/// @brief Struct representing a DSi device list entry.
struct dsi_devicelist_entry_t
{
    char driveLetter;
    u8 flags;
    u8 accessRights;
    u8 padding;
    char deviceName[16];
    char path[64];
};

static_assert(sizeof(dsi_devicelist_entry_t) == 0x54, "Invalid dsi_devicelist_entry_t size.");

/// @brief Struct representing a DSi device list.
struct dsi_devicelist_t
{
    dsi_devicelist_entry_t deviceList[11];
    u8 padding[0x24];
    char appFileName[64];
};

static_assert(sizeof(dsi_devicelist_t) == 0x400, "Invalid dsi_devicelist_t size.");
