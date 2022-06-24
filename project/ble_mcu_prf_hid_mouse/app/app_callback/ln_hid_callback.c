/**
 ****************************************************************************************
 *
 * @file ln_hid_callback.c
 *
 * @brief  Human Interface Device (HID) Profile callabck funtion source code
 *
 *Copyright (C) 2021.Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup BLE
 *
 * @brief  Human Interface Device (HID) Profile callabck funtion source code
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */



#include "rwip_config.h"             // SW configuration
#include "rwprf_config.h"
#include <string.h>
#include "ke_msg.h"
#include "usr_app.h"
#include "usr_ble_app.h"
#if (TRACE_ENABLE)
#include "utils/debug/log.h"
#endif
#include "hogpd_task.h"
#include "ln_app_hid.h"
static const uint8_t hid_mouse_report_map[] = { 
    0x05, 0x01, 
    0x09, 0x02,
    0xA1, 0x01, 
    0x85, 0x01,   // Report Id (1)     
    0x09, 0x01, 
    0xA1, 0x00,
    0x05, 0x09,
    0x19, 0x01, 
    0x29, 0x08,
    0x15, 0x00,
    0x25, 0x01, 
    0x95, 0x08, 
    0x75, 0x01,
    0x81, 0x02,
    0x05, 0x01,
    0x09, 0x30, 
    0x09, 0x31, 
    0x15, 0x81,
    0x25, 0x7F,
    0x75, 0x08, 
    0x95, 0x02, 
    0x81, 0x06, 
    0x09, 0x38, 
    0x15, 0x81, 
    0x25, 0x7F, 
    0x75, 0x08,
    0x95, 0x01, 
    0x81, 0x06,
    0x05, 0x0C,
    0x0A, 0x38,
    0x02, 0x81,
    0x06, 0xC0,
    0xC0, 0x05, 
    0x01, 0x09, 
    0x06, 0xA1,
    0x01, 0x85,
    0x02, 0x05,
    0x07, 0x19, 
    0xE0, 0x29, 
    0xE7, 0x15, 
    0x00, 0x25, 
    0x01, 0x75, 
    0x01, 0x95, 
    0x08, 0x81, 
    0x02, 0x95, 
    0x01, 0x75, 
    0x08, 0x81,
    0x01, 0x95,
    0x06, 0x75,
    0x08, 0x15,
    0x00, 0x25, 0x0F,
    0x05, 0x07, 
    0x19, 0x00, 
    0x29, 0x0F,
    0x81, 0x00, 
    0xC0, 
    0x05, 0x01,
    0x09, 0x80,
    0xA1, 0x01,
    0x85, 0x03, 
    0x15, 0x00, 
    0x25, 0x01, 
    0x75, 0x01, 
    0x95, 0x03,
    0x09, 0x81, 
    0x09, 0x82, 
    0x09, 0x83, 
    0x81, 0x02, 
    0x95, 0x01, 
    0x75, 0x05, 
    0x81, 0x03, 
    0xC0 };
