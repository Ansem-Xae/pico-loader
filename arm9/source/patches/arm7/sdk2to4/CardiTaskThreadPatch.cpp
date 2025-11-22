#include "common.h"
#include <algorithm>
#include <array>
#include "fileInfo.h"
#include "patches/PatchContext.h"
#include "patches/arm7/ReadSaveAsm.h"
#include "patches/arm7/WriteSaveAsm.h"
#include "patches/FunctionSignature.h"
#include "patches/platform/LoaderPlatform.h"
#include "patches/arm7/SaveOffsetToSdSectorAsm.h"
#include "patches/OffsetToSectorRemapAsm.h"
#include "patches/arm7/CardiTaskThreadPatchAsm.h"
#include "thumbInstructions.h"
#include "CardiTaskThreadPatch.h"

static constexpr auto sSignaturesArm = std::to_array<const FunctionSignature>
({
    { (const u32[]) { 0xE92D4FF0u, 0xE24DD004u, 0xE59FA108u, 0xE28A5040u }, 0x020029A0, 0x02005015 },
    { (const u32[]) { 0xE92D4FF0u, 0xE24DD004u, 0xE59FA10Cu, 0xE28A5040u }, 0x02004F50, 0x02017532 },
    { (const u32[]) { 0xE92D4FF0u, 0xE24DD004u, 0xE59F91DCu, 0xE2895044u }, 0x02017532, 0x02017532 },
    { (const u32[]) { 0xE92D4FF0u, 0xE24DD004u, 0xE59F91E0u, 0xE2895044u }, 0x02017532, 0x02027534 },
    { (const u32[]) { 0xE92D4FF0u, 0xE24DD004u, 0xE59F91E0u, 0xE2895048u }, 0x030028A0, 0x03004E86 },
    { (const u32[]) { 0xE92D4FF0u, 0xE24DD004u, 0xE59F91D4u, 0xE2895048u }, 0x03004F4C, 0x03017534 },
    { (const u32[]) { 0xE92D4FF0u, 0xE24DD004u, 0xE59F91F8u, 0xE2895048u }, 0x03017530, 0x04017530 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4200u, 0xE3A05000u, 0xEBFFEADCu }, 0x04002774, 0x04017533 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4200u, 0xE3A05000u, 0xEBFFEACEu }, 0x04007531, 0x04007531 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4200u, 0xE3A05000u, 0xEBFFEABFu }, 0x04007530, 0x04007531 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4200u, 0xE3A05000u, 0xEBFFEAAAu }, 0x04017530, 0x04017533 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4200u, 0xE3A05000u, 0xEBFFEA94u }, 0x04017530, 0x04017533 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4200u, 0xE3A05000u, 0xEBFFEA8Du }, 0x04017531, 0x04017533 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4200u, 0xE3A05000u, 0xEBFFEA77u }, 0x04017533, 0x04017533 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4224u, 0xE3A05000u, 0xEBFFEA70u }, 0x04027530, 0x04027539 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4224u, 0xE3A05000u, 0xEBFFEA8Du }, 0x04027530, 0x04027539 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4224u, 0xE3A05000u, 0xEBFFEA6Cu }, 0x04027531, 0x04027531 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4224u, 0xE3A05000u, 0xEBFFEA77u }, 0x04027531, 0x04027539 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4224u, 0xE3A05000u, 0xEBFFEA5Au }, 0x04027533, 0x04027533 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4200u, 0xE3A05000u, 0xEBFFFAD7u }, 0x03017531, 0x03017531 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4200u, 0xE3A05000u, 0xEBFFFAA5u }, 0x03027531, 0x03027531 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4200u, 0xE3A05000u, 0xEBFFEADBu }, 0x03027531, 0x03027531 },
    { (const u32[]) { 0xE92D41F0u, 0xE59F4224u, 0xE3A05000u, 0xEBFFEA68u }, 0x04027533, 0x04027533 }
});

