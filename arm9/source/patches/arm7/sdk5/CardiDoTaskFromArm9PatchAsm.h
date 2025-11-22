#pragma once
#include "sections.h"

DEFINE_SECTION_SYMBOLS(patch_cardidotaskfromarm9);

extern "C" void __patch_cardidotaskfromarm9_entry();

extern u32 __patch_cardidotaskfromarm9_failoffset;
extern u32 __patch_cardidotaskfromarm9_successoffset;

extern u16 __patch_cardidotaskfromarm9_move_statereg;

extern u32 __patch_cardidotaskfromarm9_readsave_asm_address;
extern u32 __patch_cardidotaskfromarm9_writesave_asm_address;