#if 0
report map note:

           Usage Page (Generic Desktop)                 05 01                   
           Usage (Mouse)                                09 02                   
           Collection (Application)                     A1 01                   
           Report Id (1)                                85 01                   
           Usage (Pointer)                              09 01                   
           Collection (Physical)                        A1 00                   
           Usage Page (Button)                          05 09                   
           Usage Minimum (Button 1)                     19 01                   
           Usage Maximum (Button 8)                     29 08                   
           Logical minimum (0)                          15 00                   
           Logical maximum (1)                          25 01                   
           Report Count (8)                             95 08                   
           Report Size (1)                              75 01                   
           Input (Data,Value,Absolute,Bit Field)        81 02                   
           Usage Page (Generic Desktop)                 05 01                   
           Usage (X)                                    09 30                   
           Usage (Y)                                    09 31                   
           Logical minimum (-127)                       15 81               
           Logical maximum (127)                        25 7F              
           Report Size (12)                             75 0C                   
           Report Count (2)                             95 02                   
           Input (Data,Value,Relative,Bit Field)        81 06                   
           Usage (Wheel)                                09 38                   
           Logical minimum (-127)                       15 81                   
           Logical maximum (127)                        25 7F                   
           Report Size (8)                              75 08                   
           Report Count (1)                             95 01                   
           Input (Data,Value,Relative,Bit Field)        81 06                   
           Usage Page (Consumer)                        05 0C                   
           Usage (AC Pan)                               0A 38 02                
           Input (Data,Value,Relative,Bit Field)        81 06                   
       End Collection                                   C0                      
   End Collection                                       C0                      
   Usage Page (Generic Desktop)                         05 01                   
   Usage (Keyboard)                                     09 06                   
   Collection (Application)                             A1 01                   
       Report Id (2)                                    85 02                   
       Usage Page (Keyboard)                            05 07                   
       Usage Minimum (Keyboard Left Control)            19 E0                   
       Usage Maximum (Keyboard Right GUI)               29 E7                   
       Logical minimum (0)                              15 00                   
       Logical maximum (1)                              25 01                   
       Report Size (1)                                  75 01                   
       Report Count (8)                                 95 08                   
       Input (Data,Value,Absolute,Bit Field)            81 02                   
       Report Count (1)                                 95 01                   
       Report Size (8)                                  75 08                   
       Input (Constant,Array,Absolute,Bit Field)        81 01                   
       Report Count (6)                                 95 06                   
       Report Size (8)                                  75 08                   
       Logical minimum (0)                              15 00                   
       Logical maximum (15)                             25 0F                   
       Usage Page (Keyboard)                            05 07                   
       Usage Minimum (No event indicated)               19 00                   
       Usage Maximum (Keyboard l and L)                 29 0F                   
       Input (Data,Array,Absolute,Bit Field)            81 00                   
   End Collection                                       C0                      
   Usage Page (Generic Desktop)                         05 01                   
   Usage (System Control)                               09 80                   
   Collection (Application)                             A1 01                   
       Report Id (3)                                    85 03                   
       Logical minimum (0)                              15 00                   
       Logical maximum (1)                              25 01                   
       Report Size (1)                                  75 01                   
       Report Count (3)                                 95 03                   
       Usage (System Power Down)                        09 81                   
       Usage (System Sleep)                             09 82                   
       Usage (System Wake Up)                           09 83                   
       Input (Data,Value,Absolute,Bit Field)            81 02                   
       Report Count (1)                                 95 01                   
       Report Size (5)                                  75 05                   
       Input (Constant,Value,Absolute,Bit Field)        81 03                   
   End Collection                                       C0                      

#endif



