
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief 向指定的缓存空间添加字节数组小端序
 * @param  cmds             指令缓存空间
 * @param  cmd_len          当前缓存空间已有数据长度
 * @param  data              的数据
 * @return int 
 */
 int lsb_push_byte_array(uint8_t *cmds, int cmd_len,const uint8_t *data, uint8_t len)
{
    memcpy(cmds + cmd_len, data, len);
    cmd_len += len;
    return cmd_len;
}

/**
 * @brief 向指定的缓存空间添加float，小端序
 * @param  cmds             指令缓存空间
 * @param  cmd_len          当前缓存空间已有数据长度
 * @param  val              4字节的数据
 * @return int 
 */
 int lsb_push_float_data(uint8_t *cmds, int cmd_len,float val)
{
    memcpy(cmds + cmd_len, &val, sizeof(float));
    cmd_len += sizeof(float);
    return cmd_len;
}

/**
 * @brief 向指定的缓存空间添加uint32_t，小端序
 * @param  cmds             指令缓存空间
 * @param  cmd_len          当前缓存空间已有数据长度
 * @param  ptr              4字节的数据
 * @return int 
 */
 int lsb_push_uint32_data(uint8_t *cmds, int cmd_len,uint32_t u32)
{
    memcpy(cmds + cmd_len, &u32, sizeof(uint32_t));
    cmd_len += sizeof(uint32_t);
    return cmd_len;
}
/**
 * @brief 向指定的缓存空间添加uint16_t，小端序
 * @param  cmds             指令缓存空间
 * @param  cmd_len          当前缓存空间已有数据长度
 * @param  ptr              2字节的数据一般是uint16_t
 * @return int 
 */
 int lsb_push_uint16_data(uint8_t *cmds, int cmd_len, uint16_t u16val)
{
    memcpy(cmds + cmd_len, &u16val, sizeof(uint16_t));
    cmd_len += sizeof(uint16_t);
    return cmd_len;
}
/**
 * @brief 向指定的缓存空间添加uint8_t，小端序
 * @param  cmds             指令缓存空间
 * @param  cmd_len          当前缓存空间已有数据长度
 * @param  ptr              1字节的数据，一般是uint8_t
 * @return int 
 */
 int lsb_push_uint8_data(uint8_t *cmds, int cmd_len, uint8_t u8val)
{
    cmds[cmd_len] = u8val;
    cmd_len += sizeof(uint8_t);
    return cmd_len;
}

