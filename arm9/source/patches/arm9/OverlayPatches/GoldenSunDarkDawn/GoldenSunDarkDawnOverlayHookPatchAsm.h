#pragma once
#include "sections.h"

DEFINE_SECTION_SYMBOLS(gsddoverlayhookpatch);

extern "C" void gsddoverlayhookpatch_entry();

extern u32 gsddoverlayhookpatch_hookFuncAddress;
extern u32 gsddoverlayhookpatch_returnAddress;
