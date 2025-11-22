#pragma once
#include "ndsHeader.h"
#include "moduleParams.h"

/// @brief Class for finding the module params of a retail arm9.
class ModuleParamsLocator
{
public:
    /// @brief Searches for the module params of the arm9 of the rom with the given \p romHeader.
    /// @param romHeader The header of the rom.
    /// @return A pointer to the found module params, or \c nullptr if the module params could not be found.
    module_params_ntr_t* FindModuleParams(const nds_header_ntr_t* romHeader);
};
