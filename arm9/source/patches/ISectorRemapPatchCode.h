#pragma once

class ISectorRemapPatchCode
{
protected:
    ISectorRemapPatchCode() { }

public:
    virtual const void* GetRemapFunction() const = 0;
};
