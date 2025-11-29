#include "common.h"
#include "../../../PatchContext.h"
#include "thumbInstructions.h"
#include "gameCode.h"
#include "DSProtectOverlayPatchv1Asm.h"
#include "DSProtectOverlayPatchv2Asm.h"
#include "DSProtectOverlayPatchv2sAsm.h"
#include "DSProtectOverlayPatch.h"

const void* DSProtectOverlayPatch::InsertPatch(PatchContext& patchContext)
{
    if (_version <= DSProtectVersion::v1_28)
    {
        return InsertPatchV1(patchContext);
    }
    else if (_version <= DSProtectVersion::v2_05)
    {
        return InsertPatchV2(patchContext);
    }
    else if (_version <= DSProtectVersion::v2_05s)
    {
        return InsertPatchV2s(patchContext);
    }
    else
    {
        LOG_WARNING("Unsupported DSProtect version\n");
        return next ? (const void*)next->InsertPatch(patchContext) : nullptr;
    }
}

void DSProtectOverlayPatch::SetOldV1Offsets(const u16* offsets) const
{
    int offsetIdx = 0;

    if (_functionMask & 0b000001)
        dsprotectpatchv1_stub_protectb1_offset = offsets[offsetIdx++];
    else
        dsprotectpatchv1_stub_protectb1_offset = 0; // does not exist

    if (_functionMask & 0b000010)
        dsprotectpatchv1_stub_notprotectb1_offset = offsets[offsetIdx++];
    else
        dsprotectpatchv1_stub_notprotectb1_offset = 0; // does not exist

    if (_functionMask & 0b000100)
        dsprotectpatchv1_stub_protectb2_offset = offsets[offsetIdx++];
    else
        dsprotectpatchv1_stub_protectb2_offset = 0; // does not exist

    if (_functionMask & 0b001000)
        dsprotectpatchv1_stub_notprotectb2_offset = offsets[offsetIdx++];
    else
        dsprotectpatchv1_stub_notprotectb2_offset = 0; // does not exist

    if (_functionMask & 0b010000)
        dsprotectpatchv1_stub_protectb3_offset = offsets[offsetIdx++];
    else
        dsprotectpatchv1_stub_protectb3_offset = 0; // does not exist

    if (_functionMask & 0b100000)
        dsprotectpatchv1_stub_notprotectb3_offset = offsets[offsetIdx++];
    else
        dsprotectpatchv1_stub_notprotectb3_offset = 0; // does not exist
}

