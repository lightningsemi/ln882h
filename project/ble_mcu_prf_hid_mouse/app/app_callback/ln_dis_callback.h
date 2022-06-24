/**
 ****************************************************************************************
 *
 * @file ln_dis_callback.h
 *
 * @brief  Device Information service callabck funtion 
 *
 *Copyright (C) 2021. Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */

#ifndef _LN_DIS_CALLBACK_H_
#define _LN_DIS_CALLBACK_H_
/**
 ****************************************************************************************
 * @addtogroup APP
 * @group BLE
 *
 * @brief Device Information service APIs.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwprf_config.h"
#include "diss_task.h"
#include "disc_task.h"





/*
 * DEFINES
 ****************************************************************************************
 */
#if (BLE_HID_DEVICE|BLE_HID_BOOT_HOST|BLE_HID_REPORT_HOST)
/// Manufacturer Name Value
#define LN_DIS_MANUFACTURER_NAME       ("Amazon Remote")
#define LN_DIS_MANUFACTURER_NAME_LEN   (13)
#else
/// Manufacturer Name Value
#define LN_DIS_MANUFACTURER_NAME       ("LN Device")
#define LN_DIS_MANUFACTURER_NAME_LEN   (9)
#endif
/// Model Number String Value
#define LN_DIS_MODEL_NB_STR            ("LN-BLE-1.0")
#define LN_DIS_MODEL_NB_STR_LEN        (10)

/// Serial Number
#define LN_DIS_SERIAL_NB_STR           ("1.0.0.0-LE")
#define LN_DIS_SERIAL_NB_STR_LEN       (10)

/// Firmware Revision
#define LN_DIS_FIRM_REV_STR            ("1.0.0")
#define LN_DIS_FIRM_REV_STR_LEN        (5)

/// System ID Value - LSB -> MSB
#define LN_DIS_SYSTEM_ID               ("\x12\x34\x56\xFF\xFE\x9A\xBC\xDE")
#define LN_DIS_SYSTEM_ID_LEN           (8)

/// Hardware Revision String
#define LN_DIS_HARD_REV_STR           ("1.0.0")
#define LN_DIS_HARD_REV_STR_LEN       (5)

/// Software Revision String
#define LN_DIS_SW_REV_STR              ("1.0.0")
#define LN_DIS_SW_REV_STR_LEN          (5)

/// IEEE
#define LN_DIS_IEEE                    ("\xFF\xEE\xDD\xCC\xBB\xAA")
#define LN_DIS_IEEE_LEN                (6)

/**
 * PNP ID Value - LSB -> MSB
 *      Vendor ID Source : 0x02 (USB Implementer Forum assigned Vendor ID value)
 *      Vendor ID : 0x045E      (Microsoft Corp)//for amazon 0x1949
 *      Product ID : 0x0040                     //for amzaon 0x0411
 *      Product Version : 0x0300
 */
#if (BLE_HID_DEVICE|BLE_HID_BOOT_HOST|BLE_HID_REPORT_HOST)
#define LN_DIS_PNP_ID               ("\x02\x49\x19\x11\x04\x00\x01")
#define LN_DIS_PNP_ID_LEN           (7)

#else
#define LN_DIS_PNP_ID               ("\x02\x5E\x04\x40\x00\x00\x03")
#define LN_DIS_PNP_ID_LEN           (7)
#endif


#if (BLE_HID_DEVICE|BLE_HID_BOOT_HOST|BLE_HID_REPORT_HOST)
#define LN_DIS_FEATURES             (DIS_MANUFACTURER_NAME_CHAR_SUP_BIT |\
                                      DIS_MODEL_NB_STR_CHAR_SUP_BIT      |\
                                      DIS_SYSTEM_ID_CHAR_SUP_BIT         |\
                                      DIS_PNP_ID_CHAR_SUP_BIT)

#else
#define LN_DIS_FEATURES             (DIS_ALL_FEAT_SUP)
#endif 




/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */
 struct ln_disc_enable_rsp
 {
     //connect index
     uint8_t conidx;
 };




/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
#if (BLE_DIS_SERVER)
    /*
     * DISS CALLBACK FUNCTIONS
     ****************************************************************************************
     */
int diss_set_value_rsp_handler(ke_msg_id_t const msgid,
                                void const *p_param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id);
    
    
int diss_value_req_ind_handler(ke_msg_id_t const msgid,
                                        void const *p_param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id);
    
    
#endif

#if BLE_DIS_CLIENT
    /*
     * DISC CALLBACK FUNCTIONS
     ****************************************************************************************
     */
int disc_enable_rsp_handler(ke_msg_id_t const msgid,
                                        void const *p_param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id);
    
    
    
int disc_rd_char_ind_handler(ke_msg_id_t const msgid,
                                        void const *p_param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id);
    
    
int disc_cmp_event_handler(ke_msg_id_t const msgid,
                                        void const *p_param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id);
    
     
#endif    //#if (BLE_DIS_CLIENT)
    


#endif //LN_DIS_CALLBACK_H_



