
#include "utils/debug/log.h"
#include "utils/ln_psk_calc.h"
#include "utils/system_parameter.h"
#include "utils/ln_list.h"
#include <stdlib.h>
#include <string.h>


#include "ln_at.h"
#include "ln_nvds.h"

#include "usr_app.h"
#include "serial.h"
#include "serial_hw.h"
#include "ln_utils.h"
#include "hal/hal_uart.h"
#include "utils/reboot_trace/reboot_trace.h"
#include "ln_app_gap.h"
#include "ln_app_callback.h"
#include "llm_int.h"
#include "ln_at_cmd_ble.h"
#include "usr_ble_sys.h"
#include "ln_app_gap.h"
#include "ln_kv_key_def.h"
#include "ln_app_gatt.h"
#include "usr_send_data.h"
#include "ble_port.h"


extern struct app_env_info_tag app_env_info;

struct ble_local_phy usr_ble_local_phy;

OS_Semaphore_t app_env_info_sem;

uint8_t gpio_init_flag=0;

uint8_t ble_mac[6];


static ln_at_err_t ln_at_exec_sys_reset(const char *name)
{
    LN_UNUSED(name);
		ln_at_printf("%s %s\r\n",name,LN_AT_BLE_RET_OK_STR);
    ln_chip_reboot();
    return LN_AT_ERR_NONE;
}


static ln_at_err_t ln_at_get_ble_name(const char *name)
{
	uint8_t role;
	int kvret;
	ble_name_param_t ble_name_p;
	char name_str_private[40];
	size_t r_len = 0;

	if(0!=check_master_slave(&role))
	{
		LOG(LOG_LVL_INFO,"get ble role fail...\r\n");
		goto __exit;
	}
	
	if(role==BLE_ROLE_MASTER)			//ble role=master
	{
		ln_at_printf(LN_AT_BLE_MASTER_RET_ERR_STR);
		goto __exit;
	}
	
	memset(&ble_name_p,0,sizeof(ble_name_param_t));
	memset(name_str_private,0,sizeof(name_str_private));
	kvret=ln_kv_get((const char *)KV_SYSPARAM_BLE_NAME , (void *)(&ble_name_p), sizeof(ble_name_param_t),&r_len);
	
	if(KV_ERR_NONE!=kvret)
	{
		if(KV_ERR_NOT_EXIST==kvret)
		{
			memcpy(name_str_private,DEVICE_NAME_DEFAULT,sizeof(DEVICE_NAME_DEFAULT));
		}
		else
		{
			LOG(LOG_LVL_ERROR,"get ble phy name with return %d\r\n",kvret);
			goto __exit;
		}
	}
	else
	{
		memcpy(name_str_private,ble_name_p.ble_name,ble_name_p.ble_name_len);
	}
    
	
	ln_at_printf("\r\n%s:%s\r\n",name,name_str_private);
	
	return LN_AT_ERR_NONE;
   
__exit:

    ln_at_printf(LN_AT_RET_ERR_STR);
    return LN_AT_ERR_COMMON;
}