const void* DSProtectOverlayPatch::InsertPatchV1(PatchContext& patchContext) const
{
    const void* nextPatch = next ? (const void*)next->InsertPatch(patchContext) : nullptr;

    dsprotectpatchv1_overlay_id = _overlayId;
    dsprotectpatchv1_base_offset = _overlayOffset;

    switch (_version)
    {
        case DSProtectVersion::v1_05:
        {
            static u16 v105Offsets[] = { 0x858, 0x934, 0xA10, 0xAEC, 0xBC8, 0xC94 };
            SetOldV1Offsets(v105Offsets);

            dsprotectpatchv1_nitro_static_init_offset = 0; // does not exist
            dsprotectpatchv1_protectb1_return_value = 0;
            dsprotectpatchv1_protectb2_return_value = 0;
            dsprotectpatchv1_protectb3_return_value = 0;
            dsprotectpatchv1_notprotectb1_return_value = 1;
            dsprotectpatchv1_notprotectb2_return_value = 1;
            dsprotectpatchv1_notprotectb3_return_value = 1;
            // Remove xor 0x320 from the callback pointer
            dsprotectpatchv1_moveCallback = 0xE230EE32; // eors lr, r0, #0x320
            break;
        }
        case DSProtectVersion::v1_06:
        {
            static u16 v106Offsets[] = { 0x860, 0x938, 0xA10, 0xAE8, 0xBC0, 0xC88 };
            SetOldV1Offsets(v106Offsets);

            dsprotectpatchv1_nitro_static_init_offset = 0; // does not exist
            dsprotectpatchv1_protectb1_return_value = 0;
            dsprotectpatchv1_protectb2_return_value = 0;
            dsprotectpatchv1_protectb3_return_value = 0;
            dsprotectpatchv1_notprotectb1_return_value = 1;
            dsprotectpatchv1_notprotectb2_return_value = 1;
            dsprotectpatchv1_notprotectb3_return_value = 1;
            break;
        }
        case DSProtectVersion::v1_08:
        {
            static u16 v108Offsets[] = { 0x858, 0x930, 0xA08, 0xAE0, 0xBB8, 0xC94 };
            SetOldV1Offsets(v108Offsets);

            dsprotectpatchv1_nitro_static_init_offset = 0; // does not exist
            dsprotectpatchv1_protectb1_return_value = 0;
            dsprotectpatchv1_protectb2_return_value = 0;
            dsprotectpatchv1_protectb3_return_value = 0;
            dsprotectpatchv1_notprotectb1_return_value = 1;
            dsprotectpatchv1_notprotectb2_return_value = 1;
            dsprotectpatchv1_notprotectb3_return_value = 1;
            break;
        }
        case DSProtectVersion::v1_10:
        {
            static u16 v110Offsets[] = { 0x860, 0x940, 0xA20, 0xB00, 0xBE0, 0xCB0 };
            SetOldV1Offsets(v110Offsets);

            dsprotectpatchv1_nitro_static_init_offset = 0; // does not exist
            dsprotectpatchv1_protectb1_return_value = 0;
            dsprotectpatchv1_protectb2_return_value = 0;
            dsprotectpatchv1_protectb3_return_value = 0;
            dsprotectpatchv1_notprotectb1_return_value = 1;
            dsprotectpatchv1_notprotectb2_return_value = 1;
            dsprotectpatchv1_notprotectb3_return_value = 1;
            break;
        }
        case DSProtectVersion::v1_20:
        {
            switch (_functionMask)
            {
                case 0b001111:
                    dsprotectpatchv1_stub_protectb1_offset = 0x864;
                    dsprotectpatchv1_stub_notprotectb1_offset = 0x94C;
                    dsprotectpatchv1_stub_protectb2_offset = 0xA34;
                    dsprotectpatchv1_stub_notprotectb2_offset = 0xB1C;
                    dsprotectpatchv1_stub_protectb3_offset = 0; // does not exist
                    dsprotectpatchv1_stub_notprotectb3_offset = 0; // does not exist
                    break;

                case 0b010101:
                    dsprotectpatchv1_stub_protectb1_offset = 0x644;
                    dsprotectpatchv1_stub_notprotectb1_offset = 0; // does not exist
                    dsprotectpatchv1_stub_protectb2_offset = 0x72C;
                    dsprotectpatchv1_stub_notprotectb2_offset = 0; // does not exist
                    dsprotectpatchv1_stub_protectb3_offset = 0x814;
                    dsprotectpatchv1_stub_notprotectb3_offset = 0; // does not exist
                    break;

                case 0b010110:
                    dsprotectpatchv1_stub_protectb1_offset = 0; // does not exist
                    dsprotectpatchv1_stub_notprotectb1_offset = 0x644;
                    dsprotectpatchv1_stub_protectb2_offset = 0x72C;
                    dsprotectpatchv1_stub_notprotectb2_offset = 0; // does not exist
                    dsprotectpatchv1_stub_protectb3_offset = 0x814;
                    dsprotectpatchv1_stub_notprotectb3_offset = 0; // does not exist
                    break;

                case 0b011001:
                    dsprotectpatchv1_stub_protectb1_offset = 0x644;
                    dsprotectpatchv1_stub_notprotectb1_offset = 0; // does not exist
                    dsprotectpatchv1_stub_protectb2_offset = 0;
                    dsprotectpatchv1_stub_notprotectb2_offset = 0x72C;
                    dsprotectpatchv1_stub_protectb3_offset = 0x814;
                    dsprotectpatchv1_stub_notprotectb3_offset = 0; // does not exist
                    break;

                case 0b110101:
                    dsprotectpatchv1_stub_protectb1_offset = 0x644;
                    dsprotectpatchv1_stub_notprotectb1_offset = 0; // does not exist
                    dsprotectpatchv1_stub_protectb2_offset = 0x72C;
                    dsprotectpatchv1_stub_notprotectb2_offset = 0; // does not exist
                    dsprotectpatchv1_stub_protectb3_offset = 0x814;
                    dsprotectpatchv1_stub_notprotectb3_offset = 0x8DC;
                    break;

                case 0b111111:
                    dsprotectpatchv1_stub_protectb1_offset = 0x864;
                    dsprotectpatchv1_stub_notprotectb1_offset = 0x94C;
                    dsprotectpatchv1_stub_protectb2_offset = 0xA34;
                    dsprotectpatchv1_stub_notprotectb2_offset = 0xB1C;
                    dsprotectpatchv1_stub_protectb3_offset = 0xC04;
                    dsprotectpatchv1_stub_notprotectb3_offset = 0xCCC;
                    break;

                default:
                    LOG_WARNING("Unsupported DSProtect function mask\n");
                    return nextPatch;
            }
            dsprotectpatchv1_nitro_static_init_offset = 0; // does not exist
            dsprotectpatchv1_protectb1_return_value = 0;
            dsprotectpatchv1_protectb2_return_value = 0;
            dsprotectpatchv1_protectb3_return_value = 0;
            dsprotectpatchv1_notprotectb1_return_value = 1;
            dsprotectpatchv1_notprotectb2_return_value = 1;
            dsprotectpatchv1_notprotectb3_return_value = 1;
            break;
        }
        case DSProtectVersion::v1_22:
        {
            switch (_functionMask)
            {
                case 0b010101:
                    dsprotectpatchv1_stub_protectb1_offset = 0x64C;
                    dsprotectpatchv1_stub_notprotectb1_offset = 0; // does not exist
                    dsprotectpatchv1_stub_protectb2_offset = 0x734;
                    dsprotectpatchv1_stub_notprotectb2_offset = 0; // does not exist
                    dsprotectpatchv1_stub_protectb3_offset = 0x81C;
                    dsprotectpatchv1_stub_notprotectb3_offset = 0; // does not exist
                    break;

                case 0b010110:
                    dsprotectpatchv1_stub_protectb1_offset = 0; // does not exist
                    dsprotectpatchv1_stub_notprotectb1_offset = 0x64C;
                    dsprotectpatchv1_stub_protectb2_offset = 0x734;
                    dsprotectpatchv1_stub_notprotectb2_offset = 0; // does not exist
                    dsprotectpatchv1_stub_protectb3_offset = 0x81C;
                    dsprotectpatchv1_stub_notprotectb3_offset = 0; // does not exist
                    break;

                case 0b010111:
                    dsprotectpatchv1_stub_protectb1_offset = 0x754;
                    dsprotectpatchv1_stub_notprotectb1_offset = 0x83C;
                    dsprotectpatchv1_stub_protectb2_offset = 0x924;
                    dsprotectpatchv1_stub_notprotectb2_offset = 0; // does not exist
                    dsprotectpatchv1_stub_protectb3_offset = 0xA0C;
                    dsprotectpatchv1_stub_notprotectb3_offset = 0; // does not exist
                    break;

                case 0b011111:
                    dsprotectpatchv1_stub_protectb1_offset = 0x874;
                    dsprotectpatchv1_stub_notprotectb1_offset = 0x95C;
                    dsprotectpatchv1_stub_protectb2_offset = 0xA44;
                    dsprotectpatchv1_stub_notprotectb2_offset = 0xB2C;
                    dsprotectpatchv1_stub_protectb3_offset = 0xC14;
                    dsprotectpatchv1_stub_notprotectb3_offset = 0; // does not exist
                    break;

                case 0b111111:
                    dsprotectpatchv1_stub_protectb1_offset = 0x874;
                    dsprotectpatchv1_stub_notprotectb1_offset = 0x95C;
                    dsprotectpatchv1_stub_protectb2_offset = 0xA44;
                    dsprotectpatchv1_stub_notprotectb2_offset = 0xB2C;
                    dsprotectpatchv1_stub_protectb3_offset = 0xC14;
                    dsprotectpatchv1_stub_notprotectb3_offset = 0xCDC;
                    break;

                default:
                    LOG_WARNING("Unsupported DSProtect function mask\n");
                    return nextPatch;
            }

            dsprotectpatchv1_nitro_static_init_offset = 0; // does not exist
            dsprotectpatchv1_protectb1_return_value = 0;
            dsprotectpatchv1_protectb2_return_value = 0;
            dsprotectpatchv1_protectb3_return_value = 0;
            dsprotectpatchv1_notprotectb1_return_value = 1;
            dsprotectpatchv1_notprotectb2_return_value = 1;
            dsprotectpatchv1_notprotectb3_return_value = 1;
            break;
        }
        case DSProtectVersion::v1_23:
        case DSProtectVersion::v1_23Z:
        {
            dsprotectpatchv1_stub_protectb1_offset = 0x410;
            dsprotectpatchv1_stub_protectb2_offset = 0x500;
            dsprotectpatchv1_stub_protectb3_offset = 0x5F0;
            dsprotectpatchv1_stub_notprotectb1_offset = 0x488;
            dsprotectpatchv1_stub_notprotectb2_offset = 0x578;
            dsprotectpatchv1_stub_notprotectb3_offset = 0x668;
            // this seems to be a decrypt function that is called from 5 static init functions
            dsprotectpatchv1_nitro_static_init_offset = 0xD78;
            dsprotectpatchv1_protectb1_return_value = 0;
            dsprotectpatchv1_protectb2_return_value = 0;
            dsprotectpatchv1_protectb3_return_value = 0;
            dsprotectpatchv1_notprotectb1_return_value = 1;
            dsprotectpatchv1_notprotectb2_return_value = 1;
            dsprotectpatchv1_notprotectb3_return_value = 1;
            break;
        }
        case DSProtectVersion::v1_25:
        case DSProtectVersion::v1_26:
        {
            dsprotectpatchv1_stub_protectb1_offset = 0x3DC;
            dsprotectpatchv1_stub_protectb2_offset = 0x4CC;
            dsprotectpatchv1_stub_protectb3_offset = 0x5BC;
            dsprotectpatchv1_stub_notprotectb1_offset = 0x454;
            dsprotectpatchv1_stub_notprotectb2_offset = 0x544;
            dsprotectpatchv1_stub_notprotectb3_offset = 0x634;
            dsprotectpatchv1_nitro_static_init_offset = 0x6AC;
            break;
        }
        case DSProtectVersion::v1_27:
        {
            dsprotectpatchv1_stub_protectb1_offset = 0x3A0;
            dsprotectpatchv1_stub_protectb2_offset = 0x418;
            dsprotectpatchv1_stub_protectb3_offset = 0x490;
            dsprotectpatchv1_stub_notprotectb1_offset = 0x3DC;
            dsprotectpatchv1_stub_notprotectb2_offset = 0x454;
            dsprotectpatchv1_stub_notprotectb3_offset = 0x4CC;
            dsprotectpatchv1_nitro_static_init_offset = 0x508;
            dsprotectpatchv1_loadReturnValue = 0; // Return the return value of the callback
            break;
        }
        case DSProtectVersion::v1_28:
        {
            dsprotectpatchv1_stub_protectb1_offset = 0x3A0;
            dsprotectpatchv1_stub_protectb2_offset = 0x418;
            dsprotectpatchv1_stub_protectb3_offset = 0x490;
            dsprotectpatchv1_stub_notprotectb1_offset = 0x3DC;
            dsprotectpatchv1_stub_notprotectb2_offset = 0x454;
            dsprotectpatchv1_stub_notprotectb3_offset = 0x4CC;
            dsprotectpatchv1_nitro_static_init_offset = 0x508;
            dsprotectpatchv1_loadReturnValue = 0; // Return the return value of the callback
            break;
        }
        default:
        {
            LOG_WARNING("Unsupported DSProtect version\n");
            return nextPatch;
        }
    }
    dsprotectpatchv1_nextAddress = (u32)nextPatch;

    u32 patch1Size = SECTION_SIZE(dsprotectpatchv1_part1);
    void* patch1Address = patchContext.GetPatchHeap().Alloc(patch1Size);
    u32 patch2Size = SECTION_SIZE(dsprotectpatchv1_part2);
    void* patch2Address = patchContext.GetPatchHeap().Alloc(patch2Size);

    dsprotectpatchv1_part2_address = (u32)&dsprotectpatchv1_entry2 - (u32)SECTION_START(dsprotectpatchv1_part2) + (u32)patch2Address;

    u32 entryAddress1 = (u32)&dsprotectpatchv1_entry1 - (u32)SECTION_START(dsprotectpatchv1_part1) + (u32)patch1Address;
    memcpy(patch1Address, SECTION_START(dsprotectpatchv1_part1), patch1Size);
    memcpy(patch2Address, SECTION_START(dsprotectpatchv1_part2), patch2Size);

    return (const void*)entryAddress1;
}

