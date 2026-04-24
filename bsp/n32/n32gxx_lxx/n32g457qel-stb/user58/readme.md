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

