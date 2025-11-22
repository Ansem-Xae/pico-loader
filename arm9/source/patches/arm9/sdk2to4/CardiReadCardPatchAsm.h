#pragma once
#include "sections.h"

DEFINE_SECTION_SYMBOLS(patch_cardireadcard);

extern "C" void patch_cardireadcard_entry();

extern u32 __patch_cardireadcard_fix_cp15_asm_address;
extern u32 __patch_cardireadcard_rom_offset_to_sd_sector_asm_address;
extern u32 __patch_cardireadcard_sdread_asm_address;

extern u16 patch_cardireadcard_return_offset;
extern u16 patch_cardireadcard_mov_src_to_r0;
extern u16 patch_cardireadcard_mov_dst_to_r1;
extern u16 patch_cardireadcard_mov_cardicommon_to_r6;
extern u16 patch_cardireadcard_adjust_cardicommon_offset;
extern u16 patch_cardireadcard_mov_r3_to_dst;