#if 0
// nordic SDK mouse report map
static const uint8_t hid_keyboard_report_map[] = {
        0x05, 0x01, // Usage Page (Generic Desktop)
        0x09, 0x02, // Usage (Mouse)

        0xA1, 0x01, // Collection (Application)

        // Report ID 1: Mouse buttons + scroll/pan
        0x85, 0x01,       // Report Id 1
        0x09, 0x01,       // Usage (Pointer)
        0xA1, 0x00,       // Collection (Physical)
        0x95, 0x05,       // Report Count (3)
        0x75, 0x01,       // Report Size (1)
        0x05, 0x09,       // Usage Page (Buttons)
        0x19, 0x01,       // Usage Minimum (01)
        0x29, 0x05,       // Usage Maximum (05)
        0x15, 0x00,       // Logical Minimum (0)
        0x25, 0x01,       // Logical Maximum (1)
        0x81, 0x02,       // Input (Data, Variable, Absolute)
        0x95, 0x01,       // Report Count (1)
        0x75, 0x03,       // Report Size (3)
        0x81, 0x01,       // Input (Constant) for padding
        0x75, 0x08,       // Report Size (8)
        0x95, 0x01,       // Report Count (1)
        0x05, 0x01,       // Usage Page (Generic Desktop)
        0x09, 0x38,       // Usage (Wheel)
        0x15, 0x81,       // Logical Minimum (-127)
        0x25, 0x7F,       // Logical Maximum (127)
        0x81, 0x06,       // Input (Data, Variable, Relative)
        0x05, 0x0C,       // Usage Page (Consumer)
        0x0A, 0x38, 0x02, // Usage (AC Pan)
        0x95, 0x01,       // Report Count (1)
        0x81, 0x06,       // Input (Data,Value,Relative,Bit Field)
        0xC0,             // End Collection (Physical)

        // Report ID 2: Mouse motion
        0x85, 0x02,       // Report Id 2
        0x09, 0x01,       // Usage (Pointer)
        0xA1, 0x00,       // Collection (Physical)
        0x75, 0x0C,       // Report Size (12)
        0x95, 0x02,       // Report Count (2)
        0x05, 0x01,       // Usage Page (Generic Desktop)
        0x09, 0x30,       // Usage (X)
        0x09, 0x31,       // Usage (Y)
        0x16, 0x01, 0xF8, // Logical maximum (2047)
        0x26, 0xFF, 0x07, // Logical minimum (-2047)
        0x81, 0x06,       // Input (Data, Variable, Relative)
        0xC0,             // End Collection (Physical)
        0xC0,             // End Collection (Application)

        // Report ID 3: Advanced buttons
        0x05, 0x0C,       // Usage Page (Consumer)
        0x09, 0x01,       // Usage (Consumer Control)
        0xA1, 0x01,       // Collection (Application)
        0x85, 0x03,       // Report Id (3)
        0x15, 0x00,       // Logical minimum (0)
        0x25, 0x01,       // Logical maximum (1)
        0x75, 0x01,       // Report Size (1)
        0x95, 0x01,       // Report Count (1)

        0x09, 0xCD,       // Usage (Play/Pause)
        0x81, 0x06,       // Input (Data,Value,Relative,Bit Field)
        0x0A, 0x83, 0x01, // Usage (AL Consumer Control Configuration)
        0x81, 0x06,       // Input (Data,Value,Relative,Bit Field)
        0x09, 0xB5,       // Usage (Scan Next Track)
        0x81, 0x06,       // Input (Data,Value,Relative,Bit Field)
        0x09, 0xB6,       // Usage (Scan Previous Track)
        0x81, 0x06,       // Input (Data,Value,Relative,Bit Field)

        0x09, 0xEA,       // Usage (Volume Down)
        0x81, 0x06,       // Input (Data,Value,Relative,Bit Field)
        0x09, 0xE9,       // Usage (Volume Up)
        0x81, 0x06,       // Input (Data,Value,Relative,Bit Field)
        0x0A, 0x25, 0x02, // Usage (AC Forward)
        0x81, 0x06,       // Input (Data,Value,Relative,Bit Field)
        0x0A, 0x24, 0x02, // Usage (AC Back)
        0x81, 0x06,       // Input (Data,Value,Relative,Bit Field)
        0xC0              // End Collection
    };
#endif


#if 0
static const uint8_t hid_keyboard_report_map[] = {
    0x05, 0x01,
    0x09, 0x02,//mouse
    0xA1, 0x01,  //app
    0x85, 0x01,//report id 1
    0x09, 0x01,//pointer
    0xA1, 0x00,//
    0x05, 0x09, //buttons
    0x19, 0x01, //button 1
    0x29, 0x08, //button 8
    0x15, 0x00,   //0
    0x25, 0x01, //3//1
    0x95, 0x08, //report count 8
    0x75, 0x01, //report size 1
    0x81, 0x02,  //input
    0x05, 0x01,  //desktop
    0x09, 0x30,  //x
    0x09, 0x31,//y
    0x16, 0x01, 0xF8,  // min -2047
    0x26, 0xFF, 0x07, //max 2047
    0x75, 0x0C, //report 12
    0x95, 0x02, //report count 2
    0x81, 0x06, //input
    0x09, 0x38, //wheel 
    0x15, 0x81, //min -127
    0x25, 0x7F,  //max 127
    0x75, 0x08,  //report size 8
    0x95, 0x01,  //report count 1
    0x81, 0x06,  //input
    0x05, 0x0C,  //consumer
    0x0A, 0x38, 
    0x02, 
    0x81, 0x06,//input
    0xC0 
};


#endif




/*
 * DEFINES
 ****************************************************************************************
 */



/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

 //HOGPD CALLBACK FUNCTIONS
#if (BLE_HID_DEVICE)
int hogpd_enable_rsp_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)

