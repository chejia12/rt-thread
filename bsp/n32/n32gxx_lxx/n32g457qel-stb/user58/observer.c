/**
 * @file observer.c
 * @brief 观察者模式实现文件
 * @author 
 * @version 1.0
 * @date 2026-02-10
 *
 * 基于RT-Thread rt_list实现的观察者模式具体实现
 */

#include "observer.h"


#define DBG_TAG               "OBSERVER"
#define DBG_LVL               DBG_INFO
#include <rtdbg.h>

/**
 * @brief 初始化被观察者
 */
rt_err_t subject_init(subject_t *subject)
{
    RT_ASSERT(subject != RT_NULL);
    
    /* 初始化观察者链表 */
    rt_list_init(&subject->observer_list);
    
    /* 运行时初始化自旋锁 */
    rt_spin_lock_init(&subject->lock);
    
    LOG_D("Subject initialized");
    return RT_EOK;
}

/**
 * @brief 添加观察者
 */
rt_err_t subject_add_observer(subject_t *subject, observer_t *observer)
{
    rt_base_t level;
    
    RT_ASSERT(subject != RT_NULL);
    RT_ASSERT(observer != RT_NULL);
    RT_ASSERT(observer->update != RT_NULL);
    
    /* 关闭中断并获取自旋锁 */
    level = rt_spin_lock_irqsave(&subject->lock);
    
    /* 检查观察者是否已经存在 */
    rt_list_t *node;
    rt_list_for_each(node, &subject->observer_list)
    {
        observer_t *existing_observer = rt_list_entry(node, observer_t, list);
        if (existing_observer == observer)
        {
            rt_spin_unlock_irqrestore(&subject->lock, level);
            LOG_W("Observer already exists");
            return -RT_ERROR;
        }
    }
    
    /* 添加观察者到链表尾部 */
    rt_list_insert_before(&subject->observer_list, &observer->list);
    
    /* 恢复中断并释放自旋锁 */
    rt_spin_unlock_irqrestore(&subject->lock, level);
    
    LOG_D("Observer added");
    return RT_EOK;
}

/**
 * @brief 移除观察者
 */
rt_err_t subject_remove_observer(subject_t *subject, observer_t *observer)
{
    rt_base_t level;
    rt_err_t result = -RT_ERROR;
    
    RT_ASSERT(subject != RT_NULL);
    RT_ASSERT(observer != RT_NULL);
    
    /* 关闭中断并获取自旋锁 */
    level = rt_spin_lock_irqsave(&subject->lock);
    
    /* 在链表中查找并移除观察者 */
    rt_list_t *node, *temp;
    rt_list_for_each_safe(node, temp, &subject->observer_list)
    {
        observer_t *existing_observer = rt_list_entry(node, observer_t, list);
        if (existing_observer == observer)
        {
            rt_list_remove(node);
            result = RT_EOK;
            break;
        }
    }
    
    /* 恢复中断并释放自旋锁 */
    rt_spin_unlock_irqrestore(&subject->lock, level);
    
    if (result == RT_EOK)
    {
        LOG_D("Observer removed");
    }
    else
    {
        LOG_W("Observer not found");
    }
    
    return result;
}

/**
 * @brief 通知所有观察者
 */
void subject_notify(subject_t *subject, void *data)
{
    rt_base_t level;
    
    RT_ASSERT(subject != RT_NULL);
    
    /* 关闭中断并获取自旋锁 */
    level = rt_spin_lock_irqsave(&subject->lock);
    
    /* 遍历所有观察者并调用更新函数 */
    rt_list_t *node, *temp;
    rt_list_for_each_safe(node, temp, &subject->observer_list)
    {
        observer_t *observer = rt_list_entry(node, observer_t, list);
        if (observer->update != RT_NULL)
        {
            /* 恢复中断后再调用回调函数，避免在中断上下文中执行用户代码 */
            rt_spin_unlock_irqrestore(&subject->lock, level);
            observer->update(observer, subject, data);
            level = rt_spin_lock_irqsave(&subject->lock);
        }
    }
    
    /* 恢复中断并释放自旋锁 */
    rt_spin_unlock_irqrestore(&subject->lock, level);
    
    LOG_D("All observers notified");
}

/**
 * @brief 初始化观察者
 */
rt_err_t observer_init(observer_t *observer, observer_update_func_t update, void *user_data)
{
    RT_ASSERT(observer != RT_NULL);
    RT_ASSERT(update != RT_NULL);
    
    /* 初始化链表节点 */
    rt_list_init(&observer->list);
    
    /* 设置回调函数和用户数据 */
    observer->update = update;
    observer->user_data = user_data;
    
    LOG_D("Observer initialized");
    return RT_EOK;
}

/**
 * @brief 获取观察者数量
 */
int subject_get_observer_count(subject_t *subject)
{
    rt_base_t level;
    int count = 0;
    
    RT_ASSERT(subject != RT_NULL);
    
    /* 关闭中断并获取自旋锁 */
    level = rt_spin_lock_irqsave(&subject->lock);
    
    /* 统计观察者数量 */
    rt_list_t *node;
    rt_list_for_each(node, &subject->observer_list)
    {
        count++;
    }
    
    /* 恢复中断并释放自旋锁 */
    rt_spin_unlock_irqrestore(&subject->lock, level);
    
    return count;
}
