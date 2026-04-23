#ifndef __TREATY_BASE__
#define __TREATY_BASE__
#include <stdint.h>

/**
 * @brief 向指定的缓存空间添加float，小端序
 * @param  cmds             指令缓存空间
 * @param  cmd_len          当前缓存空间已有数据长度
 * @param  val              4字节的数据
 * @return int 
 */
 int lsb_push_float_data(uint8_t *cmds, int cmd_len,float val);

/**
 * @brief 向指定的缓存空间添加uint32_t，小端序
 * @param  cmds             指令缓存空间
 * @param  cmd_len          当前缓存空间已有数据长度
 * @param  ptr              4字节的数据
 * @return int 
 */
 int lsb_push_uint32_data(uint8_t *cmds, int cmd_len,uint32_t u32);
/**
 * @brief 向指定的缓存空间添加uint16_t，小端序
 * @param  cmds             指令缓存空间
 * @param  cmd_len          当前缓存空间已有数据长度
 * @param  ptr              2字节的数据一般是uint16_t
 * @return int 
 */
 int lsb_push_uint16_data(uint8_t *cmds, int cmd_len, uint16_t u16val);
/**
 * @brief 向指定的缓存空间添加uint8_t，小端序
 * @param  cmds             指令缓存空间
 * @param  cmd_len          当前缓存空间已有数据长度
 * @param  ptr              1字节的数据，一般是uint8_t
 * @return int 
 */
 int lsb_push_uint8_data(uint8_t *cmds, int cmd_len, uint8_t u8val);

/*
	异或检验函数：
	校验数据的范围时数据头开始到整个数据帧长度的倒数第二个字节（即包括检验位置和数据尾）
*/
uint8_t xor_check(const uint8_t *data,const uint16_t data_len);

/**
 * @brief
 *
 * @param  cmds         存放新建命令数据的内存开始地址
 * @param  cmds_size        存放新建命令数据的内存大小
 * @return int 返回数据头长度
 */
int treaty_create_header(uint8_t *cmds, const int cmds_size, const uint8_t addr, const uint8_t cmd);

/**
 * @brief
 *
 * @param  cmds         存放新建命令数据的内存开始地址
 * @param  cmds_len        cmds内部数据大小长度
 * @return int 返回数据总长度
 */
int treaty_create_check_tail(uint8_t *cmds, int cmds_len);


/**
 * @brief 向指定的缓存空间添加字节数组小端序
 * @param  cmds             指令缓存空间
 * @param  cmd_len          当前缓存空间已有数据长度
 * @param  data              的数据
 * @return int 
 */
 int lsb_push_byte_array(uint8_t *cmds, int cmd_len,const uint8_t *data, uint8_t len);
 #endif
