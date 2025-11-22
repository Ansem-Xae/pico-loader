#pragma once
#include <libtwl/ipc/ipcFifo.h>
#include "ipcCommands.h"

static inline void sendToArm9(u32 value)
{
    while (ipc_isSendFifoFull());
    ipc_sendWordDirect(value);
}

static inline u32 receiveFromArm9()
{
    while (ipc_isRecvFifoEmpty());
    return ipc_recvWordDirect();
}
