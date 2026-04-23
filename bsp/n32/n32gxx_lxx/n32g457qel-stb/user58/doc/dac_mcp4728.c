#include <rtthread.h>

#ifdef BRD_DAC_MCP4728

#define DBG_TAG "MCP4728"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include <rtdevice.h>
#include <board.h>
#include <ulog.h>

#define MCP4728_ADDR 0x60	 // 设备地址 A0~A2 接地
#define MCP4728_REF_VAL 3.3f // 芯片参考电压
typedef struct
{
	struct rt_i2c_bus_device *i2c_bus_dev;
	struct rt_dac_device dac;
	char *iic_bus_name;
} mcp4728_dac_t;
/**
 * @brief 将指定电压转换为 12 位 DAC 值
 */
static uint16_t voltage_to_dac_value(float voltage)
{
	return (uint16_t)(voltage / MCP4728_REF_VAL * 4095);
}

/**
 * @brief 向 MCP4728 指定通道写入 DAC 值
 *
 * @param  iic_bus_name      IIC 总线名称
 * @param  channel           通道号 0~3（对应 A~D）
 * @param  val               输出电压值 0-3.3
 * @return rt_err_t
 */
rt_err_t mcp4728_write_channel(const struct rt_i2c_bus_device *i2c_bus, const uint8_t channel, const uint16_t val)

{
	RT_ASSERT(i2c_bus);
	RT_ASSERT(channel <= 3);
	rt_uint8_t data[3];
	// 构造命令字节（单通道写入命令）
	data[0] = 0x50 | (channel << 1);
	data[1] = (val >> 8) & 0xFF;
	data[2] = val & 0xFF;

	struct rt_i2c_msg msg;
	msg.addr = MCP4728_ADDR;
	msg.flags = RT_I2C_WR;
	msg.buf = data;
	msg.len = 3;
	LOG_D("WRITE CH=%d val=%d", channel, val);
	if (rt_i2c_transfer(i2c_bus, &msg, 1) == 1)
	{
		return RT_EOK;
	}
	else
	{
		LOG_W("WRITE DAC FAIL");
		return -RT_ERROR;
	}
}
static rt_err_t mcp4728_dac_enabled(struct rt_dac_device *device, rt_uint32_t channel)
{
	return RT_EOK;
}
/**
 * @brief dac 失能
 *
 * @param  device           dac 设备句柄
 * @param  channel          dac 通道
 * @return rt_err_t
 */
static rt_err_t mcp4728_dac_disabled(struct rt_dac_device *device, rt_uint32_t channel)
{
	mcp4728_dac_t *mcp4728_dac = device->parent.user_data;
	RT_ASSERT(mcp4728_dac->i2c_bus_dev);
	mcp4728_write_channel(mcp4728_dac->i2c_bus_dev, channel, 0);
	return RT_EOK;
}
/**
 * @brief dac 设置值
 *
 * @param  device           dac 设备句柄
 * @param  channel          dac 通道
 * @param  value            设置值
 * @return rt_err_t
 */
static rt_err_t mcp4728_set_dac_value(struct rt_dac_device *device, rt_uint32_t channel, rt_uint32_t *value)
{
	mcp4728_dac_t *mcp4728_dac = device->parent.user_data;
	RT_ASSERT(mcp4728_dac->i2c_bus_dev);
	mcp4728_write_channel(mcp4728_dac->i2c_bus_dev, channel, *value & 0XFFFF);
	return RT_EOK;
}
/**
 * @brief 返回dac分辨率位数
 *
 * @param  device
 * @return rt_uint8_t
 */
static rt_uint8_t mcp4728_dac_get_resolution(struct rt_dac_device *device)
{
	return 12;
}
static const struct rt_dac_ops mcp4728_dac_ops =
	{
		.disabled = mcp4728_dac_disabled,
		.enabled = mcp4728_dac_enabled,
		.convert = mcp4728_set_dac_value,
		.get_resolution = mcp4728_dac_get_resolution,
};

mcp4728_dac_t mcp4728_dac = {
	.dac.ops = &mcp4728_dac_ops,
	.iic_bus_name = BRD_DAC_MCP4728_I2C_BUS,
	.i2c_bus_dev = RT_NULL,
};

/*初始化mcp4728的GPIO引脚*/
int mcp4728_Init(void)
{
	mcp4728_dac.i2c_bus_dev = rt_i2c_bus_device_find(mcp4728_dac.iic_bus_name);
	if (!mcp4728_dac.i2c_bus_dev)
	{
		LOG_W("Error: I2C bus %s not found.", mcp4728_dac.iic_bus_name);
		return -RT_ERROR;
	}
	if (RT_EOK == rt_hw_dac_register(&mcp4728_dac.dac, "mcp4728", &mcp4728_dac_ops, &mcp4728_dac))
	{
		LOG_D("register dac device mcp4728 is ok ");
	}
	else
	{
		LOG_D("register dac device mcp4728 is fial ");
	}

	return 0;
}

INIT_COMPONENT_EXPORT(mcp4728_Init);

// static void dac_mcp4728_test(int argc, char **argv)
// {
//     if(argc!=2)
//     {
//         log_d("Usage: dac_mcp4728_init <iic_bus_name> ");
//         return;
//     }

//     mcp4728_write_channel(argv[1], 0, 1);
//     mcp4728_write_channel(argv[1], 1, 1.5);
//     mcp4728_write_channel(argv[1], 2, 2);
//     mcp4728_write_channel(argv[1], 3, 3);

// 	log_d("dac_mcp4728_test ok");
// }
// /* 导出到 msh 命令列表中 */
// MSH_CMD_EXPORT(dac_mcp4728_test, i2c dac_mcp4728_init sample);
#endif /*BRD_DAC_MCP4728*/