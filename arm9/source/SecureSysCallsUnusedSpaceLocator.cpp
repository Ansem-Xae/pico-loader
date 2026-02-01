#include "common.h"
#include "gameCode.h"
#include <algorithm>
#include "patches/PatchHeap.h"
#include "thumbInstructions.h"
#include "SecureSysCallsUnusedSpaceLocator.h"

#define THUMB_MOVS_R0_R1    THUMB_MOVS_REG(0, 1)
#define THUMB_MOVS_R2_0     THUMB_MOVS_IMM(2, 0)

static const u16 sSvcSoftResetPattern[] = { THUMB_SVC(0), THUMB_BX_LR };
static const u16 sSvcWaitByLoopPattern[] = { THUMB_SVC(3), THUMB_BX_LR };
static const u16 sSvcWaitIntrPattern[] = { THUMB_MOVS_R2_0, THUMB_SVC(4), THUMB_BX_LR };
static const u16 sSvcWaitVBlankIntrPattern[] = { THUMB_MOVS_R2_0, THUMB_SVC(5), THUMB_BX_LR };
static const u16 sSvcHaltPattern[] = { THUMB_SVC(6), THUMB_BX_LR };
static const u16 sSvcDivPattern[] = { THUMB_SVC(9), THUMB_BX_LR };
static const u16 sSvcDivRemPattern[] = { THUMB_SVC(9), THUMB_MOVS_R0_R1, THUMB_BX_LR };
static const u16 sSvcCpuSetPattern[] = { THUMB_SVC(0xB), THUMB_BX_LR };
static const u16 sSvcCpuSetFastPattern[] = { THUMB_SVC(0xC), THUMB_BX_LR };
static const u16 sSvcSqrtPattern[] = { THUMB_SVC(0xD), THUMB_BX_LR };
static const u16 sSvcGetCrc16Pattern[] = { THUMB_SVC(0xE), THUMB_BX_LR };
static const u16 sSvcIsMainMemExpandedPattern[] = { THUMB_SVC(0xF), THUMB_BX_LR };
static const u16 sSvcUnpackBitsPattern[] = { THUMB_SVC(0x10), THUMB_BX_LR };
static const u16 sSvcUncompressLz8Pattern[] = { THUMB_SVC(0x11), THUMB_BX_LR };
static const u16 sSvcUncompressLz16FromDevicePattern[] = { THUMB_SVC(0x12), THUMB_BX_LR };
static const u16 sSvcUncompressHuffmanFromDevicePattern[] = { THUMB_SVC(0x13), THUMB_BX_LR };
static const u16 sSvcUncompressRl8Pattern[] = { THUMB_SVC(0x14), THUMB_BX_LR };
static const u16 sSvcUncompressRl16FromDevicePattern[] = { THUMB_SVC(0x15), THUMB_BX_LR };

struct svc_pattern_t
{
    const u16* pattern;
    u32 length;
};

static const std::array<const svc_pattern_t, 18> sSvcPatterns
{
    svc_pattern_t { sSvcSoftResetPattern, sizeof(sSvcSoftResetPattern) },
    svc_pattern_t { sSvcWaitByLoopPattern, sizeof(sSvcWaitByLoopPattern) },
    svc_pattern_t { sSvcWaitIntrPattern, sizeof(sSvcWaitIntrPattern) },
    svc_pattern_t { sSvcWaitVBlankIntrPattern, sizeof(sSvcWaitVBlankIntrPattern) },
    svc_pattern_t { sSvcHaltPattern, sizeof(sSvcHaltPattern) },
    svc_pattern_t { sSvcDivPattern, sizeof(sSvcDivPattern) },
    svc_pattern_t { sSvcDivRemPattern, sizeof(sSvcDivRemPattern) },
    svc_pattern_t { sSvcCpuSetPattern, sizeof(sSvcCpuSetPattern) },
    svc_pattern_t { sSvcCpuSetFastPattern, sizeof(sSvcCpuSetFastPattern) },
    svc_pattern_t { sSvcSqrtPattern, sizeof(sSvcSqrtPattern) },
    svc_pattern_t { sSvcGetCrc16Pattern, sizeof(sSvcGetCrc16Pattern) },
    svc_pattern_t { sSvcIsMainMemExpandedPattern, sizeof(sSvcIsMainMemExpandedPattern) },
    svc_pattern_t { sSvcUnpackBitsPattern, sizeof(sSvcUnpackBitsPattern) },
    svc_pattern_t { sSvcUncompressLz8Pattern, sizeof(sSvcUncompressLz8Pattern) },
    svc_pattern_t { sSvcUncompressLz16FromDevicePattern, sizeof(sSvcUncompressLz16FromDevicePattern) },
    svc_pattern_t { sSvcUncompressHuffmanFromDevicePattern, sizeof(sSvcUncompressHuffmanFromDevicePattern) },
    svc_pattern_t { sSvcUncompressRl8Pattern, sizeof(sSvcUncompressRl8Pattern) },
    svc_pattern_t { sSvcUncompressRl16FromDevicePattern, sizeof(sSvcUncompressRl16FromDevicePattern) }
};

