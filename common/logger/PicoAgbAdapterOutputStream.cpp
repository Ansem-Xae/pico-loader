#include <nds.h>
#include "picoAgbAdapter.h"
#include "PicoAgbAdapterOutputStream.h"

void PicoAgbAdapterOutputStream::Write(const char* str)
{
    char c;
    vu16* ring = PICO_AGB_PRINT_ARM9_RING;
    u32 writePtr = PICO_AGB_PRINT_ARM9_WRITE_PTR;
    u32 readPtr = PICO_AGB_PRINT_ARM9_READ_PTR;
    while ((c = *str++) != 0)
    {
        u32 newWritePtr = (writePtr + 1) & (PICO_AGB_PRINT_RING_LENGTH - 1);
        while (newWritePtr == readPtr)
        {
            PICO_AGB_PRINT_ARM9_WRITE_PTR = writePtr;
            readPtr = PICO_AGB_PRINT_ARM9_READ_PTR;
        }
        if (writePtr & 1)
            ring[writePtr >> 1] = (ring[writePtr >> 1] & 0xFF) | (c << 8);
        else
            ring[writePtr >> 1] = (ring[writePtr >> 1] & 0xFF00) | c;
        writePtr = newWritePtr;
    }
    PICO_AGB_PRINT_ARM9_WRITE_PTR = writePtr;
}