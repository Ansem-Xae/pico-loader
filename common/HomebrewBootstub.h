#pragma once

#define HOMEBREW_BOOTSTUB_BOOTSIG   0x62757473746F6F62ULL

struct homebrew_bootstub_t
{
    u64 bootSig;
    void* arm9Reboot;
    void* arm7Reboot;
    u32 bootSize;
};

#define HOMEBREW_BOOTSTUB   ((homebrew_bootstub_t*)0x02FF4000)
