/**
 * @file observer.h
 * @brief 观察者模式头文件
 * @author 
 * @version 1.0
 * @date 2026-02-10
 *
 * 基于RT-Thread rt_list实现的观察者模式
 * 支持添加观察者、移除观察者、通知观察者等功能
 */

#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include <rtthread.h>
#include <rtservice.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 观察者更新回调函数类型
 * 
 * @param observer 观察者指针
 * @param subject 被观察者指针
 * @param data 传递的数据
 */
typedef void (*observer_update_func_t)(void *observer, void *subject, void *data);

/**
 * @brief 观察者结构体
 */
typedef struct observer
{
    rt_list_t list;                     /**< 链表节点，用于串连观察者 */
    observer_update_func_t update;      /**< 更新回调函数 */
    void *user_data;                    /**< 用户自定义数据 */
} observer_t;

/**
 * @brief 被观察者结构体
 */
typedef struct subject
{
    rt_list_t observer_list;            /**< 观察者链表头 */
    struct rt_spinlock lock;            /**< 自旋锁，保护观察者链表 */
} subject_t;

/**
 * @brief 初始化被观察者
 * 
 * @param subject 被观察者对象指针
 * @return rt_err_t RT_EOK表示成功，其他表示失败
 */
rt_err_t subject_init(subject_t *subject);

/**
 * @brief 添加观察者
 * 
 * @param subject 被观察者对象指针
 * @param observer 观察者对象指针
 * @return rt_err_t RT_EOK表示成功，其他表示失败
 */
rt_err_t subject_add_observer(subject_t *subject, observer_t *observer);

/**
 * @brief 移除观察者
 * 
 * @param subject 被观察者对象指针
 * @param observer 观察者对象指针
 * @return rt_err_t RT_EOK表示成功，其他表示失败
 */
rt_err_t subject_remove_observer(subject_t *subject, observer_t *observer);

/**
 * @brief 通知所有观察者
 * 
 * @param subject 被观察者对象指针
 * @param data 传递给观察者的数据
 */
void subject_notify(subject_t *subject, void *data);

/**
 * @brief 初始化观察者
 * 
 * @param observer 观察者对象指针
 * @param update 更新回调函数
 * @param user_data 用户自定义数据
 * @return rt_err_t RT_EOK表示成功，其他表示失败
 */
rt_err_t observer_init(observer_t *observer, observer_update_func_t update, void *user_data);

/**
 * @brief 获取观察者数量
 * 
 * @param subject 被观察者对象指针
 * @return int 观察者数量
 */
int subject_get_observer_count(subject_t *subject);

#ifdef __cplusplus
}
#endif

#endif /* __OBSERVER_H__ */
