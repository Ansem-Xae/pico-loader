#pragma once
#include "sections.h"

DEFINE_SECTION_SYMBOLS(dsprotectpatchv2);

extern "C" void dsprotectpatchv2_entry();
extern u16 dsprotectpatchv2_stub_protectb1_offset;
extern u16 dsprotectpatchv2_stub_protectb2_offset;
extern u16 dsprotectpatchv2_stub_protectb3_offset;
extern u16 dsprotectpatchv2_stub_protectb4_offset;
extern u16 dsprotectpatchv2_am_init_offset;
extern u16 dsprotectpatchv2_overlay_id;
extern u32 dsprotectpatchv2_nextAddress;
extern u32 dsprotectpatchv2_checksum_fix;
extern u32 dsprotectpatchv2_base_offset;

extern u16 dsprotectpatchv2_store_checksum_fix;