static ln_at_err_t ln_at_set_ble_name(uint8_t para_num, const char *name)
{
	bool is_default = false;
	uint8_t para_index = 1;
	char *ble_name=NULL;
	int mac;
	uint8_t mac_str[10];
	uint8_t i=0,j=0;
	uint8_t name_len;
	ble_name_param_t ble_name_p;
	
	if(para_num!=2)
	{
		ln_at_printf(LN_AT_BLE_PARA_NUM_ERR_STR);
		goto __exit;
	}
	
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_str_param(para_index++, &is_default, &ble_name))
	{
		goto __exit;
	}
	
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_int_param(para_index++, &is_default, &mac))
	{
		goto __exit;
	}

	if(mac)			//mac=1,add mac last 4 bytes
	{
		name_len=strlen(ble_name);
		name_len=name_len+4;		//add mac 4byte
		if(name_len>16)
		{
			ln_at_printf(LN_AT_BLE_PARA_RANGE_INVALID_STR);
			goto __exit;
		}
	
		sprintf((char *)mac_str, "%02X%02X", *(ble_mac+4),*(ble_mac+5));
		memset(&ble_name_p,0,sizeof(ble_name_param_t));
		ble_name=strcat(ble_name,(char *)mac_str);
		memcpy(ble_name_p.ble_name,ble_name,name_len+1);			
		ble_name_p.ble_name_len=name_len;
		int kvret=ln_kv_set((const char *)KV_SYSPARAM_BLE_NAME , (void *)(&ble_name_p), sizeof(ble_name_param_t));
		if(KV_ERR_NONE!=kvret)
		{
			LOG(LOG_LVL_ERROR,"set ble phy fail with return %d\r\n",kvret);
			goto __exit;
		}				
		ln_at_printf("\r\n%s %s\r\n",name,LN_AT_BLE_RET_OK_STR);
	
		return LN_AT_ERR_NONE;		
	}
	else
	{
		name_len=strlen(ble_name);
		if(name_len>16)
		{
			ln_at_printf(LN_AT_BLE_PARA_RANGE_INVALID_STR);
			goto __exit;
		}
		
    memset(&ble_name_p,0,sizeof(ble_name_param_t));
		
		memcpy(ble_name_p.ble_name,ble_name,name_len+1);
		ble_name_p.ble_name_len=name_len;

		int kvret=ln_kv_set((const char *)KV_SYSPARAM_BLE_NAME , (void *)(&ble_name_p), sizeof(ble_name_param_t));
		if(KV_ERR_NONE!=kvret)
		{
			LOG(LOG_LVL_ERROR,"set ble phy fail with return %d\r\n",kvret);
			goto __exit;
		}				
	}
	
	ln_at_printf("\r\n%s %s\r\n",name,LN_AT_BLE_RET_OK_STR);
	
	return LN_AT_ERR_NONE;
	
__exit:
	
    ln_at_printf(LN_AT_RET_ERR_STR);
    return LN_AT_ERR_COMMON;
    
}


//set uart1 baudrate
static ln_at_err_t ln_at_set_baudrate(uint8_t para_num, const char *name)
{	
	bool is_default = false;
	uint8_t para_index = 1;
	
	int baudrate=0;

	if(para_num!=1)
	{
		goto __exit;
	}
	
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_int_param(para_index++, &is_default, &baudrate))
	{
		goto __exit;
	}
	
	if((baudrate!=9600)&&(baudrate!=19200)&&(baudrate!=57600)&&(baudrate!=115200))
	{
		ln_at_printf("+ERROR:%s\r\n", LN_AT_BLE_GENERAL_PARA_INVALID_STR);
	}
	else
	{
		ln_at_printf("\r\n%s %s\r\n", name,LN_AT_BLE_RET_OK_STR);
		hal_uart_baudrate_set(UART1_BASE, baudrate);
	}
	
  return LN_AT_ERR_NONE;

	
__exit:
    ln_at_printf(LN_AT_RET_ERR_STR);
    return LN_AT_ERR_COMMON;	
}

//restore para
static ln_at_err_t ln_at_exec_para_restore(const char *name)
{
    //to do
    ln_at_printf("\r\n%s %s\r\n", name,LN_AT_BLE_RET_OK_STR);
    return LN_AT_ERR_NONE;
}

