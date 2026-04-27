# 外部计数模式的KConfig配置

# N32L40x 

2026-04-24

```
#-----------------------------EXT_COUNTER----N32L40x----20260424新增TIM 4,5,8 外部时钟1计数配置--------------------------
	menuconfig BSP_USING_TIM_EXT_COUNTER
	bool "Enable TIM External Counter"
	default n
	select RT_USING_HWTIMER
	if BSP_USING_TIM_EXT_COUNTER
		config BSP_USING_TIM1_EXT_COUNTER
			bool "Enable TIM1 External Counter"
			default n
			choice
				prompt "Select TIM1 Pin"
				default TIM1_CH1_PA8
				config TIM1_CH1_PA8
					bool "PA8 AF2"
				config TIM1_CH2_PA9
					bool "PA9 AF2"
			endchoice

		config BSP_USING_TIM2_EXT_COUNTER
			bool "Enable TIM2 External Counter"
			default n
			choice
				prompt "Select TIM2 Pin"
				default TIM2_CH1_PA0
				config TIM2_CH1_PA0
					bool "PA0 AF2"
				config TIM2_CH1_PA15
					bool "PA15 AF5"
				config TIM2_CH2_PA1
					bool "PA1 AF2"
				config TIM2_CH2_PB3
					bool "PB3 AF2"
			endchoice

		config BSP_USING_TIM3_EXT_COUNTER
			bool "Enable TIM3 External Counter"
			default n
			choice
				prompt "Select TIM3 Pin"
				default TIM3_CH1_PA6
				config TIM3_CH1_PA6
					bool "PA6 AF2"
				config TIM3_CH1_PB4
					bool "PB4 AF2"
				config TIM3_CH1_PC6
					bool "PC6 AF2"
				config TIM3_CH2_PA7
					bool "PA7 AF2"
				config TIM3_CH2_PB5
					bool "PB5 AF2"
				config TIM3_CH2_PC7
					bool "PC7 AF2"
			endchoice
		
		config BSP_USING_TIM4_EXT_COUNTER
			bool "Enable TIM4 External Counter"
			default n
			choice
				prompt "Select TIM4 Pin"
				default TIM4_EXCK1_CH1_PB6
				config TIM4_EXCK1_CH1_PB6
					bool "CH1_PB6"
				config TIM4_EXCK1_CH2_PB7
					bool "CH2_PB7"
			endchoice
		config BSP_USING_TIM5_EXT_COUNTER
			bool "Enable TIM5 External Counter"
			default n
			choice
				prompt "Select TIM5 Pin"
				default TIM5_EXCK1_CH1_PA0
				config TIM5_EXCK1_CH1_PA0
					bool "CH1_PA0"
				config TIM5_EXCK1_CH2_PA1
					bool "CH2_PA1"
			endchoice
		config BSP_USING_TIM8_EXT_COUNTER
			bool "Enable TIM8 External Counter"
			default n
			choice
				prompt "Select TIM8 Pin"
				default TIM8_EXCK1_CH1_PC6
				config TIM8_EXCK1_CH1_PC6
					bool "CH1_PC6"
				config TIM8_EXCK1_CH2_PC7
					bool "CH2_PC7"
			endchoice	
          config BSP_USING_TIM9_EXT_COUNTER
			bool "Enable TIM9 External Counter"
			default n
			choice
				prompt "Select TIM9 Pin"
				default TIM9_EXCK1_CH1_PB12
				config TIM9_EXCK1_CH1_PB12
					bool "CH1_PB12"
				config TIM9_EXCK1_CH2_PB13
					bool "CH2_PB13"
			endchoice		
	endif#---------BSP_USING_TIM_EXT_COUNTER------------
```



# 20260427串口V2版本的

## N32G457的KConfig

