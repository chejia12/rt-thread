/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <rtconfig.h>
#include <board.h>

#ifndef SFUD_NOR_FLASH_ATTACH_SPI_BUS
#define FAL_USING_NOR_FLASH_DEV_NAME             "flash"
#endif
/* ===================== Flash device Configuration ========================= */
extern struct fal_flash_dev nor_flash0;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &nor_flash0,                                                     \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table 
这里将nor flash的块设备分区为2个，一个是cfg，一个是log，都是1M大小，

分区表格式：
    分区掩码----分区名称-----块设备名称----分区的开始地址---分区大小----0 结束标记-----
*/
#define FAL_PART_TABLE                                                               \
{                                                                                    \
    {FAL_PART_MAGIC_WORD, "cfg", FAL_USING_NOR_FLASH_DEV_NAME,            0,              1*1024*1024, 0}, \
    {FAL_PART_MAGIC_WORD,  "log", FAL_USING_NOR_FLASH_DEV_NAME,           1*1024*1024,    1*1024*1024, 0}, \
}
#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
