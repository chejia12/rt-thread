#include "treaty_base_msb.h"

uint32_t msb_push_u8_to_array(uint8_t *buff,const uint32_t len, const uint8_t data)
{
    int new_len = len;
    buff[new_len++]=data;
    return new_len;
}
uint32_t msb_push_u16_to_array(uint8_t *buff,const uint32_t len, const uint16_t data)
{
    int new_len = len;
    buff[new_len++]=(data >> 8);
    buff[new_len++]=(data & 0xFF);
    return new_len;
}


uint32_t msb_push_u32_to_array(uint8_t *buff,const uint32_t len, const uint32_t data)
{
    int new_len = len;
    buff[new_len++]=(data >> 24);
    buff[new_len++]=(data >> 16);
    buff[new_len++]=(data >> 8);
    buff[new_len++]=(data & 0xFF);
    return new_len;
}
uint32_t msb_push_float_to_array(uint8_t *buff,const uint32_t len, const float data)
{
    int new_len = len;
    uint8_t *p = (uint8_t *)&data;
    buff[new_len++]=(p[3]);
    buff[new_len++]=(p[2]);
    buff[new_len++]=(p[1]);
    buff[new_len++]=(p[0]);
    return new_len;
}
uint32_t msb_push_array_to_array(uint8_t *buff,const uint32_t old_len, const uint8_t *data, uint16_t len)
{
    int new_len = old_len;
    for (uint16_t i = 0; i < len; i++)
    {
        buff[new_len++]=(data[i]);
    }
    return new_len;
}