```c
    menuconfig BSP_USING_UART
        bool "Enable UART"
        default y
        select RT_USING_SERIAL
        if BSP_USING_UART
            menuconfig BSP_USING_USART1
                bool "Enable USART1"
                default y
                if BSP_USING_USART1
                    choice
                        prompt "Set usart1 afio mode"
                        default BSP_USART1_AFIO_MODE_PA9_PA10

                        config BSP_USART1_AFIO_MODE_PA9_PA10
                            bool
                            prompt "PA9 PA10"

                        config BSP_USART1_AFIO_MODE_PB6_PB7
                            bool
                            prompt "PB6 PB7"
                    endchoice
                    
                    config BSP_UART1_RX_USING_DMA
                        bool "Enable usart1 rx dma"
                        depends on BSP_USING_USART1 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART1_TX_USING_DMA
                        bool "Enable usart1 tx dma"
                        depends on BSP_USING_USART1 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART1_DMA_PING_BUFSIZE
                        int "Set UART1 RX DMA ping-pong buffer size"
                        range 16 65535
                        depends on RT_USING_SERIAL_V2 && BSP_UART1_RX_USING_DMA
                        default 32

                    config BSP_UART1_RX_BUFSIZE
                        int "Set rx buffer size"
                        range 0 16384
                        depends on RT_USING_SERIAL_V2
                        default 256

                    config BSP_UART1_TX_BUFSIZE
                        int "Set tx buffer size"
                        range 0 16384
                        depends on RT_USING_SERIAL_V2
                        default 0
                endif

            menuconfig BSP_USING_USART2
                bool "Enable USART2"
                default n
                if BSP_USING_USART2
                    choice
                        prompt "Set usart2 afio mode"
                        default BSP_USART2_AFIO_MODE_PA2_PA3

                        config BSP_USART2_AFIO_MODE_PA2_PA3
                            bool
                            prompt "PA2 PA3"

                        config BSP_USART2_AFIO_MODE_PD5_PD6
                            bool
                            prompt "PD5 PD6"

                        config BSP_USART2_AFIO_MODE_PC8_PC9
                            bool
                            prompt "PC8 PC9"

                        config BSP_USART2_AFIO_MODE_PB4_PB5
                            bool
                            prompt "PB4 PB5"
                    endchoice
        
                    config BSP_UART2_RX_USING_DMA
                        bool "Enable usart2 rx dma"
                        depends on BSP_USING_USART2 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART2_TX_USING_DMA
                        bool "Enable usart2 tx dma"
                        depends on BSP_USING_USART2 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART2_DMA_PING_BUFSIZE
                        int "Set UART2 RX DMA ping-pong buffer size"
                        range 16 65535
                        depends on RT_USING_SERIAL_V2 && BSP_UART2_RX_USING_DMA
                        default 32

                    config BSP_UART2_RX_BUFSIZE
                        int "Set rx buffer size"
                        range 256 16384
                        depends on RT_USING_SERIAL_V2
                        default 256

                    config BSP_UART2_TX_BUFSIZE
                        int "Set tx buffer size"
                        range 0 16384
                        depends on RT_USING_SERIAL_V2
                        default 0
                endif

            menuconfig BSP_USING_USART3
                bool "Enable USART3"
                default n
                if BSP_USING_USART3
                    choice
                        prompt "Set usart3 afio mode"
                        default BSP_USART3_AFIO_MODE_PB10_PB11

                        config BSP_USART3_AFIO_MODE_PB10_PB11
                            bool
                            prompt "PB10 PB11"

                        config BSP_USART3_AFIO_MODE_PC10_PC11
                            bool
                            prompt "PC10 PC11"

                        config BSP_USART3_AFIO_MODE_PD8_PD9
                            bool
                            prompt "PD8 PD9"
                    endchoice
        
                    config BSP_UART3_RX_USING_DMA
                        bool "Enable usart3 rx dma"
                        depends on BSP_USING_USART3 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART3_TX_USING_DMA
                        bool "Enable usart3 tx dma"
                        depends on BSP_USING_USART3 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART3_DMA_PING_BUFSIZE
                        int "Set UART3 RX DMA ping-pong buffer size"
                        range 16 65535
                        depends on RT_USING_SERIAL_V2 && BSP_UART3_RX_USING_DMA
                        default 32

                    config BSP_UART3_RX_BUFSIZE
                        int "Set rx buffer size"
                        range 256 16384
                        depends on RT_USING_SERIAL_V2
                        default 256

                    config BSP_UART3_TX_BUFSIZE
                        int "Set tx buffer size"
                        range 0 16384
                        depends on RT_USING_SERIAL_V2
                        default 0
                endif

            menuconfig BSP_USING_UART4
                bool "Enable UART4"
                default n
                if BSP_USING_UART4
                    choice
                        prompt "Set uart4 afio mode"
                        default BSP_UART4_AFIO_MODE_PC10_PC11

                        config BSP_UART4_AFIO_MODE_PC10_PC11
                            bool
                            prompt "PC10 PC11"

                        config BSP_UART4_AFIO_MODE_PB2_PE7
                            bool
                            prompt "PB2 PE7"

                        config BSP_UART4_AFIO_MODE_PA13_PA14
                            bool
                            prompt "PA13 PA14"

                        config BSP_UART4_AFIO_MODE_PD0_PD1
                            bool
                            prompt "PD0 PD1"
                    endchoice
        
                    config BSP_UART4_RX_USING_DMA
                        bool "Enable uart4 rx dma"
                        depends on BSP_USING_UART4 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART4_TX_USING_DMA
                        bool "Enable uart4 tx dma"
                        depends on BSP_USING_UART4 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART4_DMA_PING_BUFSIZE
                        int "Set UART4 RX DMA ping-pong buffer size"
                        range 16 65535
                        depends on RT_USING_SERIAL_V2 && BSP_UART4_RX_USING_DMA
                        default 32
                        
                    config BSP_UART4_RX_BUFSIZE
                        int "Set rx buffer size"
                        range 256 16384
                        depends on RT_USING_SERIAL_V2
                        default 256

                    config BSP_UART4_TX_BUFSIZE
                        int "Set tx buffer size"
                        range 0 16384
                        depends on RT_USING_SERIAL_V2
                        default 0
                endif

            menuconfig BSP_USING_UART5
                bool "Enable UART5"
                default n
                if BSP_USING_UART5
                    choice
                        prompt "Set uart5 afio mode"
                        default BSP_UART5_AFIO_MODE_PC12_PD2

                        config BSP_UART5_AFIO_MODE_PC12_PD2
                            bool
                            prompt "PC12 PD2"

                        config BSP_UART5_AFIO_MODE_PB13_PB14
                            bool
                            prompt "PB13 PB14"

                        config BSP_UART5_AFIO_MODE_PE8_PE9
                            bool
                            prompt "PE8 PE9"

                        config BSP_UART5_AFIO_MODE_PB8_PB9
                            bool
                            prompt "PB8 PB9"
                    endchoice
        
                    config BSP_UART5_RX_USING_DMA
                        bool "Enable uart5 rx dma"
                        depends on BSP_USING_UART5 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART5_TX_USING_DMA
                        bool "Enable uart5 tx dma"
                        depends on BSP_USING_UART5 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART5_DMA_PING_BUFSIZE
                        int "Set UART5 RX DMA ping-pong buffer size"
                        range 16 65535
                        depends on RT_USING_SERIAL_V2 && BSP_UART5_RX_USING_DMA
                        default 32
                        
                    config BSP_UART5_RX_BUFSIZE
                        int "Set rx buffer size"
                        range 256 16384
                        depends on RT_USING_SERIAL_V2
                        default 256

                    config BSP_UART5_TX_BUFSIZE
                        int "Set tx buffer size"
                        range 0 16384
                        depends on RT_USING_SERIAL_V2
                        default 0
                endif

            menuconfig BSP_USING_UART6
                bool "Enable UART6"
                default n
                if BSP_USING_UART6
                    choice
                        prompt "Set uart6 afio mode"
                        default BSP_UART6_AFIO_MODE_PE2_PE3

                        config BSP_UART6_AFIO_MODE_PE2_PE3
                            bool
                            prompt "PE2 PE3"

                        config BSP_UART6_AFIO_MODE_PC0_PC1
                            bool
                            prompt "PC0 PC1"

                        config BSP_UART6_AFIO_MODE_PB0_PB1
                            bool
                            prompt "PB0 PB1"
                    endchoice
        
                    config BSP_UART6_RX_USING_DMA
                        bool "Enable uart6 rx dma"
                        depends on BSP_USING_UART6 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART6_TX_USING_DMA
                        bool "Enable uart6 tx dma"
                        depends on BSP_USING_UART6 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART6_DMA_PING_BUFSIZE
                        int "Set UART6 RX DMA ping-pong buffer size"
                        range 16 65535
                        depends on RT_USING_SERIAL_V2 && BSP_UART6_RX_USING_DMA
                        default 32

                    config BSP_UART6_RX_BUFSIZE
                        int "Set rx buffer size"
                        range 256 16384
                        depends on RT_USING_SERIAL_V2
                        default 256

                    config BSP_UART6_TX_BUFSIZE
                        int "Set tx buffer size"
                        range 0 16384
                        depends on RT_USING_SERIAL_V2
                        default 0
                endif

            menuconfig BSP_USING_UART7
                bool "Enable UART7"
                default n
                if BSP_USING_UART7
                    choice
                        prompt "Set uart7 afio mode"
                        default BSP_UART7_AFIO_MODE_PC4_PC5

                        config BSP_UART7_AFIO_MODE_PC4_PC5
                            bool
                            prompt "PC4 PC5"

                        config BSP_UART7_AFIO_MODE_PC2_PC3
                            bool
                            prompt "PC2 PC3"

                        config BSP_UART6_AFIO_MODE_PG0_PG1
                            bool
                            prompt "PG0 PG1"
                    endchoice
        
                    config BSP_UART7_RX_USING_DMA
                        bool "Enable uart7 rx dma"
                        depends on BSP_USING_UART7 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART7_TX_USING_DMA
                        bool "Enable uart7 tx dma"
                        depends on BSP_USING_UART7 && RT_SERIAL_USING_DMA
                        default n

                    config BSP_UART7_DMA_PING_BUFSIZE
                        int "Set UART7 RX DMA ping-pong buffer size"
                        range 16 65535
                        depends on RT_USING_SERIAL_V2 && BSP_UART7_RX_USING_DMA
                        default 32

                    config BSP_UART7_RX_BUFSIZE
                        int "Set rx buffer size"
                        range 256 16384
                        depends on RT_USING_SERIAL_V2
                        default 256

                    config BSP_UART7_TX_BUFSIZE
                        int "Set tx buffer size"
                        range 0 16384
                        depends on RT_USING_SERIAL_V2
                        default 0
                endif
        endif

```