struct ds_protect_v2_patch_t
{
    u32 checksumFix;
    u16 stubProtectb1Offset;
    u16 stubProtectb2Offset;
    u16 stubProtectb3Offset;
    u16 stubProtectb4Offset;
    u16 amInitOffset;
    u16 storeChecksumFix;
};

static const ds_protect_v2_patch_t sDSProtectV2Patches[] =
{
    { 0x7CDBA8BD, 0x670, 0x6AC, 0x6E8, 0x724, 0x760, THUMB_STR_IMM(THUMB_R7, THUMB_R0, 0) },      // v2.00
    { 0xA6FD7CE1, 0x1120, 0x115C, 0x1198, 0x11D4, 0x1210, THUMB_STR_IMM(THUMB_R7, THUMB_R0, 0) }, // v2.01
    { 0x45C535B6, 0x129C, 0x1324, 0x13AC, 0x1434, 0x14BC, THUMB_STR_IMM(THUMB_R7, THUMB_R0, 0) }, // v2.03
    { 0x32E120D5, 0x132C, 0x13D8, 0x1484, 0x1530, 0x15DC, THUMB_STR_IMM(THUMB_R7, THUMB_R0, 8) }, // v2.05
};

const void* DSProtectOverlayPatch::InsertPatchV2(PatchContext& patchContext) const
{
    dsprotectpatchv2_nextAddress = next ? (u32)next->InsertPatch(patchContext) : 0u;
    dsprotectpatchv2_overlay_id = _overlayId;
    dsprotectpatchv2_base_offset = _overlayOffset;

    const auto& patch = sDSProtectV2Patches[(u32)_version - (u32)DSProtectVersion::v2_00];
    dsprotectpatchv2_checksum_fix = patch.checksumFix;
    dsprotectpatchv2_stub_protectb1_offset = patch.stubProtectb1Offset;
    dsprotectpatchv2_stub_protectb2_offset = patch.stubProtectb2Offset;
    dsprotectpatchv2_stub_protectb3_offset = patch.stubProtectb3Offset;
    dsprotectpatchv2_stub_protectb4_offset = patch.stubProtectb4Offset;
    dsprotectpatchv2_am_init_offset = patch.amInitOffset;
    dsprotectpatchv2_store_checksum_fix = patch.storeChecksumFix;

    u32 patchSize = SECTION_SIZE(dsprotectpatchv2);
    void* patchAddress = patchContext.GetPatchHeap().Alloc(patchSize);

    u32 entryAddress = (u32)&dsprotectpatchv2_entry - (u32)SECTION_START(dsprotectpatchv2) + (u32)patchAddress;
    memcpy(patchAddress, SECTION_START(dsprotectpatchv2), patchSize);

    return (const void*)entryAddress;
}