const u16* SecureSysCallsUnusedSpaceLocator::FindPattern(const u16* data, u32 length, const u16* pattern, u32 patternLength) const
{
    length >>= 1;
    patternLength >>= 1;
    for (u32 i = 0; i < length - patternLength; i++)
    {
        bool ok = true;
        for (u32 j = 0; j < patternLength; j++)
        {
            if (data[i + j] != pattern[j])
            {
                ok = false;
                break;
            }
        }
        if (ok)
            return &data[i];
    }
    return nullptr;
}

void SecureSysCallsUnusedSpaceLocator::FindUnusedSpace(const nds_header_ntr_t* romHeader, PatchHeap& patchHeap) const
{
    // NOTE: We can not check if arm9RomOffset == 0x4000, because some rom hacks repack the rom in unusual ways.

    u32 secureStart = romHeader->arm9LoadAddress;

    if (*(u32*)(secureStart + 0x800) == 0x4770DF00)
    {
        // secure area for development purposes has this area empty
        patchHeap.AddFreeSpace((void*)secureStart, 0x800);
        LOG_DEBUG("Added free space starting at 0x%x with size 0x%x\n", secureStart, 0x800);
        return;
    }

    if (((u32*)secureStart)[0] != 0xE7FFDEFF || ((u32*)secureStart)[1] != 0xE7FFDEFF)
    {
        LOG_ERROR("No secure area space found\n");
        return;
    }

    std::array<svc_pattern_t, sSvcPatterns.size()> patternLocations;

    for (u32 i = 0; i < sSvcPatterns.size(); i++)
    {
        patternLocations[i].pattern = FindPattern((const u16*)secureStart, 0x800, sSvcPatterns[i].pattern, sSvcPatterns[i].length);
        patternLocations[i].length = sSvcPatterns[i].length;
    }

    std::qsort(patternLocations.begin(), patternLocations.size(), sizeof(svc_pattern_t), [](const void* a, const void* b)
    {
        const auto cmp = static_cast<const svc_pattern_t*>(a)->pattern <=> static_cast<const svc_pattern_t*>(b)->pattern;
        if (cmp < 0)
        {
            return -1;
        }
        if (cmp > 0)
        {
            return 1;
        }
        return 0;
    });

    u32 current = secureStart;
    for (u32 i = 0; i < sSvcPatterns.size(); i++)
    {
        if (!patternLocations[i].pattern)
            continue;
        u32 freeSpaceEnd = (u32)patternLocations[i].pattern & ~3;
        if (current < freeSpaceEnd)
        {
            u32 freeSpace = freeSpaceEnd - current;
            patchHeap.AddFreeSpace((void*)current, freeSpace);
            LOG_DEBUG("Added free space starting at 0x%x with size 0x%x\n", current, freeSpace);
        }
        current = ((u32)patternLocations[i].pattern + patternLocations[i].length + 3) & ~3;
    }
    if (current < secureStart + 0x800)
    {
        u32 freeSpace = secureStart + 0x800 - current;
        patchHeap.AddFreeSpace((void*)current, freeSpace);
        LOG_DEBUG("Added free space starting at 0x%x with size 0x%x\n", current, freeSpace);
    }
}
