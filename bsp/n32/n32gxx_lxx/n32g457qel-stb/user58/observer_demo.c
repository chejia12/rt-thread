/**
 * @file observer_example.c
 * @brief 观察者模式使用示例
 * @author 
 * @version 1.0
 * @date 2026-02-10
 *
 * 演示如何使用基于RT-Thread rt_list的观察者模式
 */
#include <rtthread.h>
#ifdef BRD_OBSERVER_SAMPLE
#include "observer.h"

#define DBG_TAG               "OBS_EX"
#define DBG_LVL               DBG_LOG
#include <rtdbg.h>

/* 定义一个具体的数据结构作为被观察对象 */
typedef struct
{
    subject_t subject;          /* 继承被观察者 */
    int temperature;           /* 温度数据 */
    int humidity;              /* 湿度数据 */
} sensor_data_t;

/* 定义观察者类型 */
typedef struct
{
    observer_t observer;       /* 继承观察者 */
    char name[32];            /* 观察者名称 */
} sensor_observer_t;

/* 观察者1的更新回调函数 */
static void temperature_observer_update(void *observer, void *subject, void *data)
{
    sensor_observer_t *temp_observer = (sensor_observer_t *)observer;
    sensor_data_t *sensor_data = (sensor_data_t *)subject;
    
    rt_kprintf("Temperature Observer [%s]: Temperature = %d°C, Humidity = %d%%\n", 
               temp_observer->name, sensor_data->temperature, sensor_data->humidity);
}

/* 观察者2的更新回调函数 */
static void humidity_observer_update(void *observer, void *subject, void *data)
{
    sensor_observer_t *humi_observer = (sensor_observer_t *)observer;
    sensor_data_t *sensor_data = (sensor_data_t *)subject;
    
    rt_kprintf("Humidity Observer [%s]: Temperature = %d°C, Humidity = %d%%\n", 
               humi_observer->name, sensor_data->temperature, sensor_data->humidity);
}

/* 观察者3的更新回调函数 */
static void display_observer_update(void *observer, void *subject, void *data)
{
    sensor_observer_t *display_observer = (sensor_observer_t *)observer;
    sensor_data_t *sensor_data = (sensor_data_t *)subject;
    
    rt_kprintf("Display Observer [%s]: Updating display with T=%d°C, H=%d%%\n", 
               display_observer->name, sensor_data->temperature, sensor_data->humidity);
}

/* 测试函数 */
static void observer_test(void)
{
    /* 创建被观察者 */
    sensor_data_t sensor_data;
    sensor_data.temperature = 25;
    sensor_data.humidity = 60;
    
    /* 初始化被观察者 */
    if (subject_init(&sensor_data.subject) != RT_EOK)
    {
        LOG_E("Failed to initialize subject");
        return;
    }
    
    /* 创建观察者 */
    sensor_observer_t temp_observer;
    sensor_observer_t humi_observer;
    sensor_observer_t display_observer;
    
    /* 初始化观察者 */
    rt_strncpy(temp_observer.name, "TempMonitor", sizeof(temp_observer.name));
    if (observer_init(&temp_observer.observer, temperature_observer_update, RT_NULL) != RT_EOK)
    {
        LOG_E("Failed to initialize temperature observer");
        return;
    }
    
    rt_strncpy(humi_observer.name, "HumiMonitor", sizeof(humi_observer.name));
    if (observer_init(&humi_observer.observer, humidity_observer_update, RT_NULL) != RT_EOK)
    {
        LOG_E("Failed to initialize humidity observer");
        return;
    }
    
    rt_strncpy(display_observer.name, "DisplayCtrl", sizeof(display_observer.name));
    if (observer_init(&display_observer.observer, display_observer_update, RT_NULL) != RT_EOK)
    {
        LOG_E("Failed to initialize display observer");
        return;
    }
    
    LOG_I("=== Observer Pattern Test Start ===");
    
    /* 添加观察者 */
    LOG_I("Adding observers...");
    subject_add_observer(&sensor_data.subject, &temp_observer.observer);
    subject_add_observer(&sensor_data.subject, &humi_observer.observer);
    subject_add_observer(&sensor_data.subject, &display_observer.observer);
    
    LOG_I("Current observer count: %d", subject_get_observer_count(&sensor_data.subject));
    
    /* 模拟数据变化并通知观察者 */
    LOG_I("\n--- Test 1: Initial data notification ---");
    subject_notify(&sensor_data.subject, RT_NULL);
    
    LOG_I("\n--- Test 2: Temperature change ---");
    sensor_data.temperature = 28;
    subject_notify(&sensor_data.subject, RT_NULL);
    
    LOG_I("\n--- Test 3: Humidity change ---");
    sensor_data.humidity = 65;
    subject_notify(&sensor_data.subject, RT_NULL);
    
    LOG_I("\n--- Test 4: Both changes ---");
    sensor_data.temperature = 30;
    sensor_data.humidity = 70;
    subject_notify(&sensor_data.subject, RT_NULL);
    
    /* 移除一个观察者 */
    LOG_I("\n--- Test 5: Remove temperature observer ---");
    subject_remove_observer(&sensor_data.subject, &temp_observer.observer);
    LOG_I("Observer count after removal: %d", subject_get_observer_count(&sensor_data.subject));
    
    LOG_I("\n--- Test 6: Notify remaining observers ---");
    sensor_data.temperature = 22;
    sensor_data.humidity = 55;
    subject_notify(&sensor_data.subject, RT_NULL);
    
    /* 尝试重复添加同一个观察者 */
    LOG_I("\n--- Test 7: Try to add duplicate observer ---");
    if (subject_add_observer(&sensor_data.subject, &humi_observer.observer) != RT_EOK)
    {
        LOG_W("Duplicate observer rejected as expected");
    }
    
    /* 尝试移除不存在的观察者 */
    LOG_I("\n--- Test 8: Try to remove non-existent observer ---");
    sensor_observer_t fake_observer;
    observer_init(&fake_observer.observer, temperature_observer_update, RT_NULL);
    if (subject_remove_observer(&sensor_data.subject, &fake_observer.observer) != RT_EOK)
    {
        LOG_W("Non-existent observer removal failed as expected");
    }
    
    LOG_I("\n=== Observer Pattern Test Complete ===");
}