# spi外设的芯片选择

## N32G457

```
    menuconfig BSP_USING_SPI
        bool "Enable SPI BUS"
        default n
        select RT_USING_SPI
        if BSP_USING_SPI
            config BSP_USING_SPI1
                bool "Enable SPI1 BUS"
                default n
                if BSP_USING_SPI1
                    choice
                    prompt "Select SPI1 PIN"
                    default BSP_SPI1_REMAP_0
                    config BSP_SPI1_REMAP_0
                        bool "PA5 PA6 PA7"
                    config BSP_SPI1_REMAP_1
                        bool "PB3 PB4 PB5 "
                    config BSP_SPI1_REMAP_3
                        bool "PE7 PE 8 PE9"
                endchoice
                endif

            config BSP_USING_SPI2
                bool "Enable SPI2 BUS"
                default n
                if BSP_USING_SPI2
                    choice
                    prompt "Select SPI2 PIN"
                    default BSP_SPI2_REMAP_0
                    config BSP_SPI2_REMAP_0
                        bool "PB13 PB14 PB15"
                    config BSP_SPI2_REMAP_1
                        bool "PC7 PC8 PC9"
                    config BSP_SPI2_REMAP_3
                        bool "PE11 PE12 PE13"
                endchoice
                endif

            config BSP_USING_SPI3
                bool "Enable SPI3 BUS"
                default n
                if BSP_USING_SPI3
                    choice
                    prompt "Select SPI3 PIN"
                    default BSP_SPI3_REMAP_0
                    config BSP_SPI3_REMAP_0
                        bool "PB3 PB4 PB5 "
                    config BSP_SPI3_REMAP_1
                        bool "PC10 PC11 PC12"
                    config BSP_SPI3_REMAP_3
                        bool "PD9 PD11 PD12"
                endchoice
                endif
    	endif#BSP_USING_SPI

```