//get ble phy mode
static ln_at_err_t ln_at_get_ble_phy_mode(const char *name)
{
	/*
	struct ln_gapc_get_info_cmd get_info;
	
	int kvret;
	size_t r_len = 0;
	int8_t tx_p=0;
	int8_t rx_p=0;
	get_info.operation=GET_PEER_NAME;
  ln_app_get_peer_info(0,&get_info);
	ln_at_printf("\r\n%s %s\r\n",name,LN_AT_BLE_RET_OK_STR);
	kvret=ln_kv_get((const char *)KV_SYSPARAM_BLE_PHY , (void *)(&usr_ble_local_phy), sizeof(usr_ble_local_phy),&r_len);
	if(KV_ERR_NONE!=kvret)
	{
		LOG(LOG_LVL_ERROR,"get ble phy fail with return %d\r\n",kvret);
	}
	tx_p=usr_ble_local_phy.tx_phy;
	rx_p=usr_ble_local_phy.rx_phy;
	
	LOG(LOG_LVL_TRACE,"get tx phy=%d\r\n",tx_p);
	LOG(LOG_LVL_TRACE,"get rx phy=%d\r\n",rx_p);
	

	return LN_AT_ERR_NONE;
	*/
	int8_t phy=0;
	struct ln_gapc_get_info_cmd get_info;
	struct ln_gapc_connection_req_info *p_tag=NULL;
	int conidx=0;
	
	memcpy(p_tag,&(app_env_info.conn_req_info),sizeof(struct ln_gapc_connection_req_info));
	
	if(0!=check_ble_connected())
	{
		ln_at_printf(LN_AT_BLE_CMD_ONLY_SUPPORT_CONNECTED_STR);
		goto __exit;
	}
	
	get_info.operation=GET_PHY;
	
	conidx=p_tag->conidx;

	ln_app_get_peer_info(conidx,&get_info);
	
	if(OS_OK!=OS_SemaphoreCreateBinary(&app_env_info_sem))
	{
		LOG(LOG_LVL_INFO,"OS_SemaphoreCreateBinary app_env_sem fail.\r\n");
	}

	OS_SemaphoreWait(&app_env_info_sem,5);

	phy=app_env_info.le_phy_info.tx_phy;
	
	OS_SemaphoreDelete(&app_env_info_sem);
	
	ln_at_printf("\r\n%s:%d\r\n",name,phy);
	
	return LN_AT_ERR_NONE;

__exit:
	ln_at_printf(LN_AT_RET_ERR_STR);
	return LN_AT_ERR_COMMON;	

}

//set ble phy mode
static ln_at_err_t ln_at_set_ble_phy_mode(uint8_t para_num,const char*name)
{
	bool is_default = false;
	uint8_t para_index = 1;
	int phy=0;
	if(para_num!=1)
	{
		ln_at_printf(LN_AT_BLE_PARA_NUM_ERR_STR);
		goto __exit;		
	}
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_int_param(para_index++, &is_default, &phy))
	{
		goto __exit;
	}
	
	if(phy!=1&&phy!=2)
	{
		ln_at_printf(LN_AT_BLE_PARA_RANGE_INVALID_STR);
		goto __exit;
	}
	
	if(0!=check_ble_connected())
	{
		ln_at_printf(LN_AT_BLE_CMD_ONLY_SUPPORT_CONNECTED_STR);
		goto __exit;
	}
		
	ln_app_set_phy(app_env_info.conn_req_info.conidx,phy,phy,GAPC_PHY_OPT_LE_CODED_ALL_RATES);
	
	ln_at_printf("\r\n%s %s\r\n",name,LN_AT_BLE_RET_OK_STR);
	
	return LN_AT_ERR_NONE;

__exit:
	
	ln_at_printf(LN_AT_RET_ERR_STR);
	return LN_AT_ERR_COMMON;	
}

static ln_at_err_t ln_at_get_gpio_out(const char*name)
{
	uint8_t output=0;
	if(!gpio_init_flag)
	{
		ln_at_printf("gpio do not set,please use set GPIOOUT first..\r\n");
		goto __exit;
	}
	
	for(uint8_t i=0;i<4;i++)
	{
		output=hal_gpio_pin_output_read(BLE_MODULE_GPIO_BASE,(BLE_MODULE_GPIO_0<<i));
		ln_at_printf("%s: %d,%d,%s\r\n",name,i,output,LN_AT_BLE_RET_OK_STR);
	}
	return LN_AT_ERR_NONE;
__exit:
	
	ln_at_printf(LN_AT_RET_ERR_STR);
	return LN_AT_ERR_COMMON;	
	
}

