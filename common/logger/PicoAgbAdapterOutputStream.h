#pragma once
#include "IOutputStream.h"

class PicoAgbAdapterOutputStream : public IOutputStream
{
public:
    PicoAgbAdapterOutputStream() { }

    void Write(const char* str) override;    
    void Flush() override { }
};