/* 在FinSH中导出测试命令 */
MSH_CMD_EXPORT(observer_test, test observer pattern implementation);

/**================================测试代码==========================================
msh />observer_test
[12930] V/OBS_EX tshell: === Observer Pattern Test Start ===
[12937] V/OBS_EX tshell: Adding observers...
[12942] V/OBS_EX tshell: Current observer count: 3
[12947] V/OBS_EX tshell:
--- Test 1: Initial data notification ---
Temperature Observer [TempMonitor]: Temperature = 25°C, Humidity = 60%
Humidity Observer [HumiMonitor]: Temperature = 25°C, Humidity = 60%
Display Observer [DisplayCtrl]: Updating display with T=25°C, H=60%
[12972] V/OBS_EX tshell:
--- Test 2: Temperature change ---
Temperature Observer [TempMonitor]: Temperature = 28°C, Humidity = 60%
Humidity Observer [HumiMonitor]: Temperature = 28°C, Humidity = 60%
Display Observer [DisplayCtrl]: Updating display with T=28°C, H=60%
[12997] V/OBS_EX tshell:
--- Test 3: Humidity change ---
Temperature Observer [TempMonitor]: Temperature = 28°C, Humidity = 65%
Humidity Observer [HumiMonitor]: Temperature = 28°C, Humidity = 65%
Display Observer [DisplayCtrl]: Updating display with T=28°C, H=65%
[13022] V/OBS_EX tshell:
--- Test 4: Both changes ---
Temperature Observer [TempMonitor]: Temperature = 30°C, Humidity = 70%
Humidity Observer [HumiMonitor]: Temperature = 30°C, Humidity = 70%
Display Observer [DisplayCtrl]: Updating display with T=30°C, H=70%
[13046] V/OBS_EX tshell:
--- Test 5: Remove temperature observer ---
[13053] V/OBS_EX tshell: Observer count after removal: 2
[13059] V/OBS_EX tshell:
--- Test 6: Notify remaining observers ---
Humidity Observer [HumiMonitor]: Temperature = 22°C, Humidity = 55%
Display Observer [DisplayCtrl]: Updating display with T=22°C, H=55%
[13078] V/OBS_EX tshell:
--- Test 7: Try to add duplicate observer ---
[13086] W/OBSERVER tshell: Observer already exists
[13091] W/OBS_EX tshell: Duplicate observer rejected as expected
[13098] V/OBS_EX tshell:
--- Test 8: Try to remove non-existent observer ---
[13105] W/OBSERVER tshell: Observer not found
[13110] W/OBS_EX tshell: Non-existent observer removal failed as expected
[13118] V/OBS_EX tshell:
=== Observer Pattern Test Complete ===

 
 */

 #endif/*BRD_OBSERVER_SAMPLE*/