## 修改了drv_spi.c源码

```c
/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-19     Nations      first version
 * 
 *
 * 202512-03
 *      1.spi 的驱动优化，在G457上spi2可行
 *      2.新增了Kconfig的配置文件，测试spi2的remap=3测试OK
 * 2025-12-04
 *      1.增加了spi1和spi2的Kconfig配置选项，未测试
 * 2026-04-27
 * 	1.在N32G45x测试了spi1，读取nor flash测试OK
 * 	2.删除了不必要的代码，添加了注释
 *  */
#include <rtthread.h>

#define DBG_TAG "SPI"
#define DBG_LVL LOG_LVL_INFO
#include <rtdbg.h>

#include "drv_base.h"
#include "drv_spi.h"
#if defined(RT_USING_SPI) && defined(RT_USING_PIN)
#include <rtdevice.h>

#if defined(BSP_USING_SPI1) || defined(BSP_USING_SPI2) || \
	defined(BSP_USING_SPI3)
/**
 * @brief 抽象定义的一个SPI设备结构体
 *2026-04-27
 */
typedef struct
{
	struct rt_spi_bus spi_bus; /*spi设备对象*/
	SPI_Module *spi;		   /*总线句柄*/
	uint32_t spi_remap;		   /*引脚映射*/
	uint16_t pin_sclk;		   /*时钟引脚*/
	uint16_t pin_mosi;		   /*mosi引脚*/
	uint16_t pin_miso;		   /*miso引脚*/
	GPIO_Module *gpio_grp;	   /*gpio分组*/
	char *name;				   /*spi设备名称*/
	char *info;				   /*spi的提示信息*/
} n32_spi_hd_t;


/**spi对象定义 */
static n32_spi_hd_t n32_spi_hd[] = {

#ifdef BSP_USING_SPI1
	{
		.spi = SPI1,
		.name = "spi1",
#ifdef BSP_SPI1_REMAP_0
		.spi_remap = 0,
		.pin_sclk = GPIO_PIN_5,
		.pin_miso = GPIO_PIN_6,
		.pin_mosi = GPIO_PIN_7,
		.gpio_grp = GPIOA,
		.info = " PA5 PA6 PA7",
#endif /*BSP_SPI_REMAP_0*/
#ifdef BSP_SPI1_REMAP_1
		.spi_remap = GPIO_RMP1_SPI1,
		.pin_sclk = GPIO_PIN_3,
		.pin_miso = GPIO_PIN_4,
		.pin_mosi = GPIO_PIN_5,
		.gpio_grp = GPIOB,
		.info = " PB3 PB4 PB5 (JATG)",
#endif /*BSP_SPI_REMAP_1*/
#ifdef BSP_SPI1_REMAP_3
		.spi_remap = GPIO_RMP3_SPI1,
		.pin_sclk = GPIO_PIN_7,
		.pin_miso = GPIO_PIN_8,
		.pin_mosi = GPIO_PIN_9,
		.gpio_grp = GPIOE,
		.info = " PE7 PE8 PE9",
#endif /*BSP_SPI_REMAP_3*/
	},
#endif /*BSP_USING_SPI1*/

#ifdef BSP_USING_SPI2
	{

		.spi = SPI2,
		.name = "spi2",
#ifdef BSP_SPI2_REMAP_0
		.spi_remap = 0,
		.pin_sclk = GPIO_PIN_13,
		.pin_miso = GPIO_PIN_14,
		.pin_mosi = GPIO_PIN_15,
		.gpio_grp = GPIOB,
		.info = " PB13,PB14,PB15",
#endif /*BSP_SPI_REMAP_0*/
#ifdef BSP_SPI2_REMAP_1
		.spi_remap = GPIO_RMP1_SPI2,
		.pin_sclk = GPIO_PIN_7,
		.pin_miso = GPIO_PIN_8,
		.pin_mosi = GPIO_PIN_9,
		.gpio_grp = GPIOC,
		.info = " PC7 PC8 PC9",
#endif /*BSP_SPI_REMAP_1*/
#ifdef BSP_SPI2_REMAP_3
		.spi_remap = GPIO_RMP2_SPI2,
		.pin_sclk = GPIO_PIN_11,
		.pin_miso = GPIO_PIN_12,
		.pin_mosi = GPIO_PIN_13,
		.gpio_grp = GPIOE,
		.info = " PE11 PE12 PE13",
#endif /*BSP_SPI_REMAP_3*/
	},
#endif /*BSP_USING_SPI2*/

#ifdef BSP_USING_SPI3
	{

		.spi = SPI3,
		.name = "spi3",
#ifdef BSP_SPI3_REMAP_0
		.spi_remap = 0,
		.pin_sclk = GPIO_PIN_3,
		.pin_miso = GPIO_PIN_4,
		.pin_mosi = GPIO_PIN_5,
		.gpio_grp = GPIOB,
		.info = "spi3 PB3 PB4 PB5",
#endif /*BSP_SPI3_REMAP_0*/
#ifdef BSP_SPI3_REMAP_1
		.spi_remap = GPIO_RMP1_SPI3,
		.pin_sclk = GPIO_PIN_10,
		.pin_miso = GPIO_PIN_11,
		.pin_mosi = GPIO_PIN_12,
		.gpio_grp = GPIOC,
		.info = "spi3 PC10 PC11 PC12",
#endif /*BSP_SPI3_REMAP_1*/
#ifdef BSP_SPI3_REMAP2
		.spi_remap = GPIO_RMP2_SPI3,
		.pin_sclk = GPIO_PIN_9,
		.pin_miso = GPIO_PIN_10,
		.pin_mosi = GPIO_PIN_11,
		.gpio_grp = GPIOD,
		.info = "spi2 PD9 PD11 PD12",
#endif /*BSP_SPI3_REMAP_3*/
	},
#endif /*BSP_USING_SPI2*/

};

/**
 * @brief spi总线配置
 *
 * @param  device           设备对象
 * @param  configuration    spi配置参数
 * @return rt_err_t
 */
static rt_err_t configure(struct rt_spi_device *device, struct rt_spi_configuration *configuration)
{
	SPI_InitType SPI_InitStructure;
	RCC_ClocksType RCC_ClockFreq;
	SPI_Module *spi_periph;

	RT_ASSERT(device != RT_NULL);
	RT_ASSERT(configuration != RT_NULL);

	RCC_GetClocksFreqValue(&RCC_ClockFreq);

	spi_periph = (SPI_Module *)device->bus->parent.user_data;

#if defined(SOC_N32G45X) || defined(SOC_N32WB452) || defined(SOC_N32G4FR)
	if (spi_periph != SPI1 && spi_periph != SPI2 && spi_periph != SPI3)
	{
		return -RT_EIO;
	}
#elif defined(SOC_N32L43X) || defined(SOC_N32L40X) || defined(SOC_N32G43X)
	if (spi_periph != SPI1 && spi_periph != SPI2)
	{
		return -RT_EIO;
	}
#endif

	if (configuration->data_width <= 8)
	{
		SPI_InitStructure.DataLen = SPI_DATA_SIZE_8BITS;
	}
	else if (configuration->data_width <= 16)
	{
		SPI_InitStructure.DataLen = SPI_DATA_SIZE_16BITS;
	}
	else
	{
		return -RT_EIO;
	}

	{
		rt_uint32_t spi_apb_clock;
		rt_uint32_t max_hz;

		max_hz = configuration->max_hz;

		LOG_D("sys   freq: %d\n", RCC_ClockFreq.SysclkFreq);
		LOG_D("CK_APB2 freq: %d\n", RCC_ClockFreq.Pclk2Freq);
		LOG_D("max   freq: %d\n", max_hz);

		if (spi_periph == SPI1)
		{
			spi_apb_clock = RCC_ClockFreq.Pclk2Freq;
		}
		else
		{
			spi_apb_clock = RCC_ClockFreq.Pclk1Freq;
		}

		if (max_hz >= spi_apb_clock / 2)
		{
			SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_2;
		}
		else if (max_hz >= spi_apb_clock / 4)
		{
			SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_4;
		}
		else if (max_hz >= spi_apb_clock / 8)
		{
			SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_8;
		}
		else if (max_hz >= spi_apb_clock / 16)
		{
			SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_16;
		}
		else if (max_hz >= spi_apb_clock / 32)
		{
			SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_32;
		}
		else if (max_hz >= spi_apb_clock / 64)
		{
			SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_64;
		}
		else if (max_hz >= spi_apb_clock / 128)
		{
			SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_128;
		}
		else
		{
			/*  min prescaler 256 */
			SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_256;
		}
	} /* baudrate */

	switch (configuration->mode & RT_SPI_MODE_3)
	{
	case RT_SPI_MODE_0:
		SPI_InitStructure.CLKPOL = SPI_CLKPOL_LOW;
		SPI_InitStructure.CLKPHA = SPI_CLKPHA_FIRST_EDGE;
		break;
	case RT_SPI_MODE_1:
		SPI_InitStructure.CLKPOL = SPI_CLKPOL_LOW;
		SPI_InitStructure.CLKPHA = SPI_CLKPHA_SECOND_EDGE;
		break;
	case RT_SPI_MODE_2:
		SPI_InitStructure.CLKPOL = SPI_CLKPOL_HIGH;
		SPI_InitStructure.CLKPHA = SPI_CLKPHA_FIRST_EDGE;
		break;
	case RT_SPI_MODE_3:
		SPI_InitStructure.CLKPOL = SPI_CLKPOL_HIGH;
		SPI_InitStructure.CLKPHA = SPI_CLKPHA_SECOND_EDGE;
		break;
	}

	/* MSB or LSB */
	if (configuration->mode & RT_SPI_MSB)
	{
		SPI_InitStructure.FirstBit = SPI_FB_MSB;
	}
	else
	{
		SPI_InitStructure.FirstBit = SPI_FB_LSB;
	}
	/* SPI configuration */
	SPI_InitStructure.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
	SPI_InitStructure.SpiMode = SPI_MODE_MASTER;
	SPI_InitStructure.NSS = SPI_NSS_SOFT;
	SPI_InitStructure.CRCPoly = 7;

	SPI_Init(spi_periph, &SPI_InitStructure);

	/* Enable the sFLASH_SPI  */
	SPI_Enable(spi_periph, ENABLE);

	return RT_EOK;
}
/**
 * @brief spi数据收发接口
 * 
 * @param  device           设备对象
 * @param  message          消息体
 * @return rt_ssize_t 
 */
static rt_ssize_t xfer(struct rt_spi_device *device, struct rt_spi_message *message)
{
	struct n32_spi_cs *cs_pin = device->parent.user_data;
	SPI_Module *spi_periph = (SPI_Module *)device->bus->parent.user_data;
	struct rt_spi_configuration *config = &device->config;

	RT_ASSERT(device != NULL);
	RT_ASSERT(message != NULL);

	/* take CS */
	if (message->cs_take)
	{
		rt_pin_write(cs_pin->GPIO_Pin, PIN_LOW);
		LOG_D("spi take cs %d \n", cs_pin->GPIO_Pin);
	}

	if (config->data_width <= 8)
	{
		const rt_uint8_t *send_ptr = message->send_buf;
		rt_uint8_t *recv_ptr = message->recv_buf;
		rt_uint32_t size = message->length;

		LOG_D("spi poll transfer start: %d\n", size);

		while (size--)
		{
			rt_uint8_t data = 0xA5;

			if (send_ptr != RT_NULL)
			{
				data = *send_ptr++;
			}

			/* Loop while DAT register in not emplty */
			while (SPI_I2S_GetStatus(spi_periph, SPI_I2S_TE_FLAG) == RESET)
				;

			/* Send the byte */
			SPI_I2S_TransmitData(spi_periph, data);

			/* Wait until a data is received */
			while (SPI_I2S_GetStatus(spi_periph, SPI_I2S_RNE_FLAG) == RESET)
				;

			/* Get the received data */
			data = SPI_I2S_ReceiveData(spi_periph);

			if (recv_ptr != RT_NULL)
			{
				*recv_ptr++ = data;
			}
		}
		LOG_D("spi poll transfer finsh\n");
	}
	else if (config->data_width <= 16)
	{
		const rt_uint16_t *send_ptr = message->send_buf;
		rt_uint16_t *recv_ptr = message->recv_buf;
		rt_uint32_t size = message->length;

		while (size--)
		{
			rt_uint16_t data = 0xFF;

			if (send_ptr != RT_NULL)
			{
				data = *send_ptr++;
			}

			/* Loop while DAT register in not emplty */
			while (SPI_I2S_GetStatus(spi_periph, SPI_I2S_TE_FLAG) == RESET)
				;

			/* Send the byte */
			SPI_I2S_TransmitData(spi_periph, data);

			/* Wait until a data is received */
			while (RESET == SPI_I2S_GetStatus(spi_periph, SPI_I2S_RNE_FLAG))
				;

			/* Get the received data */
			data = SPI_I2S_ReceiveData(spi_periph);

			if (recv_ptr != RT_NULL)
			{
				*recv_ptr++ = data;
			}
		}
	}

	/* release CS */
	if (message->cs_release)
	{
		rt_pin_write(cs_pin->GPIO_Pin, PIN_HIGH);
		LOG_D("spi release cs\n");
	}

	return message->length;
}
/**
 * @brief spi的ops接口实现
 * 
 */
static struct rt_spi_ops spi_ops =
	{
		configure,
		xfer,
};

/**
 * @brief spi时钟使能
 * 
 * @param  spi              总线句柄
 */
static void n32_spi_rcc_enable(const SPI_Module *spi)
{
	if (SPI1 == spi)
	{
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_SPI1, ENABLE);
	}
	else if (SPI2 == spi)
	{
		RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_SPI2, ENABLE);
	}
	else if (SPI3 == spi)
	{
		RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_SPI3, ENABLE);
	}
}

/**
 * @brief spi的gpio初始化
 * 
 * @param  spi_hd           
 */
void n32_spi_gpio_init(const n32_spi_hd_t *spi_hd)
{
	GPIO_InitType GPIO_InitStructure;
	GPIO_InitStruct(&GPIO_InitStructure);
	n32_gpio_rcc_enable(spi_hd->gpio_grp);
	if (spi_hd->spi_remap)
	{
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);
	}
#if defined(SOC_N32G45X) || defined(SOC_N32WB452) || defined(SOC_N32G4FR)

	/* Confige SPI1_SCLK and SPI1_MOSI */
	GPIO_InitStructure.Pin = spi_hd->pin_sclk | spi_hd->pin_mosi;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitPeripheral(spi_hd->gpio_grp, &GPIO_InitStructure);

	/* Confige SPI1_MISO */
	GPIO_InitStructure.Pin = spi_hd->pin_miso;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitPeripheral(spi_hd->gpio_grp, &GPIO_InitStructure);
#elif defined(SOC_N32L43X) || defined(SOC_N32L40X) || defined(SOC_N32G43X)
	GPIO_InitStruct(&GPIO_InitStructure);
	/* Confige SPI1_SCLK(PA5) and SPI1_MISO(PA6) and SPI1_MOSI(PA7) */
	GPIO_InitStructure.Pin = spi_hd->pin_sclk | spi_hd->pin_mosi | spi_hd->pin_miso;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Alternate = GPIO_AF0_SPI1;
	GPIO_InitPeripheral(spi_hd->gpio_grp, &GPIO_InitStructure);
#endif
}
/**
 * @brief spi外设的初始化配置
 * 
 * @param  spi_hd           
 */
static void n32_spi_init(n32_spi_hd_t *spi_hd)
{
	n32_spi_rcc_enable(spi_hd->spi);
	n32_spi_gpio_init(spi_hd);
	spi_hd->spi_bus.parent.user_data = spi_hd->spi;
	if (spi_hd->spi_remap)
	{
		GPIO_ConfigPinRemap(spi_hd->spi_remap, ENABLE);
	}
	if (RT_EOK == rt_spi_bus_register(&spi_hd->spi_bus, spi_hd->name, &spi_ops))
	{
		LOG_D("register %s is ok,pin={%s}", spi_hd->name, spi_hd->info);
	}
	else
	{
		LOG_W("register %s is error,pin={%s}", spi_hd->name, spi_hd->info);
	}
}

/**
 * @brief spi的初始化，注册多个spi总线
 * 
 * @return int 
 */
int rt_hw_spi_init(void)
{
	for (int i = 0; i < sizeof(n32_spi_hd) / sizeof(n32_spi_hd_t); i++)
	{
		n32_spi_init(n32_spi_hd + i);
	}
	return RT_EOK;

}
INIT_BOARD_EXPORT(rt_hw_spi_init);

#endif /* defined(BSP_USING_SPI1) || defined(BSP_USING_SPI2) || defined(BSP_USING_SPI3) */
#endif

```

