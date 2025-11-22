/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include <nds/ndstypes.h>
#include <nds/disc_io.h>
#include <string.h>
#include <libtwl/rtos/rtosIrq.h>
#include "core/Environment.h"
// #include <libtwl/rtos/rtosIrq.h>
// #include <libtwl/rtos/rtosEvent.h>
#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "dldi.h"
#include "../mmc/sdmmc.h"
// #include "core/semihosting.h"

/* Definitions of physical drive number for each drive */
#define DEV_FAT		0 //dldi
#define DEV_SD		1 //dsi sd
#define DEV_PC		2 //image on pc via semihosting
#define DEV_PC2		3 //image on pc via agb semihosting

// static int sPcFileHandle;
// static rtos_event_t sSemihostingCommandDoneEvent;

//extern "C" int sdmmc_sd_startup();
// extern FN_MEDIUM_STARTUP _DLDI_startup_ptr;
// extern FN_MEDIUM_READSECTORS _DLDI_readSectors_ptr;
// extern FN_MEDIUM_WRITESECTORS _DLDI_writeSectors_ptr;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

extern "C" DSTATUS disk_status (
    BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    return 0;
}

// static void gbaIrq(u32 mask)
// {
//     rtos_signalEvent(&sSemihostingCommandDoneEvent);
// }

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

extern "C" DSTATUS disk_initialize (
    BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
    if (pdrv == DEV_FAT)
    {
        return 0;
    }
    else if (Environment::IsDsiMode() && pdrv == DEV_SD)
    {
        pico_SDMMC_init(SDMMC_DEV_CARD);
        return 0;
    }
    else if (pdrv == DEV_PC)
    {
        // sPcFileHandle = sh_openFile("d:\\Emulators\\No$Debugger 3.0\\DSI-1.SD", SH_OPEN_MODE_R_PLUS_B);
        return 0;
    }
    else if (pdrv == DEV_PC2)
    {
        // rtos_createEvent(&sSemihostingCommandDoneEvent);
        // rtos_disableIrqMask(RTOS_IRQ_GBA_IREQ);
        // rtos_ackIrqMask(RTOS_IRQ_GBA_IREQ);
        // rtos_setIrqFunc(RTOS_IRQ_GBA_IREQ, gbaIrq);
        // rtos_enableIrqMask(RTOS_IRQ_GBA_IREQ);
        return 0;
    }

    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

extern "C" DRESULT disk_read (
    BYTE pdrv,		/* Physical drive nmuber to identify the drive */
    BYTE *buff,		/* Data buffer to store read data */
    DWORD sector,	/* Start sector in LBA */
    UINT count		/* Number of sectors to read */
)
{
    if (pdrv == DEV_FAT)
    {
        dldi_readSectors(buff, sector, count);
        return RES_OK;
    }
    else if (Environment::IsDsiMode() && pdrv == DEV_SD)
    {
        pico_SDMMC_readSectors(SDMMC_DEV_CARD, sector, buff, count);
        return RES_OK;
    }
    else if (pdrv == DEV_PC)
    {
        // sh_seekFile(sPcFileHandle, sector * 512);
        // sh_readFile(sPcFileHandle, buff, count * 512);
        // return RES_OK;
    }
    else if (pdrv == DEV_PC2)
    {
        // rtos_clearEvent(&sSemihostingCommandDoneEvent);
        u32 agbMem = *(u32*)0x027FFF7C;
        *(vu16*)(agbMem + 0x10002) = 1;
        *(vu32*)(agbMem + 0x10004) = sector;
        *(vu32*)(agbMem + 0x10008) = count;
        rtos_disableIrqMask(RTOS_IRQ_GBA_IREQ);
        rtos_ackIrqMask(RTOS_IRQ_GBA_IREQ);
        *(vu16*)(agbMem + 0x10000) = 0x55;
        while (!(rtos_getIrqFlags() & RTOS_IRQ_GBA_IREQ));
        // rtos_waitEvent(&sSemihostingCommandDoneEvent, false, true);
        if (*(vu16*)(agbMem + 0x10000) != 0xAA)
            return RES_ERROR;
        memcpy(buff, (const void*)(agbMem + 0x10020), count * 512);
        return RES_OK;
    }

    return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

extern "C" DRESULT disk_write (
    BYTE pdrv,			/* Physical drive nmuber to identify the drive */
    const BYTE *buff,	/* Data to be written */
    DWORD sector,		/* Start sector in LBA */
    UINT count			/* Number of sectors to write */
)
{
    if (pdrv == DEV_FAT)
    {
        dldi_writeSectors(buff, sector, count);
        return RES_OK;
    }
    else if (Environment::IsDsiMode() && pdrv == DEV_SD)
    {
        pico_SDMMC_writeSectors(SDMMC_DEV_CARD, sector, buff, count);
        return RES_OK;
    }
    else if (pdrv == DEV_PC)
    {
        // sh_seekFile(sPcFileHandle, sector * 512);
        // sh_writeFile(sPcFileHandle, buff, count * 512);
        // return RES_OK;
    }
    else if (pdrv == DEV_PC2)
    {
        // rtos_clearEvent(&sSemihostingCommandDoneEvent);
        u32 agbMem = *(u32*)0x027FFF7C;
        memcpy((void*)(agbMem + 0x10020), buff, count * 512);
        *(vu16*)(agbMem + 0x10002) = 2;
        *(vu32*)(agbMem + 0x10004) = sector;
        *(vu32*)(agbMem + 0x10008) = count;
        rtos_disableIrqMask(RTOS_IRQ_GBA_IREQ);
        rtos_ackIrqMask(RTOS_IRQ_GBA_IREQ);
        *(vu16*)(agbMem + 0x10000) = 0x55;
        while (!(rtos_getIrqFlags() & RTOS_IRQ_GBA_IREQ));
        // rtos_waitEvent(&sSemihostingCommandDoneEvent, false, true);
        if (*(vu16*)(agbMem + 0x10000) != 0xAA)
            return RES_ERROR;
        return RES_OK;
    }

    return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

extern "C" DRESULT disk_ioctl (
    BYTE pdrv,		/* Physical drive nmuber (0..) */
    BYTE cmd,		/* Control code */
    void *buff		/* Buffer to send/receive control data */
)
{
    return RES_OK;
}

