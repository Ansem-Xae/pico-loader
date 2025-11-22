#include "common.h"
#include "ModuleParamsLocator.h"

module_params_ntr_t* ModuleParamsLocator::FindModuleParams(const nds_header_ntr_t* romHeader)
{
    //todo: static footer
    module_params_ntr_t* moduleParams = nullptr;
    if (romHeader->arm9ModuleParamsAddress != 0)
    {
        moduleParams = (module_params_ntr_t*)(romHeader->arm9LoadAddress + romHeader->arm9ModuleParamsAddress);
        if (moduleParams->magicBigEndian != MODULE_PARAMS_NTR_MAGIC_BE ||
            moduleParams->magicLittleEndian != MODULE_PARAMS_NTR_MAGIC_LE)
        {
            // try again by subtracting arm9 rom address
            moduleParams = (module_params_ntr_t*)((u8*)moduleParams - romHeader->arm9RomOffset);
            if (moduleParams->magicBigEndian != MODULE_PARAMS_NTR_MAGIC_BE ||
                moduleParams->magicLittleEndian != MODULE_PARAMS_NTR_MAGIC_LE)
            {
                LOG_DEBUG("Module params not found at header specified offset 0x%x\n", romHeader->arm9ModuleParamsAddress);
                moduleParams = nullptr;
            }
        }
    }
    else
    {
        LOG_DEBUG("Header specifies no module params address\n");
    }

    if (!moduleParams)
    {
        // try searching for the module params as not all roms have the address in the header
        // it should be within the first 0x1000 bytes of the arm9
        for (u32 i = 0; i < 0x1000; i += 4)
        {
            u32* ptr = (u32*)(romHeader->arm9LoadAddress + i);
            if (ptr[0] == MODULE_PARAMS_NTR_MAGIC_BE && ptr[1] == MODULE_PARAMS_NTR_MAGIC_LE)
            {
                moduleParams = (module_params_ntr_t*)((u8*)ptr + 8 - sizeof(module_params_ntr_t));
                break;
            }
        }
    }

    if (!moduleParams)
    {
        // as a last resort, scan the entire arm9 binary
        for (u32 i = 0; i < romHeader->arm9Size; i += 4)
        {
            u32* ptr = (u32*)(romHeader->arm9LoadAddress + i);
            if (ptr[0] == MODULE_PARAMS_NTR_MAGIC_BE && ptr[1] == MODULE_PARAMS_NTR_MAGIC_LE)
            {
                moduleParams = (module_params_ntr_t*)((u8*)ptr + 8 - sizeof(module_params_ntr_t));
                break;
            }
        }
    }

    return moduleParams;
}
