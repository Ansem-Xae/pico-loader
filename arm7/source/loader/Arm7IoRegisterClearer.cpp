#include "common.h"
#include <libtwl/dma/dmaNitro.h>
#include <libtwl/dma/dmaTwl.h>
#include <libtwl/gfx/gfxStatus.h>
#include <libtwl/rtos/rtosIrq.h>
#include <libtwl/sio/sio.h>
#include <libtwl/sound/sound.h>
#include <libtwl/sound/soundCapture.h>
#include <libtwl/sound/soundChannel.h>
#include <libtwl/sys/sysPower.h>
#include <libtwl/timer/timer.h>
#include "SdmmcDefinitions.h"
#include "core/Environment.h"
#include "Arm7IoRegisterClearer.h"

void Arm7IoRegisterClearer::ClearIoRegisters() const
{
    REG_IME = 0;
    REG_IE = 0;
    if (Environment::IsDsiMode())
    {
        ClearTwlIoRegisters();
    }
    ClearNtrIoRegisters();
}

void Arm7IoRegisterClearer::ClearNtrIoRegisters() const
{
    REG_DISPSTAT = 0;
    REG_TM0CNT_H = 0; // timer 0
    REG_TM0CNT_L = 0;
    REG_TM1CNT_H = 0; // timer 1
    REG_TM1CNT_L = 0;
    REG_TM2CNT_H = 0; // timer 2
    REG_TM2CNT_L = 0;
    REG_TM3CNT_H = 0; // timer 3
    REG_TM3CNT_L = 0;
    REG_DMA0CNT = 0; // dma 0
    REG_DMA0SAD = 0;
    REG_DMA0DAD = 0;
    REG_DMA1CNT = 0; // dma 1
    REG_DMA1SAD = 0;
    REG_DMA1DAD = 0;
    REG_DMA2CNT = 0; // dma 2
    REG_DMA2SAD = 0;
    REG_DMA2DAD = 0;
    REG_DMA3CNT = 0; // dma 3
    REG_DMA3SAD = 0;
    REG_DMA3DAD = 0;
    REG_RCNT0_L = 0;

    for (int i = 0; i < 16; i++)
    {
        REG_SOUNDxCNT(i) = 0;
        REG_SOUNDxSAD(i) = 0;
        REG_SOUNDxTMR(i) = 0;
        REG_SOUNDxPNT(i) = 0;
        REG_SOUNDxLEN(i) = 0;
	}

    REG_SOUNDCNT = 0;
    REG_SNDCAP0CNT = 0;
    REG_SNDCAP1CNT = 0;

    REG_SNDCAP0DAD = 0;
    REG_SNDCAP0LEN = 0;
    REG_SNDCAP1DAD = 0;
    REG_SNDCAP1LEN = 0;

    sys_setWifiPower(false);
    sys_setSoundPower(true);
}

