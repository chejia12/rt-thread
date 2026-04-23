#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <ulog.h>

#define MCP4728_ADDR 0x60    // 设备地址 A0~A2 接地
#define MCP4728_REF_VAL 3.3f // 芯片参考电压

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
 * @param  val               12位 DAC 值（0~4095）
 * @return rt_err_t
 */
rt_err_t mcp4728_write_channel(const char *iic_bus_name, const uint8_t channel, const float val)

{
    RT_ASSERT(iic_bus_name != RT_NULL);
    RT_ASSERT(channel <= 3);
    RT_ASSERT(val >= 0 && val <= MCP4728_REF_VAL);
    rt_uint8_t data[3];

    struct rt_i2c_bus_device *i2c_bus = RT_NULL;
    i2c_bus = rt_i2c_bus_device_find(iic_bus_name);
    if (!i2c_bus)
    {
        log_d("Error: I2C bus %s not found.", iic_bus_name);
        return -RT_ERROR;
    }

    uint16_t dac_value = voltage_to_dac_value(val);


    // 构造命令字节（单通道写入命令）
    data[0] = 0x50 | (channel << 1);
    data[1] = (dac_value >> 8) & 0xFF;
    data[2] = dac_value & 0xFF;

    struct rt_i2c_msg msg;
    msg.addr = MCP4728_ADDR;
    msg.flags = RT_I2C_WR;
    msg.buf = data;
    msg.len = 3;

    if (rt_i2c_transfer(i2c_bus, &msg, 1) == 1)
        return RT_EOK;
    else
        return -RT_ERROR;
}
static int init_mcp4728(void)
{
    mcp4728_write_channel("i2c1", 0, 0);
    mcp4728_write_channel("i2c1", 1, 0);
    mcp4728_write_channel("i2c1", 2, 0);
    mcp4728_write_channel("i2c1", 3, 0);

    mcp4728_write_channel("i2c2", 0, 0);
    mcp4728_write_channel("i2c2", 1, 0);
    mcp4728_write_channel("i2c2", 2, 0);

	
	log_d("init_mcp4728 ok");
	
	 return RT_EOK;
}
// INIT_APP_EXPORT(init_mcp4728); // 上电自动初始化

static void dac_mcp4728_test(int argc, char **argv)
{
    if(argc!=2)
    {
        log_d("Usage: dac_mcp4728_init <iic_bus_name> ");
        return;
    }

    mcp4728_write_channel(argv[1], 0, 1);
    mcp4728_write_channel(argv[1], 1, 1.5);
    mcp4728_write_channel(argv[1], 2, 2);
    mcp4728_write_channel(argv[1], 3, 3);

	
	log_d("dac_mcp4728_test ok");
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(dac_mcp4728_test, i2c dac_mcp4728_init sample);