static ln_at_err_t ln_at_set_gpio_out(uint8_t para_num,const char*name)
{
	bool is_default = false;
	uint8_t para_index = 1;
		
	int gpio_pin=0;
	int output=0;
	
	if(para_num!=2)
	{
		ln_at_printf(LN_AT_BLE_PARA_NUM_ERR_STR);
		goto __exit;		
	}
	
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_int_param(para_index++, &is_default, &gpio_pin))
	{
		goto __exit;
	}
	
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_int_param(para_index++, &is_default, &output))
	{
		goto __exit;
	}
	
	if((gpio_pin!=0&&gpio_pin!=1&&gpio_pin!=2&&gpio_pin!=3)||(output!=0&&output!=1))
	{
		ln_at_printf(LN_AT_BLE_PARA_RANGE_INVALID_STR);
		goto __exit;
	}

	if(!gpio_init_flag)
	{
		gpio_init_t_def gpio_def;
		
		gpio_def.dir=GPIO_OUTPUT;
		gpio_def.mode=GPIO_MODE_DIGITAL;
		gpio_def.pull=GPIO_PULL_UP;
		gpio_def.speed=GPIO_HIGH_SPEED;
		gpio_def.pin=BLE_MODULE_GPIO_0|BLE_MODULE_GPIO_1|BLE_MODULE_GPIO_2|BLE_MODULE_GPIO_3; 
		
		hal_gpio_init(BLE_MODULE_GPIO_BASE, &gpio_def);
	}
	
	gpio_pin=BLE_MODULE_GPIO_0<<gpio_pin;	//gpio base:B3
	
	if(output)			//output 1
	{
		hal_gpio_pin_set(BLE_MODULE_GPIO_BASE,gpio_pin);
	}
	else
	{
		hal_gpio_pin_reset(BLE_MODULE_GPIO_BASE,gpio_pin);
	}
	
	gpio_init_flag=1;
	
	ln_at_printf("\r\n%s %s\r\n",name,LN_AT_BLE_RET_OK_STR);
	return LN_AT_ERR_NONE;	
	
__exit:
	
	ln_at_printf(LN_AT_RET_ERR_STR);
	return LN_AT_ERR_COMMON;	
	
}

static ln_at_err_t ln_at_get_ble_mac(const char*name)
{
	ln_at_printf("\r\n%s:%02X%02X%02X%02X%02X%02X %s\r\n",name,ble_mac[0],ble_mac[1],ble_mac[2],\
								ble_mac[3],ble_mac[4],ble_mac[5],LN_AT_BLE_RET_OK_STR);
	return LN_AT_ERR_NONE;	
}

static ln_at_err_t ln_at_set_ble_txpower(uint8_t para_num,const char*name)
{
	bool is_default = false;
	uint8_t para_index = 1;

	if(para_num!=1)
	{
		ln_at_printf(LN_AT_BLE_PARA_NUM_ERR_STR);
		goto __exit;		
	}

	return LN_AT_ERR_NONE;
	
__exit:
	
	ln_at_printf(LN_AT_RET_ERR_STR);
	return LN_AT_ERR_COMMON;			
}

static ln_at_err_t ln_at_get_ble_txpower(const char*name)
{
	int max_power=0;
	ln_app_get_dev_info(GET_INFO_ADV_TX_POWER);
	if(OS_OK!=OS_SemaphoreCreateBinary(&app_env_info_sem))
	{
		LOG(LOG_LVL_INFO,"OS_SemaphoreCreateBinary app_env_sem fail.\r\n");
	}

	OS_SemaphoreWait(&app_env_info_sem,5);
	
	max_power=app_env_info.actv_creat_info.tx_pwr;
	//min_power=app_env_info.dev_tx_pwr.min_tx_pwr;
	
	OS_SemaphoreDelete(&app_env_info_sem);
	
	ln_at_printf("max pwr:%d,min pwr:%d\r\n",max_power);
	
	return LN_AT_ERR_NONE;
}

