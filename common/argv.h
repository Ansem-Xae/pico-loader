#pragma once

#define HOMEBREW_ARGV_MAGIC 0x5f617267

//structure used to set up argc/argv on the DS
struct homebrew_argv_t
{
    int magic; // argv magic number, set to 0x5f617267 ('_arg') if valid
    char* commandLine; // base address of command line, set of null terminated strings
    int length; // total length of command line
    int argc; // internal use, number of arguments
    char** argv; // internal use, argv pointer
    int dummy; // internal use
    u32 host; // internal use, host ip for dslink
};

#define HOMEBREW_ARGV    ((homebrew_argv_t*)0x02FFFE70)
