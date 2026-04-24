#ifndef MODBUS__REG_H
#define MODBUS__REG_H

#include <stdint.h>
typedef union
{
	uint16_t val_u16[2];
	uint32_t val_u32;
	float val_float;
} union_var;

/**
 * @brief 将浮点数转换成寄存器，这里默认小端模式
 *
 * @param val 浮点数
 * @param reg 寄存器
 */

 void float_to_reg(const float val, uint16_t *reg)
{
	union_var var;
	var.val_float = val;
	reg[0] = var.val_u16[1];
	reg[1] = var.val_u16[0];
}
/**
 * @brief 将寄存器转换成浮点数，这里默认小端模式
 * 
 * @param  reg              寄存器
 * @return float 
 */
 float reg_to_float(const uint16_t *reg)
{
	union_var var;
	var.val_u16[1] = reg[0];
	var.val_u16[0] = reg[1];
	return var.val_float;
}
/**
 * @brief 将无符号32位整数转换成寄存器，这里默认小端模式
 * 
 * @param val 无符号32位整数
 * @param reg 寄存器
 */
 void u32_to_reg(const uint32_t val, uint16_t *reg)
{
	union_var var;
	var.val_u32 = val;
	reg[0] = var.val_u16[1];
	reg[1] = var.val_u16[0];
}
/**
 * @brief 将寄存器转换成无符号32位整数，这里默认小端模式
 * 
 * @param reg 寄存器
 * @return uint32_t 无符号32位整数
 */
 uint32_t reg_to_u32(const uint16_t *reg)
{
	union_var var;
	var.val_u16[1] = reg[0];
	var.val_u16[0] = reg[1];
	return var.val_u32;
}
#endif