//set ble notify
static ln_at_err_t ln_at_set_ble_notify(uint8_t para_num,const char*name)
{
	bool is_default = false;
	uint8_t para_index = 1;
	char* mac_str=NULL;
	int len=0;
	char* data=NULL;
	uint8_t role=0;
	int conidx=0;
	
	struct ln_gattc_send_evt_cmd send_data;
	struct gatt_notify_data *p_data=blib_malloc(sizeof(struct gatt_notify_data));
	
	if(para_num!=3)
	{
		ln_at_printf(LN_AT_BLE_PARA_NUM_ERR_STR);
		goto __exit;
	}
	if(0!=check_ble_connected())
	{
		ln_at_printf(LN_AT_BLE_CMD_ONLY_SUPPORT_CONNECTED_STR);
		goto __exit;
	}
	if(0!=check_master_slave(&role))
	{
		LOG(LOG_LVL_INFO,"get ble role fail...\r\n");
		goto __exit;
	}
	
	if(role==BLE_ROLE_MASTER)			//ble role=master
	{
		ln_at_printf(LN_AT_BLE_MASTER_RET_ERR_STR);
		goto __exit;
	}
	
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_str_param(para_index++, &is_default, &mac_str))
	{
		goto __exit;
	}
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_int_param(para_index++, &is_default, &len))
	{
		goto __exit;
	}
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_str_param(para_index++, &is_default, &data))
	{
		goto __exit;
	}
	
	struct ln_gapc_connection_req_info *p_tag=blib_malloc(sizeof(struct ln_gapc_connection_req_info));
	
	memset(p_data,0,sizeof(struct gatt_notify_data));
	
	memcpy(p_data->data,(uint8_t *)data,strlen(data));
	p_data->data_len=len;

	memcpy(p_tag,&(app_env_info.conn_req_info),sizeof(struct ln_gapc_connection_req_info));
	conidx=p_tag->conidx;
	send_data.handle = 0x10 + HANDLE_DATA_TRANS_CHAR_VAL_TX;
	send_data.length = p_data->data_len;
	
	memcpy(send_data.value,p_data->data,len);
	
	ln_app_gatt_send_ntf(conidx,&send_data);
	
	blib_free(p_tag);
	blib_free(p_data);
	
	ln_at_printf("\r\n%s %s\r\n",name,LN_AT_BLE_RET_OK_STR);
	
	return LN_AT_ERR_NONE;		
	
__exit:
	ln_at_printf(LN_AT_RET_ERR_STR);
	return LN_AT_ERR_COMMON;			
	
}

/*
*General commands
*/

//AT   return +OK
//LN_AT_CMD_REG(AT,NULL,NULL,NULL,NULL);

//AT+BAUDRATE=<p>    set
//p=9600,19200,38400,57600,115200·
//return +BAUDRATE OK
LN_AT_CMD_REG(BAUDRATE,NULL,ln_at_set_baudrate,NULL,NULL);

//AT+RESET    exec
//return +RESET OK
LN_AT_CMD_REG(RESET,NULL,NULL,NULL,ln_at_exec_sys_reset);

//AT+RESTORE    exec
//return +RESTORE OK
LN_AT_CMD_REG(RESTORE,NULL,NULL,NULL,ln_at_exec_para_restore);

//AT+BTPHYMODE?    get
//AT+BTPHYMODE=<p>    set
//p=1,2    1=1M    2=2M
//get return +BTPHYMODE OK
//set return +BTPHYMODE:1 OK
LN_AT_CMD_REG(BTPHYMODE,ln_at_get_ble_phy_mode,ln_at_set_ble_phy_mode,NULL,NULL);

//AT+GPIOOUT?    get
//AT+GPIOOUT=<p1>,<p2>    set
//p1=0,1,2,3(pin0~pin3)  p2=0,1(output 0 or 1)
//get return +GPIOOUT:0,0,OK
//set return +GPIOOUT OK
LN_AT_CMD_REG(GPIOOUT,ln_at_get_gpio_out,ln_at_set_gpio_out,NULL,NULL);

//AT+BTMAC? get
//get return +BTMAC:AB1253365113 OK
LN_AT_CMD_REG(BTMAC,ln_at_get_ble_mac,NULL,NULL,NULL);

//AT+TXPOWER? get
//AT+TXPOWER=<p1> set
LN_AT_CMD_REG(TXPOWER,ln_at_get_ble_txpower,NULL,NULL,NULL);



//Slave/Server CMD
LN_AT_CMD_REG(NAME,ln_at_get_ble_name,ln_at_set_ble_name,NULL,NULL);

//Slave notify
LN_AT_CMD_REG(NOTIFY,NULL,ln_at_set_ble_notify,NULL,NULL);

//Slave Receive
//to do
LN_AT_CMD_REG(RECEIVE,NULL,NULL,NULL,NULL);