static constexpr auto sSignaturesThumb = std::to_array<const FunctionSignature>
({
    { (const u32[]) { 0xB081B5F0u, 0x1C2E4D2Du, 0x27103640u, 0xF7FC2400u }, 0x02004FB0, 0x02004FB4 },
    { (const u32[]) { 0xB081B5F0u, 0x1C2E4D2Bu, 0x27103640u, 0xF7FC2400u }, 0x02007531, 0x02017532 },
    { (const u32[]) { 0xB081B5F0u, 0x1C264C57u, 0x27103644u, 0xF7FC2500u }, 0x02027532, 0x02027535 },
    { (const u32[]) { 0xB081B5F0u, 0x1C264C55u, 0x27103648u, 0xF7FC2500u }, 0x03007530, 0x03012776 },
    { (const u32[]) { 0xB081B5F0u, 0x1C264C5Cu, 0x27033648u, 0xF7FC2500u }, 0x03017531, 0x03027532 },
    { (const u32[]) { 0x4C5CB5F8u, 0xF7FC2500u, 0x1C26FAB3u, 0x36481C07u }, 0x04007531, 0x04007531 },
    { (const u32[]) { 0x4C5CB5F8u, 0xF7FC2500u, 0x1C26FA97u, 0x36481C07u }, 0x04007531, 0x04007531 },
    { (const u32[]) { 0x4C5CB5F8u, 0xF7FC2500u, 0x1C26FA4Fu, 0x36481C07u }, 0x04017530, 0x04017533 },
    { (const u32[]) { 0x4C5CB5F8u, 0xF7FC2500u, 0x1C26FA23u, 0x36481C07u }, 0x04017531, 0x04017531 },
    { (const u32[]) { 0x4D62B5F8u, 0xF7FC2400u, 0x1C2EFA2Du, 0x36481C07u }, 0x04027531, 0x04027539 }
});

bool CardiTaskThreadPatch::FindPatchTarget(PatchContext& patchContext)
{
    for (const auto& signature : sSignaturesArm)
    {
        if (CheckSignature(patchContext, signature))
        {
            if (signature.GetPattern()[2] == 0xE59F91DCu)
            {
                _peach = true;
            }
            else if (signature.GetPattern()[3] == 0xEBFFFAD7u
                || signature.GetPattern()[3] == 0xEBFFFAA5u
                || signature.GetPattern()[3] == 0xEBFFEADBu)
            {
                _pokemonDownloader = true;
            }
            break;
        }
    }

    if (!_cardiTaskThread)
    {
        for (const auto& signature : sSignaturesThumb)
        {
            if (CheckSignature(patchContext, signature))
            {
                _thumb = true;
                break;
            }
        }
    }

    if (!_cardiTaskThread)
    {
        LOG_FATAL("CARDi_TaskThread not found\n");
    }

    return _cardiTaskThread != nullptr;
}

