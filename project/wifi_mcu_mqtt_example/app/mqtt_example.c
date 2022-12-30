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

/*
 * 1. This mqtt demo use tencent iot server, please refer this url to set your own mqtt model
 *    (https://console.cloud.tencent.com/);
 * 2. You can use mqtt.fx to verify your mqtt model firstly. 
 *    (https://cloud.tencent.com/document/product/634/14630)
 * 3. Please refer to this url(https://cloud.tencent.com/document/product/634/32546) to get the 
 *    username & password, and note that these data have to be updated; 
 */

#define MQTT_SERVER_IP				"192.168.0.101"
#define MQTT_SERVER_PORT			7788

#define MQTT_CLIENT_ID		"admin"
#define MQTT_CLIENT_USR		"admin"
#define MQTT_CLIENT_PAS		"admin"

#define MQTT_TOPIC_TEST		"$thing/down/property/CN9ON49O9Y/ln_light_01"

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

static char s_rxbuf[512] = {0};

static void 
mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
  LWIP_UNUSED_ARG(data);

  Log_i("MQTT client \"%s\" data cb: len %d, flags %d\n", client_info->client_id,
          (int)len, (int)flags);
	
  memset(s_rxbuf, 0, 512);
  memcpy(s_rxbuf, data, len);
  Log_i("%s", s_rxbuf);
}

static void
mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;

  Log_i("MQTT client \"%s\" publish cb: topic %s, len %d\n", client_info->client_id,
          topic, (int)tot_len);
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
	inet_aton(MQTT_SERVER_IP, &cp);
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
