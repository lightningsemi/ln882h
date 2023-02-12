/*
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Dirk Ziegelmeier <dziegel@gmx.de>
 *
 */
 

#include "lwip/apps/mqtt.h"
#include "mqtt_example.h"
#include "usr_ctrl.h"

#include "stdbool.h"
#include "lwip/dns.h"
#include "ln_at.h"
#include "ln_utils.h"
#include "netif/ethernetif.h"

/*
* 1. This mqtt demo use "broker-cn.emqx.io:1883", please refer (https://www.emqx.com/zh).
 * 2. You can use mqtt.fx to verify your code. 
 */
 
#define MQTT_RX_BUF_LEN				2048

#define	LN_DEMO_MQTT
//#define	SMART_JC_DEMO
//#define	SMART_JC_LOCAL

#ifdef LN_DEMO_MQTT
#define MQTT_SERVER					"broker-cn.emqx.io"
#define MQTT_SERVER_PORT			1883

#define MQTT_CLIENT_ID				"admin"
#define MQTT_CLIENT_USR				"admin"
#define MQTT_CLIENT_PAS				"admin"
#define MQTT_TOPIC_TEST				"sensor"
#endif

#ifdef SMART_JC_DEMO
#define MQTT_SERVER					"smart.jc-lock.com"
#define MQTT_SERVER_PORT			8883

#define MQTT_CLIENT_ID				"admin"
#define MQTT_CLIENT_USR				"admin"
#define MQTT_CLIENT_PAS				"admin"
#define MQTT_TOPIC_TEST				"stmartlock/uid/receive"
#endif

#ifdef SMART_JC_LOCAL
#define MQTT_SERVER					"192.168.0.187"
#define MQTT_SERVER_PORT			8883

#define MQTT_CLIENT_ID				"admin"
#define MQTT_CLIENT_USR				"admin"
#define MQTT_CLIENT_PAS				"admin"
#define MQTT_TOPIC_TEST				"stmartlock/uid/receive"
#endif

#if LWIP_TCP

/** Define this to a compile-time IP address initialization
 * to connect anything else than IPv4 loopback
 */
#ifndef LWIP_MQTT_EXAMPLE_IPADDR_INIT
#if LWIP_IPV4
#define LWIP_MQTT_EXAMPLE_IPADDR_INIT = IPADDR4_INIT((u32_t)2030086336)
#else
#define LWIP_MQTT_EXAMPLE_IPADDR_INIT
#endif
#endif

static ip_addr_t mqtt_ip LWIP_MQTT_EXAMPLE_IPADDR_INIT;
static mqtt_client_t* mqtt_client;

static const struct mqtt_connect_client_info_t mqtt_client_info =
{
  MQTT_CLIENT_ID,
  MQTT_CLIENT_USR, /* user */
  MQTT_CLIENT_PAS, /* pass */
  100,  /* keep alive */
  NULL, /* will_topic */
  NULL, /* will_msg */
  2,    /* will_qos */
  0     /* will_retain */
#if LWIP_ALTCP && LWIP_ALTCP_TLS
  , NULL
#endif
};

static char s_rxbuf[MQTT_RX_BUF_LEN] = {0};

static void 
mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
  LWIP_UNUSED_ARG(data);

  Log_i("MQTT client \"%s\" data cb: len %d, flags %d\n", client_info->client_id,
          (int)len, (int)flags);
	
  memset(s_rxbuf, 0, MQTT_RX_BUF_LEN);
  memcpy(s_rxbuf, data, len);
  Log_i("%s", s_rxbuf);
  
	ln_at_printf("%d,", len);	
	for(int i = 0; i < len; i++)
	{
		ln_at_printf("%c", s_rxbuf[i]);	
	}
	ln_at_printf("\r\n");	
}

static void
mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;

  Log_i("MQTT client \"%s\" publish cb: topic %s, len %d\n", client_info->client_id,
          topic, (int)tot_len);
	ln_at_printf("MQTTSUBRECV:0,\"%s\",", topic);
}

