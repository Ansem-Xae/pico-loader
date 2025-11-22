#pragma once

struct loader_info_t
{
    u16 clusterShift;
    u16 picoLoaderBootDrive;
    u32 database;
    u32 clusterMap9[10];
    u32 clusterMap7[16];
};