void CardiTaskThreadPatch::ApplyPatch(PatchContext& patchContext)
{
    if (!_cardiTaskThread)
        return;
    //0xA4 = ADDLS PC, PC, R0,LSL#2

    u32 patch1Size = SECTION_SIZE(patch_carditaskthread);
    void* patch1Address = patchContext.GetPatchHeap().Alloc(patch1Size);
    auto loaderPlatform = patchContext.GetLoaderPlatform();
    const SdReadPatchCode* readPatchCode;
    const SdWritePatchCode* writePatchCode;
    const SectorRemapPatchCode* sectorRemapPatchCode;
    readPatchCode = loaderPlatform->CreateSdReadPatchCode(
        patchContext.GetPatchCodeCollection(), patchContext.GetPatchHeap());
    writePatchCode = loaderPlatform->CreateSdWritePatchCode(
        patchContext.GetPatchCodeCollection(), patchContext.GetPatchHeap());
    sectorRemapPatchCode = patchContext.GetPatchCodeCollection().AddUniquePatchCode<SaveOffsetToSdSectorPatchCode>
    (
        patchContext.GetPatchHeap(),
        (const save_file_info_t*)((u32)SHARED_SAVE_FILE_INFO - 0x02F00000 + 0x02700000)
    );
    void* tmpBuffer = patchContext.GetPatchHeap().Alloc(512);
    auto readSavePatchCode = patchContext.GetPatchCodeCollection().AddUniquePatchCode<ReadSavePatchCode>
    (
        patchContext.GetPatchHeap(),
        sectorRemapPatchCode,
        readPatchCode,
        tmpBuffer
    );
    auto writeSavePatchCode = patchContext.GetPatchCodeCollection().AddUniquePatchCode<WriteSavePatchCode>
    (
        patchContext.GetPatchHeap(),
        sectorRemapPatchCode,
        readPatchCode,
        writePatchCode,
        tmpBuffer
    );

    __patch_carditaskthread_readsave_asm_address = (u32)readSavePatchCode->GetReadSaveFunction();
    __patch_carditaskthread_writesave_asm_address = (u32)writeSavePatchCode->GetWriteSaveFunction();

    u32 entryAddress;
    u32 patchOffset;
    if (_thumb)
    {
        if (patchContext.GetSdkVersion() >= 0x4027531)
        {
            patchOffset = 0x90;
            entryAddress = (u32)&__patch_carditaskthread_entry - (u32)SECTION_START(patch_carditaskthread) + (u32)patch1Address;
            __patch_carditaskthread_mov_cardicommon_to_r4 = THUMB_MOVS_REG(THUMB_R4, THUMB_R5);
            __patch_carditaskthread_successoffset = *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x0C) + 9;
            __patch_carditaskthread_failoffset = *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x14) + 9;

            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x00) = THUMB_LDR_PC_IMM(THUMB_R1, 4);
            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x02) = THUMB_MOV_HIREG(THUMB_HI_LR, THUMB_HI_PC);
            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x04) = THUMB_BX(THUMB_R1);
            *(u32*)((u8*)_cardiTaskThread + patchOffset + 0x08) = entryAddress;
        }
        else if (patchContext.GetSdkVersion() >= 0x4007530)
        {
            patchOffset = 0x90;
            entryAddress = (u32)&__patch_carditaskthread_entry - (u32)SECTION_START(patch_carditaskthread) + (u32)patch1Address;
            __patch_carditaskthread_mov_cardicommon_to_r4 = THUMB_NOP;
            __patch_carditaskthread_successoffset = *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x0C) + 9;
            __patch_carditaskthread_failoffset = *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x14) + 9;

            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x00) = THUMB_LDR_PC_IMM(THUMB_R1, 4);
            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x02) = THUMB_MOV_HIREG(THUMB_HI_LR, THUMB_HI_PC);
            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x04) = THUMB_BX(THUMB_R1);
            *(u32*)((u8*)_cardiTaskThread + patchOffset + 0x08) = entryAddress;
        }
        else if (patchContext.GetSdkVersion() >= 0x3027531)
        {
            patchOffset = 0x98;
            entryAddress = (u32)&__patch_carditaskthread_entry - (u32)SECTION_START(patch_carditaskthread) + (u32)patch1Address;
            __patch_carditaskthread_mov_cardicommon_to_r4 = THUMB_NOP;
            __patch_carditaskthread_successoffset = *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x0E) + 8;
            __patch_carditaskthread_failoffset = *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x18) + 8;

            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x00) = THUMB_LDR_PC_IMM(THUMB_R1, 4);
            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x02) = THUMB_MOV_HIREG(THUMB_HI_LR, THUMB_HI_PC);
            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x04) = THUMB_BX(THUMB_R1);
            *(u32*)((u8*)_cardiTaskThread + patchOffset + 0x08) = entryAddress;
        }
        else if (patchContext.GetSdkVersion() >= 0x3012776
            || (patchContext.GetSdkVersion() >= 0x2027532 && patchContext.GetSdkVersion() <= 0x2027535))
        {
            patchOffset = 0x82;
            entryAddress = (u32)&__patch_carditaskthread_entry - (u32)SECTION_START(patch_carditaskthread) + (u32)patch1Address;
            __patch_carditaskthread_mov_cardicommon_to_r4 = THUMB_NOP;
            __patch_carditaskthread_successoffset = *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x0E) + 8;
            __patch_carditaskthread_failoffset = *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x18) + 8;

            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x00) = THUMB_LDR_PC_IMM(THUMB_R1, 4);
            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x02) = THUMB_MOV_HIREG(THUMB_HI_LR, THUMB_HI_PC);
            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x04) = THUMB_BX(THUMB_R1);
            *(u32*)((u8*)_cardiTaskThread + patchOffset + 0x06) = entryAddress;
        }
        else if (patchContext.GetSdkVersion() >= 0x2004FB0)
        {
            patchOffset = patchContext.GetSdkVersion() >= 0x2007531 ? 0x60 : 0x64;
            entryAddress = (u32)&__patch_carditaskthread_entry - (u32)SECTION_START(patch_carditaskthread) + (u32)patch1Address;
            __patch_carditaskthread_mov_cardicommon_to_r4 = THUMB_MOVS_REG(THUMB_R4, THUMB_R5);
            __patch_carditaskthread_failoffset = 0;
            __patch_carditaskthread_successoffset = 0;
            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x00) = THUMB_LDR_PC_IMM(THUMB_R1, 0);
            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x02) = 0xE001; // jump over entryAddress
            *(u32*)((u8*)_cardiTaskThread + patchOffset + 0x04) = entryAddress;
            *(u16*)((u8*)_cardiTaskThread + patchOffset + 0x08) = THUMB_NOP;
        }
        else
        {
            LOG_FATAL("Unsupported thumb CARDi_TaskThread\n");
            while(1);
        }
    }
    else
    {
        if (patchContext.GetSdkVersion() < 0x2020000 && !_peach)
        {
            // uses a table dispatch
            //0xA4 = ldr r1,= table
            __patch_carditaskthread_failoffset = 4;
            __patch_carditaskthread_successoffset = 4;
            if (patchContext.GetSdkVersion() >= 0x2005015)//0x2007532)//0x2012774)
                patchOffset = 0xA4;
            else
                patchOffset = 0xA0;

            entryAddress = (u32)&__patch_carditaskthread_entry - (u32)SECTION_START(patch_carditaskthread) + (u32)patch1Address;
            __patch_carditaskthread_mov_cardicommon_to_r4 = THUMB_MOV_HIREG(THUMB_R4, THUMB_HI_R10);

            *(u32*)((u8*)_cardiTaskThread + patchOffset + 0x00) = 0xe59f0004; // ldr r0,= entryAddress
            *(u32*)((u8*)_cardiTaskThread + patchOffset + 0x04) = 0xe1a0e00f; // mov lr, pc
            *(u32*)((u8*)_cardiTaskThread + patchOffset + 0x08) = 0xe12fff10; // bx r0
            *(u32*)((u8*)_cardiTaskThread + patchOffset + 0x0C) = entryAddress;
        }
        else
        {
            if (patchContext.GetSdkVersion() >= 0x4027531)
            {
                patchOffset = 0xA8;
                entryAddress = (u32)&__patch_carditaskthread_entry_sdk4 - (u32)SECTION_START(patch_carditaskthread) + (u32)patch1Address;
                __patch_carditaskthread_mov_cardicommon_to_r4 = THUMB_NOP;
            }
            else if (patchContext.GetSdkVersion() >= 0x4007530 || _pokemonDownloader)
            {
                patchOffset = 0xA8;
                entryAddress = (u32)&__patch_carditaskthread_entry - (u32)SECTION_START(patch_carditaskthread) + (u32)patch1Address;
                __patch_carditaskthread_mov_cardicommon_to_r4 = THUMB_NOP;
            }
            else if (patchContext.GetSdkVersion() >= 0x3017531)
            {
                patchOffset = 0xB4;
                entryAddress = (u32)&__patch_carditaskthread_entry - (u32)SECTION_START(patch_carditaskthread) + (u32)patch1Address;
                __patch_carditaskthread_mov_cardicommon_to_r4 = THUMB_MOV_HIREG(THUMB_R4, THUMB_HI_R9);
            }
            else
            {
                patchOffset = 0x9C;
                entryAddress = (u32)&__patch_carditaskthread_entry - (u32)SECTION_START(patch_carditaskthread) + (u32)patch1Address;
                __patch_carditaskthread_mov_cardicommon_to_r4 = THUMB_MOV_HIREG(THUMB_R4, THUMB_HI_R9);
            }

            *(u32*)((u8*)_cardiTaskThread + patchOffset + 0x00) = 0xe59f000C; // ldr r0,= entryAddress
            *(u32*)((u8*)_cardiTaskThread + patchOffset + 0x04) = 0xe1a0e00f; // mov lr, pc
            *(u32*)((u8*)_cardiTaskThread + patchOffset + 0x08) = 0xe12fff10; // bx r0
            *(u32*)((u8*)_cardiTaskThread + patchOffset + 0x14) = entryAddress;
        }
    }

    memcpy(patch1Address, SECTION_START(patch_carditaskthread), patch1Size);
}

bool CardiTaskThreadPatch::CheckSignature(const PatchContext& patchContext, const FunctionSignature& signature)
{
    if (patchContext.GetSdkVersion() >= signature.GetMinimumSdkVersion() &&
        patchContext.GetSdkVersion() <= signature.GetMaximumSdkVersion())
    {
        _cardiTaskThread = patchContext.FindPattern32(signature.GetPattern(), 16);
        if (_cardiTaskThread)
        {
            LOG_DEBUG("CARDi_TaskThread found at 0x%p\n", _cardiTaskThread);
            return true;
        }
    }

    return false;
}
