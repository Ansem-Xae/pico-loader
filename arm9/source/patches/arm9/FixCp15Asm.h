#pragma once
#include "sections.h"

DEFINE_SECTION_SYMBOLS(fixcp15);

extern "C" void fix_cp15_asm();