{
	struct hogpd_enable_rsp *param = (struct hogpd_enable_rsp *)p_param;
	uint8_t conidx = param->conidx;    
#if (TRACE_ENABLE)
	LOG(LOG_LVL_INFO,"hogpd_enable_rsp_handler,status:0x%x,conidx:0x%x\r\n",param->status,conidx);
#endif
    struct hogpd_enable_rsp *data = blib_malloc(sizeof(struct hogpd_enable_rsp ));
        data->conidx=param->conidx;
        usr_queue_msg_send(BLE_MSG_HOGPD_ENABLE,sizeof(struct hogpd_enable_rsp ),data);
	ke_msg_sync_lock_release();
	return (KE_MSG_CONSUMED);
}

 int hogpd_report_upd_rsp_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
	struct hogpd_report_upd_rsp *param = (struct hogpd_report_upd_rsp *)p_param;
	uint8_t conidx = param->conidx;    
#if (TRACE_ENABLE)
	LOG(LOG_LVL_INFO,"hogpd_report_upd_rsp_handler,status:0x%x,conidx:0x%x\r\n",param->status,conidx);
#endif
	ke_msg_sync_lock_release();
    if( param->status == GAP_ERR_NO_ERROR)
    {
        struct hogpd_report_upd_rsp *data = blib_malloc(sizeof(struct hogpd_report_upd_rsp ));
        data->conidx=param->conidx;
        usr_queue_msg_send(BLE_MSG_HOGPD_REP_UPD_RSP,sizeof(struct hogpd_report_upd_rsp ),data);
    }
	return (KE_MSG_CONSUMED);
}

int hogpd_report_req_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
    struct hogpd_report_req_ind *param = (struct hogpd_report_req_ind *)p_param;
	uint8_t conidx = param->conidx;    
#if (TRACE_ENABLE)
	LOG(LOG_LVL_INFO,"hogpd_report_req_ind_handler,opeation:0x%x,type:0x%x,hid_idx:0x%x,idx:0x%x,length:0x%x\r\n",param->operation,param->report.type,param->report.hid_idx,param->report.idx,param->report.length);
#endif
    if (param->operation == HOGPD_OP_REPORT_READ) {
            if(param->report.type == HOGPD_REPORT)
          {
                struct hogpd_report_cfm *req = blib_malloc(sizeof(struct hogpd_report_cfm) + 8);
                memset(req,0,sizeof(struct hogpd_report_cfm) +8);
                req->report.hid_idx = 0;
        		req->report.length = 8;
        		req->report.idx = 0;
        		req->report.type =  HOGPD_REPORT;//HOGPD_BOOT_MOUSE_INPUT_REPORT;
        		req->report.value[1] =rand()%255; 
                req->report.value[2] = rand()%255;
                req->report.value[3] = rand()%255;
                req->operation = param->operation;
                req->conidx = param->conidx;
            	ln_app_hogpd_report_req_cfm(req);
                blib_free(req);
            }
            if (param->report.type == HOGPD_REPORT_MAP) {
                struct hogpd_report_cfm *req = blib_malloc(sizeof(struct hogpd_report_cfm) + sizeof(hid_mouse_report_map));
                req->conidx = conidx;
                req->status = GAP_ERR_NO_ERROR; 
                 req->operation = param->operation;
                req->report.hid_idx = param->report.hid_idx;
                req->report.type = param->report.type;   
                req->report.idx = param->report.idx;
                req->report.length = sizeof(hid_mouse_report_map);
                 /// Report data
                 memcpy(&req->report.value[0], &hid_mouse_report_map[0], sizeof(hid_mouse_report_map));
                ln_app_hogpd_report_req_cfm(req);
                blib_free(req);
            }
           if(param->report.type == HOGPD_BOOT_KEYBOARD_INPUT_REPORT)
            {
                struct hogpd_report_cfm *req = blib_malloc(sizeof(struct hogpd_report_cfm) + 8);
                req->report.hid_idx = 0;
        		req->report.length = 8;
        		req->report.idx = 0;
        		req->report.type =  HOGPD_BOOT_KEYBOARD_INPUT_REPORT;
        		req->report.value[2] = 00;
                req->operation = param->operation;
                req->conidx = param->conidx;
                ln_app_hogpd_report_req_cfm(req);
                 blib_free(req);
           }
           if(param->report.type == HOGPD_BOOT_MOUSE_INPUT_REPORT)
            {
                
                struct hogpd_report_cfm *req = blib_malloc(sizeof(struct hogpd_report_cfm) + 8);
                req->report.hid_idx = 0;
        		req->report.length = 8;
        		req->report.idx = 0;
        		req->report.type =  HOGPD_BOOT_MOUSE_INPUT_REPORT;
        		req->report.value[1] =rand()%255; 
                req->report.value[2] = rand()%255;
                req->report.value[3] = rand()%255;
                req->operation = param->operation;
                req->conidx = param->conidx;
                ln_app_hogpd_report_req_cfm(req);
                blib_free(req);
           }
        }
    
	return (KE_MSG_CONSUMED);
}

