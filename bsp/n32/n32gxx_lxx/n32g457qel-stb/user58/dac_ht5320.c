
#include <rtthread.h>
#ifdef BRD_DAC_HT5320

#define DBG_TAG "HT5320"
#define DBG_LVL DBG_WARNING
#include <rtdbg.h>

#include <rtdevice.h>
#include <board.h>
#include "drv_spi.h"

typedef struct
{
	uint32_t cs_pin;			  /*cs pin*/
	struct rt_spi_device spi_dev; /*spi dev*/
	struct n32_spi_cs spi_cs;/*n32 独有的cs pin管理方法*/
	char name[32];
} ht5320_t;

static ht5320_t ht5320[] = {
#ifdef BRD_DAC_HT5320_CH0
	{.cs_pin = BRD_DAC_HT5320_CH0},
#endif /*BRD_DAC_HT5320_CH0*/

#ifdef BRD_DAC_HT5320_CH1
	{.cs_pin = BRD_DAC_HT5320_CH1},
#endif /*BRD_DAC_HT5320_CH1*/

#ifdef BRD_DAC_HT5320_CH2
	{.cs_pin = BRD_DAC_HT5320_CH2},
#endif /*BRD_DAC_HT5320_CH2*/

#ifdef BRD_DAC_HT5320_CH3
	{.cs_pin = BRD_DAC_HT5320_CH3}
#endif /*BRD_DAC_HT5320_CH3*/
};

/**
 * @brief ht5320 数据发送
 *
 * @param  dev_name         ht5320 d设备名称
 * @param  data             待写入数据
 */
static void ht5320_write(char *dev_name, uint16_t data)
{
	struct rt_spi_device *spi_dev_ht5320;
	/* 查找 spi 设备获取设备句柄 */
	spi_dev_ht5320 = (struct rt_spi_device *)rt_device_find(dev_name);
	if (!spi_dev_ht5320)
	{
		LOG_W("spi sample run failed! can't find %s device!", dev_name);
	}
	else
	{
		// 配置spi
		struct rt_spi_configuration cfg = {
			.mode = RT_SPI_MSB | RT_SPI_MASTER | RT_SPI_MODE_0,
			.data_width = 8,
			.max_hz = 1000 * 1000};
		rt_spi_configure(spi_dev_ht5320, &cfg);
		// 构造数据
		struct rt_spi_message msg1;
		uint8_t send_data[2] = {data >> 8, data & 0xff};
		msg1.send_buf = send_data;
		msg1.recv_buf = RT_NULL;
		msg1.length = 2;
		msg1.cs_take = 1;
		msg1.cs_release = 1;
		msg1.next = RT_NULL;
		rt_spi_transfer_message(spi_dev_ht5320, &msg1);
		LOG_D("%s Write data: %04x",dev_name,data);
	}
}
static rt_err_t ht5320_dac_enabled(struct rt_dac_device *device, rt_uint32_t channel)
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
static rt_err_t ht5320_dac_disabled(struct rt_dac_device *device, rt_uint32_t channel)
{
	ht5320_t *ht5320ptr=(ht5320_t*)device->parent.user_data;
	RT_ASSERT(channel<sizeof(ht5320)/sizeof(ht5320_t));
	ht5320_write(ht5320ptr[channel].name, 0);
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
static rt_err_t ht5320_set_dac_value(struct rt_dac_device *device, rt_uint32_t channel, rt_uint32_t *value)
{
	ht5320_t *ht5320ptr=(ht5320_t*)device->parent.user_data;
	RT_ASSERT(channel<sizeof(ht5320)/sizeof(ht5320_t));
	ht5320_write(ht5320ptr[channel].name, (*value)&0xffff);
	return RT_EOK;
}
/**
 * @brief 返回dac分辨率位数
 *
 * @param  device
 * @return rt_uint8_t
 */
static rt_uint8_t ht5320_dac_get_resolution(struct rt_dac_device *device)
{
	return 12;
}
static const struct rt_dac_ops ht5320_dac_ops =
	{
		.disabled = ht5320_dac_disabled,
		.enabled = ht5320_dac_enabled,
		.convert = ht5320_set_dac_value,
		.get_resolution = ht5320_dac_get_resolution,
};

static struct rt_dac_device ht5320_dac = {
	.ops = &ht5320_dac_ops,
};

/*初始化HT5320的GPIO引脚*/
int HT5320_Init(void)
{
	int num = sizeof(ht5320) / sizeof(ht5320_t);
	for (int i = 0; i < num; i++)
	{
		rt_pin_mode(ht5320[i].cs_pin, PIN_MODE_OUTPUT);
		ht5320[i].spi_cs.GPIO_Pin = ht5320[i].cs_pin;
		// 注册一个spi设备，使用老版本的接口绑定设备
		rt_memset(ht5320[i].name, 0, 32);
		rt_snprintf(ht5320[i].name, 32, "%s%d", BRD_DAC_HT5320_SPI_BUS, i);
		rt_err_t res = rt_spi_bus_attach_device(&ht5320[i].spi_dev,
												ht5320[i].name,
												BRD_DAC_HT5320_SPI_BUS,
												(void *)&ht5320[i].spi_cs);
		if (res != RT_EOK)
		{
			LOG_W("spi device %s register  failed!", ht5320[i].name);
		}
		else
		{
			LOG_D("spi device %s register  ok! ", ht5320[i].name);
		}
	}
	rt_hw_dac_register(&ht5320_dac, "dac", &ht5320_dac_ops, ht5320);

	return 0;
}

INIT_PREV_EXPORT(HT5320_Init);

// dac设置
static void ht5320_test(int argc, char **argv)
{
	if (argc != 3)
	{
		log_i("Please input bepp type 0-4 ");
		return;
	}
	int ch = 0;

	rt_sscanf(argv[1], "%d", &ch);
	float val = 0;
	rt_sscanf(argv[2], "%f", &val);
	uint16_t adc = 0;
	log_i("ht5320 ch %d = %f", ch, val);
	if (ch == 1)
	{
		adc = (uint16_t)(val / 3.3f * 4096);
		ht5320_write(ht5320[0].name, adc);
	}
	else if (ch == 2)
	{
		adc = (uint16_t)(val / 3.3f * 4096);
		ht5320_write(ht5320[1].name, adc);
	}
	else if (ch == 3)
	{
		val = (val + 0.1579f) / 306.32f;
		adc = (uint16_t)(val / 3.3f * 4096);
		ht5320_write(ht5320[2].name, adc);
	}
	else if (ch == 4)
	{
		val = (val + 0.1579f) / 306.32f;
		adc = (uint16_t)(val / 3.3f * 4096);
		ht5320_write(ht5320[3].name, adc);
	}
	log_i("ch = %d val = %f adc %d %04x gaoya %f", ch, val, adc, adc, val * 306.32f - 0.1579f);
}
MSH_CMD_EXPORT(ht5320_test, ht5320_test);

#endif /* BRD_DAC_HT5320 */
