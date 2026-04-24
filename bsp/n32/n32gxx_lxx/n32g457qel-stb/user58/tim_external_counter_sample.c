
#ifdef BSP_USING_TIM_EXT_COUNTER_SAMPLE // demo
#define ETR_DEV_NAME "eclktm1"

static rt_mailbox_t mb_etr;
static void timout_etr(void *parameter)
{
    uint32_t count = 0;
    rt_device_t etr_dev = (rt_dev_t)parameter;
    rt_device_read(etr_dev, 0, &count, sizeof(count));
    rt_mb_send_wait(mb_etr, count, 0);
}
static void thd_entry_etr(void *parameter)
{
    uint32_t value = 0;
    while (1)
    {
        if (RT_EOK == rt_mb_recv(mb_etr, &value, RT_WAITING_FOREVER))
        {
            LOG_I("ETR CNT %d", value);
        }
    }
}
static void extr_app_demo(void)
{
    static rt_device_t etr_dev = RT_NULL;
    etr_dev = rt_device_find(ETR_DEV_NAME);
    if (etr_dev)
    {
        rt_device_open(etr_dev, 0);
    }
    else
    {
        LOG_W("ETR device not find");
    }
    mb_etr = rt_mb_create(ETR_DEV_NAME, 5, RT_IPC_FLAG_FIFO);
    RT_ASSERT(mb_etr);
    rt_thread_t thd_etr = rt_thread_create(ETR_DEV_NAME, thd_entry_etr, etr_dev, 512, 20, 5);
    RT_ASSERT(thd_etr);
    rt_thread_startup(thd_etr);
    rt_timer_t timer = rt_timer_create(ETR_DEV_NAME, timout_etr, etr_dev, 1000, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_HARD_TIMER);
    RT_ASSERT(timer);
    rt_timer_start(timer);
}
INIT_APP_EXPORT(extr_app_demo);
#endif /* demo */