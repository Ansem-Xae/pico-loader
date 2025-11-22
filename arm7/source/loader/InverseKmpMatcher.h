#pragma once
#include "common.h"

template <typename T, u32 PatternLength>
class InverseKmpMatcher
{
    const T* _pattern;
    u8 _prefixFunc[PatternLength];
public:
    consteval InverseKmpMatcher(const T(&pattern)[PatternLength])
        : _pattern(pattern)
    {
        _prefixFunc[0] = 0;
        int k = 0;
        for (u32 q = 1; q < PatternLength; q++)
        {
            while (k > 0 && pattern[k] != pattern[q])
                k = _prefixFunc[k - 1];
            if (pattern[k] == pattern[q])
                k++;
            _prefixFunc[q] = k;
        }
    }

    int FindFirstOccurance(const T* data, u32 length) const
    {
        int q = 0;
        for (u32 i = 0; i < length; i++)
        {
            while (q > 0 && ~_pattern[q] != data[i])
                q = _prefixFunc[q - 1];
            if (~_pattern[q] == data[i])
                q++;
            if (q == PatternLength)
                return i - q + 1;
        }
        return -1;
    }
};