static void
mqtt_request_cb(void *arg, err_t err)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;

  Log_i("MQTT client \"%s\" request cb: err %d\n", client_info->client_id, (int)err);
}

static void
mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
  LWIP_UNUSED_ARG(client);

  Log_i("MQTT client \"%s\" connection cb: status %d\n", client_info->client_id, (int)status);

  if (status == MQTT_CONNECT_ACCEPTED) {
    mqtt_sub_unsub(client,
            MQTT_TOPIC_TEST, 0,
            mqtt_request_cb, LWIP_CONST_CAST(void*, client_info),
            1);
//    mqtt_sub_unsub(client,
//            "topic_qos0", 0,
//            mqtt_request_cb, LWIP_CONST_CAST(void*, client_info),
//            1);
  }
}
#endif /* LWIP_TCP */

#include "lwip/inet.h"
void
mqtt_example_init(void)
{
#if LWIP_TCP
	ip4_addr_t cp = {0};
	Log_i("MQTT Server: %s", MQTT_SERVER);
	bool bRes = false;
    for (size_t i = 0; i < 5; i++)
    {
        if (dns_gethostbyname(MQTT_SERVER, &(cp), NULL, NULL) == ERR_OK)
        {
            bRes = true;
            break;
        }
        OS_MsDelay(500);
    }
	
	if (!bRes)
	{
		Log_e("dns parse fail!!");
		while(1);
	}

	Log_i("cp:%d", cp.addr);
	
  mqtt_client = mqtt_client_new();

  mqtt_client_connect(mqtt_client,
          &cp, MQTT_SERVER_PORT,
          mqtt_connection_cb, LWIP_CONST_CAST(void*, &mqtt_client_info),
          &mqtt_client_info);
		  
  mqtt_set_inpub_callback(mqtt_client,
          mqtt_incoming_publish_cb,
          mqtt_incoming_data_cb,
          LWIP_CONST_CAST(void*, &mqtt_client_info));
#endif /* LWIP_TCP */
}

ln_at_err_t ln_mqtt_at_mqttpub_set(uint8_t para_num, const char *name)
{
    LN_UNUSED(name);
    Log_i("argc:%d", para_num);

    bool is_default = false;
    uint8_t para_index = 1;
    int int_param = NULL;
	int qos = 0;
	int retain = 0;
	char *pData = NULL;
	char *pTopic = NULL;
	err_t ret = ERR_OK;
	
	//check connect stat
    if(NETDEV_LINK_UP != netdev_get_link_state(netdev_get_active())){
        goto __err;
    }
	
	//linkID
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_int_param(para_index++, &is_default, &int_param))
    {
        goto __err;
    }
	
	if (int_param != 0)
	{
		goto __err;
	}
	
	//topic
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_str_param(para_index++, &is_default, &pTopic))
    {
        goto __err;
    }
	
	if (is_default || !pTopic)
    {
        goto __err;
    }
	
	//data
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_str_param(para_index++, &is_default, &pData))
    {
        goto __err;
    }
	
	if (is_default || !pData)
    {
        goto __err;
    }
	
	//qos
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_int_param(para_index++, &is_default, &qos))
    {
        qos = 0;
    }
	
	//retain
	if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_int_param(para_index++, &is_default, &retain))
    {
        retain = 0;
    }
	
	//pub
	ret = mqtt_publish(mqtt_client, pTopic, pData, strlen(pData), qos, retain, NULL, NULL);
	Log_i("mqtt_publish return: %d", ret);
	
    ln_at_printf(LN_AT_RET_OK_STR);
    return LN_AT_ERR_NONE;

__err:
    ln_at_printf(LN_AT_RET_ERR_STR);
    return LN_AT_ERR_NONE;
}
LN_AT_CMD_REG(MQTTPUB, NULL, ln_mqtt_at_mqttpub_set, NULL, NULL);

