#include "usr_ble_sys.h"
#include "utils/debug/log.h"

void hex_to_asciistring(uint8_t* str,uint32_t size,uint8_t* str1)
{
	uint8_t deposit [2];
	int i=0;
	uint8_t j = 0;
 
	for(i=0;i<size;i++)
	{
 
		deposit[1] = str[i] & 0x0F;
		deposit[0] = (str[i] &0xF0) >> 4;
		for(j = 0; j < 2; j++)
		{
		switch(deposit[j])
		{
			case 0x00:
			  str1[i*2+j]='0';
				break;
			case 0x01:
			  str1[i*2+j]='1';
				break;
			case 0x02:
			  str1[i*2+j]='2';
				break;
			case 0x03:
			  str1[i*2+j]='3';
				break;
			case 0x04:
			 str1[i*2+j]='4';
				break;
			case 0x05:
			  str1[i*2+j]='5';
				break;
			case 0x06:
			  str1[i*2+j]='6';
				break;
			case 0x07:
			  str1[i*2+j]='7';
				break;
			case 0x08:
			  str1[i*2+j]='8';
				break;
			case 0x09:
			  str1[i*2+j]='9';
				break;
			case 0x0A:
			  str1[i*2+j]='A';
				break;
			case 0x0B:
			  str1[i*2+j]='B';
				break;
			case 0x0C:
			  str1[i*2+j]='C';
				break;
			case 0x0D:
			  str1[i*2+j]='D';
				break;
			case 0x0E:
			  str1[i*2+j]='E';
				break;
			case 0x0F:
			  str1[i*2+j]='F';
				break;
			default:
				return ;
		}
 
	}
	}
   return ;
}

int check_sysparam_ble_name(ble_name_param_t *p_param)
{
	size_t r_len = 0;
	int kvret;
	if (!ln_kv_has_key(KV_SYSPARAM_BLE_NAME)) 
	{
		LOG(LOG_LVL_INFO,"KV_SYSPARAM_NAME is NULL!\r\n");
		return -1;
	}
	else
	{
		kvret=ln_kv_get((const char *)KV_SYSPARAM_BLE_NAME , (void *)(p_param), sizeof(ble_name_param_t),&r_len);
		if(KV_ERR_NONE!=kvret)
		{
			LOG(LOG_LVL_ERROR,"kv get ble name fail return %d\r\n",kvret);
			return -1;
		}
		return 0;
	}
}

//factory reset all param
int ble_param_factory_reset_all(void)
{
	//to do
	int ret = BLE_SYSPARAM_ERR_NONE;
	if (ret != BLE_SYSPARAM_ERR_NONE) {
			return BLE_SYSPARAM_ERR_RESET_ALL;
	}
	return BLE_SYSPARAM_ERR_NONE;

}

//check if master or slave mode
uint8_t check_master_slave(uint8_t *role)
{
	uint8_t ble_role;
	int ret;
	size_t r_len = 0;
	
	if (!ln_kv_has_key(KV_SYSPARAM_BLE_ROLE)) 
	{
		LOG(LOG_LVL_INFO,"ble role has not define yet,system will set to slave automatically!\r\n");
		
		ble_role=BLE_ROLE_SLAVE;
		ret=ln_kv_set(KV_SYSPARAM_BLE_ROLE, (void *)&ble_role, sizeof(uint8_t));
		if(BLE_SYSPARAM_ERR_NONE!=ret)
		{
			LOG(LOG_LVL_INFO,"SYSPARAM_BLE_ROLE set fail...\r\n");
			return -1;
		}
  }
	ret=ln_kv_get((const char *)KV_SYSPARAM_BLE_ROLE , (void *)(&ble_role), sizeof(uint8_t),&r_len);
	if(KV_ERR_NONE!=ret)
	{
		LOG(LOG_LVL_ERROR,"get ble role fail with return %d\r\n",ret);
		return -1;
	}
	
	*role=ble_role;

	return 0;
}



