/*
 * bootloader_user.c
 *
 *  Created on: Jul 11, 2020
 *      Author: mehdi
 */
#include "fatfs.h"
#include "bootloader.h"
#include "bootloader_user.h"
/*** Bootloader ***************************************************************/
void Enter_Bootloader(void)
{
    FRESULT  fr;
    UINT     num;
    uint8_t  i;
    uint8_t  status;
    uint64_t data;
    uint32_t cntr;
    uint32_t addr;
    char     msg[40] = {0x00};

    /* Check for flash write protection */
    if(Bootloader_GetProtectionStatus() & BL_PROTECTION_WRP)
    {
        printf("Application space in flash is write protected.\n\r");
        printf("Press button to disable flash write protection...\n\r");
        LED_R_ON();
        for(i = 0; i < 100; ++i)
        {
            LED_Y_TG();
            HAL_Delay(50);
            if(IS_BTN_PRESSED())
            {
                printf("Disabling write protection and generating system reset...\n\r");
                Bootloader_ConfigProtection(BL_PROTECTION_NONE);
            }
        }
        LED_R_OFF();
        LED_Y_OFF();
        printf("Button was not pressed, write protection is still active.\n\r");
        printf("Exiting Bootloader.\n\r");
        return;
    }

    /* Initialize SD card */
    if(SD_Init())
    {
        /* SD init failed */
        printf("SD card cannot be initialized.\n\r");
        return;
    }

    /* Mount SD card */
    fr = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
    if(fr != FR_OK)
    {
        /* f_mount failed */
        printf("SD card cannot be mounted.\n\r");
        sprintf(msg, "FatFs error code: %u\n\r", fr);
        printf(msg);
        return;
    }
    printf("SD mounted.\n\r");

    /* Open file for programming */
    fr = f_open(&SDFile, CONF_FILENAME, FA_READ);
    if(fr != FR_OK)
    {
        /* f_open failed */
        printf("File cannot be opened.\n\r");
        sprintf(msg, "FatFs error code: %u\n\r", fr);
        printf(msg);

        SD_Eject();
        printf("SD ejected.\n\r");
        return;
    }
    printf("Software found on SD.\n\r");

    /* Check size of application found on SD card */
    if(Bootloader_CheckSize(f_size(&SDFile)) != BL_OK)
    {
        printf("Error: app on SD card is too large.\n\r");

        f_close(&SDFile);
        SD_Eject();
        printf("SD ejected.\n\r");
        return;
    }
    printf("App size OK.\n\r");

    /* Step 1: Init Bootloader and Flash */
    Bootloader_Init();

    /* Step 2: Erase Flash */
    printf("Erasing flash...\n\r");
    LED_Y_ON();
    Bootloader_Erase();
    LED_Y_OFF();
    printf("Flash erase finished.\n\r");

    /* If BTN is pressed, then skip programming */
    if(IS_BTN_PRESSED())
    {
        printf("Programming skipped.\n\r");

        f_close(&SDFile);
        SD_Eject();
        printf("SD ejected.\n\r");
        return;
    }

    /* Step 3: Programming */
    printf("Starting programming...\n\r");
    LED_Y_ON();
    cntr = 0;
    Bootloader_FlashBegin();
    do
    {
        data = 0xFFFFFFFFFFFFFFFF;
        fr   = f_read(&SDFile, &data, 8, &num);
        if(num)
        {
            status = Bootloader_FlashNext(data);
            if(status == BL_OK)
            {
                cntr++;
            }
            else
            {
                sprintf(msg, "Programming error at: %lu byte\n\r", (cntr * 8));
                printf(msg);

                f_close(&SDFile);
                SD_Eject();
                printf("SD ejected.\n\r");

                LED_G_OFF();
                LED_Y_OFF();
                return;
            }
        }
        if(cntr % 256 == 0)
        {
            /* Toggle green LED during programming */
            LED_G_TG();
        }
    } while((fr == FR_OK) && (num > 0));

    /* Step 4: Finalize Programming */
    Bootloader_FlashEnd();
    f_close(&SDFile);
    LED_G_OFF();
    LED_Y_OFF();
    printf("Programming finished.\n\r");
    sprintf(msg, "Flashed: %lu bytes.", (cntr * 8));
    printf(msg);

    /* Open file for verification */
    fr = f_open(&SDFile, CONF_FILENAME, FA_READ);
    if(fr != FR_OK)
    {
        /* f_open failed */
        printf("File cannot be opened.\n\r");
        sprintf(msg, "FatFs error code: %u\n\r", fr);
        printf(msg);

        SD_Eject();
        printf("SD ejected.\n\r");
        return;
    }

    /* Step 5: Verify Flash Content */
    addr = APP_ADDRESS;
    cntr = 0;
    do
    {
        data = 0xFFFFFFFFFFFFFFFF;
        fr   = f_read(&SDFile, &data, 4, &num);
        if(num)
        {
            if(*(uint32_t*)addr == (uint32_t)data)
            {
                addr += 4;
                cntr++;
            }
            else
            {
                sprintf(msg, "Verification error at: %lu byte.\n\r", (cntr * 4));
                printf(msg);

                f_close(&SDFile);
                SD_Eject();
                printf("SD ejected.\n\r");

                LED_G_OFF();
                return;
            }
        }
        if(cntr % 256 == 0)
        {
            /* Toggle green LED during verification */
            LED_G_TG();
        }
    } while((fr == FR_OK) && (num > 0));
    printf("Verification passed.\n\r");
    LED_G_OFF();

    /* Eject SD card */
    SD_Eject();
    printf("SD ejected.\n\r");

    /* Enable flash write protection */
#if(USE_WRITE_PROTECTION)
    printf("Enablig flash write protection and generating system reset...\n\r");
    if(Bootloader_ConfigProtection(BL_PROTECTION_WRP) != BL_OK)
    {
        printf("Failed to enable write protection.\n\r");
        printf("Exiting Bootloader.\n\r");
    }
#endif
}

/*** SD Card ******************************************************************/
uint8_t SD_Init(void)
{
  //  SDCARD_ON();

//    if(FATFS_Init())
//    {
//        /* Error */
//        return 1;
//    }

    if(BSP_SD_Init())
    {
        /* Error */
        return 1;
    }

    return 0;
}

void SD_DeInit(void)
{
	BSP_SD_Init();
//    FATFS_DeInit();
    //SDCARD_OFF();
}

void SD_Eject(void)
{
    f_mount(NULL, (TCHAR const*)SDPath, 0);
}
