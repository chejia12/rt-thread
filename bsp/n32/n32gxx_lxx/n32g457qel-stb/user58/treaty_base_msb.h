#ifndef __TREATY_BASE_H__
#define __TREATY_BASE_H__

#include <stdint.h>

/**
 * @brief 向数组中写入 uint8_t 类型数据
 * @param vec 存储数据的数组
 * @param data 要写入的数据
 * @return uint32_t 写入数据的长度
 */
uint32_t msb_push_u8_to_array(uint8_t *buff, const uint32_t len, const uint8_t data);
/**
 * @brief 向数组中写入 uint16_t 类型数据
 * @param vec 存储数据的数组
 * @param data 要写入的数据
 * @return uint32_t 写入数据的长度
 */
uint32_t msb_push_u16_to_array(uint8_t *buff, const uint32_t len, const uint16_t data);
/**
 * @brief 向数组中写入 uint32_t 类型数据
 * @param vec 存储数据的数组
 * @param data 要写入的数据
 * @return uint32_t 写入数据的长度
 */
uint32_t msb_push_u32_to_array(uint8_t *buff, const uint32_t len, const uint32_t data);
/**
 * @brief 向数组中写入 float 类型数据
 * @param vec 存储数据的数组
 * @param data 要写入的数据
 * @return uint32_t 写入数据长度
 */
uint32_t msb_push_float_to_array(uint8_t *buff, const uint32_t len, const float data);
/**
 * @brief 向数组中写入数组类型数据
 * @param vec 存储数据的数组
 * @param data 要写入的数据
 * @param len 要写入的数据长度
 * @return uint32_t 写入数据长度
 */
uint32_t msb_push_array_to_array(uint8_t *buff, const uint32_t old_len, const uint8_t *data, uint16_t len);


#endif