struct ds_protect_v2s_patch_t
{
    u32 checksumFix;
    u16 instantDetectOffset;
    u16 storeChecksumFix;
};

static const ds_protect_v2s_patch_t sDSProtectV2sPatches[] =
{
    { 0xEDEA7F01, 0x1BC, THUMB_STR_IMM(THUMB_R7, THUMB_R0, 0) }, // v2.00s
    { 0xEDEA7F1F, 0x1BC, THUMB_STR_IMM(THUMB_R7, THUMB_R0, 0) }, // v2.01s
    { 0x167325AA, 0x20C, THUMB_STR_IMM(THUMB_R7, THUMB_R0, 0) }, // v2.03s
    { 0x32E120D5, 0x230, THUMB_STR_IMM(THUMB_R7, THUMB_R0, 4) }  // v2.05s
};

const void* DSProtectOverlayPatch::InsertPatchV2s(PatchContext& patchContext) const
{
    dsprotectpatchv2s_nextAddress = next ? (u32)next->InsertPatch(patchContext) : 0u;
    dsprotectpatchv2s_overlay_id = _overlayId;

    const auto& patch = sDSProtectV2sPatches[(u32)_version - (u32)DSProtectVersion::v2_00s];
    dsprotectpatchv2s_checksum_fix = patch.checksumFix;
    dsprotectpatchv2s_stub_instantdetect_offset = patch.instantDetectOffset + _overlayOffset;
    dsprotectpatchv2s_store_checksum_fix = patch.storeChecksumFix;

    u32 patchSize = SECTION_SIZE(dsprotectpatchv2s);
    void* patchAddress = patchContext.GetPatchHeap().Alloc(patchSize);
    u32 entryAddress = (u32)&dsprotectpatchv2s_entry - (u32)SECTION_START(dsprotectpatchv2s) + (u32)patchAddress;
    memcpy(patchAddress, SECTION_START(dsprotectpatchv2s), patchSize);

    return (const void*)entryAddress;
}