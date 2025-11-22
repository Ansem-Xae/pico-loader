#pragma once
#include "../Patch.h"

/// @brief Arm7 patch to get and update the arena low addresses for wram and main memory.
class OsGetInitArenaLoPatch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

    void* GetArm7PrivateWramArenaLo() const { return _wramBssEnd; }
    void SetArm7PrivateWramArenaLo(void* wramBssEnd) { _wramBssEnd = wramBssEnd; }
    void* GetMainMemoryArenaLo() const { return _mainMemoryArenaLo; }
    void SetMainMemoryArenaLo(void* mainMemoryArenaLo) { _mainMemoryArenaLo = mainMemoryArenaLo; }

private:
    u32* _osGetInitArenaLo = nullptr;
    u32 _wramBssOffset = 0;
    void* _wramBssEnd = nullptr;
    void* _mainMemoryArenaLo = nullptr;
};