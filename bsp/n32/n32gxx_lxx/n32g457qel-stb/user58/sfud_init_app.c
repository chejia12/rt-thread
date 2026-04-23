/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-31     BruceOu      first implementation
 * 2023-06-03     CX           fixed sf probe error bug

 Kconfig配置项 
     config BRD_DFS_NOR_FLASH
        bool "Enable IN NOR FLASH MOUNT DFS elm or lfs"
        default n
        if BRD_DFS_NOR_FLASH
            config SFUD_NOR_FLASH_ATTACH_SPI_BUS
                string "select spi  bus of attach nor flash"
                default "spi1"
			config SPI_CS_NO_FLASH
				int "select spi cs pin"
				range 0 112
				default 4
            config SFUD_NOR_FLASH_NAME
                string "set nor flash device name  dev type = SPI Device "
                default "spi11"
			
        endif


 */
#include <rtthread.h>

#define DBG_TAG "SIMPLE"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


#include <rtdevice.h>
#include <board.h>
#include "drv_spi.h"

#ifdef RT_USING_SFUD

#include <rthw.h>
#include <finsh.h>

#ifndef SFUD_NOR_FLASH_NAME
#define SFUD_NOR_FLASH_NAME "spi11"
#endif

#ifndef SFUD_NOR_FLASH_ATTACH_SPI_BUS
#define SFUD_NOR_FLASH_ATTACH_SPI_BUS "spi1"
#endif

#ifndef FAL_USING_NOR_FLASH_DEV_NAME
#define FAL_USING_NOR_FLASH_DEV_NAME "flash"
#endif

#ifndef SPI_CS_NO_FLASH
#define SPI_CS_NO_FLASH GET_PIN(A, 4)
#endif
/**
 * @brief 挂载/注册spi设备
 *
 */
static void register_spi_dev_of_norflash(void)
{
	rt_err_t res;
	static struct n32_spi_cs spi_cs;
	static struct rt_spi_device spi_dev_gd25q; /* SPI device */
	spi_cs.GPIO_Pin = SPI_CS_NO_FLASH;
	rt_pin_mode(SPI_CS_NO_FLASH, PIN_MODE_OUTPUT);
	rt_pin_write(SPI_CS_NO_FLASH, PIN_HIGH);
	/*注册一个spi设备，使用老版本的接口绑定设备*/ 
	/*res = rt_spi_bus_attach_device(&spi_dev_gd25q, SPI_DEVICE_NAME, SPI_BUS_NAME, (void *)&spi_cs);*/ 
	/**在RTT5.2.1做了修改，之前使用 rt_spi_bus_attach_device*/
	res = rt_spi_bus_attach_device_cspin(&spi_dev_gd25q, SFUD_NOR_FLASH_NAME, SFUD_NOR_FLASH_ATTACH_SPI_BUS, SPI_CS_NO_FLASH, (void *)&spi_cs);
	if (res != RT_EOK)
	{
		LOG_W("rt_spi_bus_attach_device() run failed!\n");
		return ;
	}
}
static int rt_hw_spi_flash_init(void)
{
	/*挂载/注册spi设备*/ 
	register_spi_dev_of_norflash();
	/*注册一个块设备*/ 
	if (RT_NULL == rt_sfud_flash_probe(FAL_USING_NOR_FLASH_DEV_NAME, SFUD_NOR_FLASH_NAME))
	{
		return -RT_ERROR;
	}

#ifdef RT_USING_FAL
#include <fal.h>
	fal_init();
	// fal_blk_device_create("log"); /*挂载一个log块设备*/ 

	/*注册一个mtd设备，因为littlefs需要在mtd设备运行*/
	fal_mtd_nor_device_create("log");
#endif
	return RT_EOK;
}
INIT_PREV_EXPORT(rt_hw_spi_flash_init);
#endif

#if defined(RT_USING_DFS) && defined(FAL_USING_NOR_FLASH_DEV_NAME)
#include <dfs_fs.h>

#ifdef PKG_USING_LITTLEFS
#define FS_TYLE "lfs"/*文件系统类型 littlefs:ls；Fat32:elm*/
#define MTD_DEV "log" 
#else
#define FS_TYLE "elm"/*文件系统类型 littlefs:ls；Fat32:elm*/
#define MTD_DEV FAL_USING_NOR_FLASH_DEV_NAME
#endif
int mnt_init(void)
{

	// 挂载文件系统
	int res = dfs_mount(MTD_DEV, "/", FS_TYLE, 0, 0);
	if (res != 0)
	{
		if (0 == dfs_mkfs(FS_TYLE, MTD_DEV))
		{
			LOG_W("dfs_mkfs %s ok: %d\n", MTD_DEV, res);
			if (dfs_mount(MTD_DEV, "/", FS_TYLE, 0, 0))
			{
				LOG_D("Mount %s successfully: %d", MTD_DEV, res);
			}
			else
			{
				LOG_W("Mount %s fail: %d\n", MTD_DEV, res);
			}
		}
		else
		{
			LOG_W("dfs_mkfs %s fail: %d", MTD_DEV, res);
		}
	}
	else
	{
		LOG_D("Mounted %s to / successfully", MTD_DEV);
	}

	return 0;
}
INIT_COMPONENT_EXPORT(mnt_init);

/**
 * @brief 查询历史报警数据
 */
int dfs_formart(void)
{

	if (0 == dfs_mkfs(FS_TYLE, MTD_DEV))
	{
		if (dfs_mount(MTD_DEV, "/", FS_TYLE, 0, 0))
		{
			LOG_D("Mount %s successfully", MTD_DEV);
		}
		else
		{
			LOG_W("Mount %s fail", MTD_DEV);
		}
	}
	else
	{
		LOG_W("dfs_mkfs %s fail", MTD_DEV);
	}
	return 0;
}
/* 导出到MSH命令列表 */
MSH_CMD_EXPORT(dfs_formart, dfs_formart);

#endif