void Arm7IoRegisterClearer::ClearTwlIoRegisters() const
{
    REG_IE2 = 0;
    REG_NDMA0SAD = 0;
    REG_NDMA0DAD = 0;
    REG_NDMA0TCNT = 0;
    REG_NDMA0WCNT = 0;
    REG_NDMA0BCNT = 0;
    REG_NDMA0FDATA = 0;
    REG_NDMA0CNT = 0;
    REG_NDMA1SAD = 0;
    REG_NDMA1DAD = 0;
    REG_NDMA1TCNT = 0;
    REG_NDMA1WCNT = 0;
    REG_NDMA1BCNT = 0;
    REG_NDMA1FDATA = 0;
    REG_NDMA1CNT = 0;
    REG_NDMA2SAD = 0;
    REG_NDMA2DAD = 0;
    REG_NDMA2TCNT = 0;
    REG_NDMA2WCNT = 0;
    REG_NDMA2BCNT = 0;
    REG_NDMA2FDATA = 0;
    REG_NDMA2CNT = 0;
    REG_NDMA3SAD = 0;
    REG_NDMA3DAD = 0;
    REG_NDMA3TCNT = 0;
    REG_NDMA3WCNT = 0;
    REG_NDMA3BCNT = 0;
    REG_NDMA3FDATA = 0;
    REG_NDMA3CNT = 0;

    *(vu16*)(SDMMC_BASE + REG_SDDATACTL32) &= 0xF7FFu;
    *(vu16*)(SDMMC_BASE + REG_SDDATACTL32) &= 0xEFFFu;
    *(vu16*)(SDMMC_BASE + REG_SDDATACTL32) |= 0x402u;
    *(vu16*)(SDMMC_BASE + REG_SDDATACTL) = (*(vu16*)(SDMMC_BASE + REG_SDDATACTL) & 0xFFDD) | 2;
    *(vu16*)(SDMMC_BASE + REG_SDDATACTL32) &= 0xFFFFu;
    *(vu16*)(SDMMC_BASE + REG_SDDATACTL) &= 0xFFDFu;
    *(vu16*)(SDMMC_BASE + REG_SDBLKLEN32) = 512;
    *(vu16*)(SDMMC_BASE + REG_SDBLKCOUNT32) = 1;
    *(vu16*)(SDMMC_BASE + REG_SDRESET) &= 0xFFFEu;
    *(vu16*)(SDMMC_BASE + REG_SDRESET) |= 1u;
    *(vu16*)(SDMMC_BASE + REG_SDIRMASK0) |= TMIO_MASK_ALL;
    *(vu16*)(SDMMC_BASE + REG_SDIRMASK1) |= TMIO_MASK_ALL>>16;
    *(vu16*)(SDMMC_BASE + REG_SDSTATUS0) = 0;
    *(vu16*)(SDMMC_BASE + REG_SDSTATUS1) = 0;
    *(vu16*)(SDMMC_BASE + 0x0fc) |= 0xDBu; //SDCTL_RESERVED7
    *(vu16*)(SDMMC_BASE + 0x0fe) |= 0xDBu; //SDCTL_RESERVED8
    *(vu16*)(SDMMC_BASE + REG_SDPORTSEL) &= 0xFFFCu;
    *(vu16*)(SDMMC_BASE + REG_SDCLKCTL) = 0x20;
    *(vu16*)(SDMMC_BASE + REG_SDOPT) = 0x40EE;
    *(vu16*)(SDMMC_BASE + REG_SDPORTSEL) &= 0xFFFCu;
    *(vu16*)(SDMMC_BASE + REG_SDBLKLEN) = 512;
    *(vu16*)(SDMMC_BASE + REG_SDSTOP) = 0;

    *(vu16*)(0x04004A00 + REG_SDDATACTL32) &= 0xF7FFu;
    *(vu16*)(0x04004A00 + REG_SDDATACTL32) &= 0xEFFFu;
    *(vu16*)(0x04004A00 + REG_SDDATACTL32) |= 0x402u;
    *(vu16*)(0x04004A00 + REG_SDDATACTL) = (*(vu16*)(SDMMC_BASE + REG_SDDATACTL) & 0xFFDD) | 2;
    *(vu16*)(0x04004A00 + REG_SDDATACTL32) &= 0xFFFFu;
    *(vu16*)(0x04004A00 + REG_SDDATACTL) &= 0xFFDFu;
    *(vu16*)(0x04004A00 + REG_SDBLKLEN32) = 512;
    *(vu16*)(0x04004A00 + REG_SDBLKCOUNT32) = 1;
    *(vu16*)(0x04004A00 + REG_SDRESET) &= 0xFFFEu;
    *(vu16*)(0x04004A00 + REG_SDRESET) |= 1u;
    *(vu16*)(0x04004A00 + REG_SDIRMASK0) |= TMIO_MASK_ALL;
    *(vu16*)(0x04004A00 + REG_SDIRMASK1) |= TMIO_MASK_ALL>>16;
    *(vu16*)(0x04004A00 + REG_SDSTATUS0) = 0;
    *(vu16*)(0x04004A00 + REG_SDSTATUS1) = 0;
    *(vu16*)(0x04004A00 + 0x0fc) |= 0xDBu; //SDCTL_RESERVED7
    *(vu16*)(0x04004A00 + 0x0fe) |= 0xDBu; //SDCTL_RESERVED8
    *(vu16*)(0x04004A00 + REG_SDPORTSEL) &= 0xFFFCu;
    *(vu16*)(0x04004A00 + REG_SDCLKCTL) = 0x20;
    *(vu16*)(0x04004A00 + REG_SDOPT) = 0x40EE;
    *(vu16*)(0x04004A00 + REG_SDPORTSEL) &= 0xFFFCu;
    *(vu16*)(0x04004A00 + REG_SDBLKLEN) = 512;
    *(vu16*)(0x04004A00 + REG_SDSTOP) = 0;
}
