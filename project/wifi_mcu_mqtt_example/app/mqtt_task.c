#include "mqtt_task.h"
#include "utils/debug/ln_assert.h"

static OS_Thread_t g_mqtt_thread;
#define MQTT_APP_TASK_STACK_SIZE          (20*1024) //Byte

static OS_Semaphore_t s_get_ip_sem;
void netdev_get_ip_changed_flag_cb(struct netif *nif)
{
	OS_SemaphoreRelease(&s_get_ip_sem);
}

static void mqtt_task_entry(void *params)
{
	if (OS_SemaphoreCreate(&s_get_ip_sem, 0, 1) != OS_OK)
    {
        Log_e("OS_SemaphoreCreate fail.");
        return;
    }
	
    while (OS_SemaphoreWait(&s_get_ip_sem, 3000) != OS_OK)
    {
		Log_e("wait for ip ... ");
    }
	
	mqtt_example_init();	
	
	while(1)
	{
		
	}
}

void create_mqtt_task(void)
{
	if(OS_OK != OS_ThreadCreate(&g_mqtt_thread, "MqttTask", mqtt_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, MQTT_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }

}	
