#ifndef __LIB_CRC_XOR__
#define	__LIB_CRC_XOR__
#include <stdint.h>
 
/*
	异或检验函数：
    4.数据检验采用异或校验方式，检验范围是（数据头，地址，数据长度，指令，数据）
*/
uint8_t xor_check(const uint8_t *data,const uint16_t data_len);
/**
 * @brief MODBUS CRC16校验函数
 * 
 * @param  buffer           
 * @param  length           
 * @return uint16_t 
 */
uint16_t modbus_crc16(const uint8_t *buffer, const int length);

/*
    crc8校验函数
*/
unsigned char crc8_maxim(unsigned char *puchMsg, unsigned int usDataLen);

/**
 * @brief CCITT_CRC16校验函数
 *
 * @param  buf
 * @param  len
 * @return unsigned short
 */
unsigned short CCITT_CRC16(const unsigned char *buf, const unsigned int len);
#endif /* __LIB_CRC_XOR__ */
