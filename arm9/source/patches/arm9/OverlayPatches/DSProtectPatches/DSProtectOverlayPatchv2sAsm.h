#pragma once
#include "sections.h"

DEFINE_SECTION_SYMBOLS(dsprotectpatchv2s);

extern "C" void dsprotectpatchv2s_entry();
extern u32 dsprotectpatchv2s_stub_instantdetect_offset;
extern u32 dsprotectpatchv2s_overlay_id;
extern u32 dsprotectpatchv2s_nextAddress;
extern u32 dsprotectpatchv2s_checksum_fix;

extern u16 dsprotectpatchv2s_store_checksum_fix;