int hogpd_ntf_cfg_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
	 struct hogpd_ntf_cfg_ind *param = (struct hogpd_ntf_cfg_ind *)p_param;
	 uint8_t conidx = param->conidx;    
#if (TRACE_ENABLE)
	 LOG(LOG_LVL_INFO,"hogpd_ntf_cfg_ind_handler,ntf_cfg:0x%x\r\n",param->ntf_cfg[conidx]);
#endif
     struct hogpd_ntf_cfg_ind *data = blib_malloc(sizeof(struct hogpd_ntf_cfg_ind ));
     data->conidx=param->conidx;
     usr_queue_msg_send(BLE_MSG_HOGPD_NTF_CFG_IND,sizeof(struct hogpd_ntf_cfg_ind ),data);
	 return (KE_MSG_CONSUMED);
}

int hogpd_hid_info_req_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
#if (TRACE_ENABLE)
	LOG(LOG_LVL_INFO,"hogpd_hid_info_req_ind_handler\r\n");
#endif
	return (KE_MSG_CONSUMED);
}

int hogpd_ctnl_pt_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
	//struct hogpd_ctnl_pt_ind *param = (struct hogpd_ctnl_pt_ind *)p_param;
#if (TRACE_ENABLE)
	LOG(LOG_LVL_INFO,"hogpd_ctnl_pt_ind_handler\r\n");
#endif
	return (KE_MSG_CONSUMED);
}


#if 0
int hogpd_cmp_evt_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{ 
#if (TRACE_ENABLE)
	LOG(LOG_LVL_INFO,"hogpd_cmp_evt_handler\r\n");
#endif
    return (KE_MSG_CONSUMED);
}

int hogpd_connect_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
	struct hogpd_connect_info *param = (struct hogpd_connect_info *)p_param;  
#if (TRACE_ENABLE)
	LOG(LOG_LVL_INFO,"hogpd_connect_ind_handler,conidx:0x%x\r\n",param->conidx);
#endif
	return (KE_MSG_CONSUMED);
}

int hogpd_disconnect_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
	struct hogpd_connect_info *param = (struct hogpd_connect_info *)p_param;   
#if (TRACE_ENABLE)
	LOG(LOG_LVL_INFO,"hogpd_disconnect_ind_handler,conidx:0x%x,reason:0x%x\r\n",param->conidx,param->reason);
#endif
	return (KE_MSG_CONSUMED);
}

int hogpd_dfu_img_id_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
	
#if (TRACE_ENABLE)
	LOG(LOG_LVL_INFO,"hogpd_dfu_img_id_ind_handler\r\n");
#endif
	
	return (KE_MSG_CONSUMED);
}

#endif



int hogpd_proto_mode_req_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
	struct hogpd_proto_mode_req_ind *param = (struct hogpd_proto_mode_req_ind *)p_param;
	//uint8_t conidx = param->conidx;    
#if (TRACE_ENABLE)
	LOG(LOG_LVL_INFO,"hogpd_proto_mode_reqind_handler\r\n");
#endif
    struct hogpd_proto_mode_cfm cfm;
    cfm.conidx = param->conidx;
    cfm.hid_idx = param->hid_idx;
    cfm.proto_mode = param->proto_mode;
    cfm.status = 0;
    ln_app_hogpd_proto_mode_req_cfm(&cfm);
	return (KE_MSG_CONSUMED);
}



#endif









