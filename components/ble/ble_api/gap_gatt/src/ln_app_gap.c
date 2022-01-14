/**
 ****************************************************************************************
 *
 * @file ln_app_gap.c
 *
 * @brief GAP APIs source code
 *
 *Copyright (C) 2021. Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @addtogroup APP
 * @group BLE
 *
 * @brief GAP APIs source code
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ln_app_gap.h"
#include <string.h>
#include "gapm_task.h"               // GAP Manallger Task API
#include "gapc_task.h"               // GAP Controller Task API
#include "ble_port.h"
/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void ln_app_gapm_reset(void)
{
    // Reset the stack
    struct gapm_reset_cmd *p_cmd = KE_MSG_ALLOC(GAPM_RESET_CMD,
                                   TASK_GAPM, TASK_APP,
                                   gapm_reset_cmd);
    p_cmd->operation = GAPM_RESET;
    ln_ke_msg_send(p_cmd);
}

void ln_app_set_dev_config(struct ln_gapm_set_dev_config_cmd *cfg_param)
{
    struct gapm_set_dev_config_cmd *p_cmd = KE_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD,
                                            TASK_GAPM, TASK_APP,
                                            gapm_set_dev_config_cmd);
    p_cmd->operation = GAPM_SET_DEV_CONFIG;
    p_cmd->role = cfg_param->role;
    p_cmd->renew_dur = cfg_param->renew_dur;
    memcpy(p_cmd->addr.addr, cfg_param->addr.addr, GAP_BD_ADDR_LEN);
    memcpy(p_cmd->irk.key, cfg_param->irk.key, GAP_KEY_LEN);
    p_cmd->privacy_cfg = cfg_param->privacy_cfg;
    p_cmd->pairing_mode = cfg_param->pairing_mode;
    p_cmd->gap_start_hdl = cfg_param->gap_start_hdl;
    p_cmd->gatt_start_hdl = cfg_param->gatt_start_hdl;
    p_cmd->att_cfg = (cfg_param->att_devname_write_perm & 0x7) |
                     ((cfg_param->att_apperance_write_perm & 0x7) << 3) |
                     ((cfg_param->att_slv_pref_conn_param_present & 0x1) << 6) |
                     ((cfg_param->svc_change_feat_present & 0x1) << 7);
    p_cmd->sugg_max_tx_octets = cfg_param->sugg_max_tx_octets;
    p_cmd->sugg_max_tx_time = cfg_param->sugg_max_tx_time;
    p_cmd->max_mtu = cfg_param->max_mtu;
    p_cmd->max_mps = cfg_param->max_mps;
    p_cmd->max_nb_lecb = cfg_param->max_nb_lecb;
    p_cmd->tx_pref_phy = cfg_param->tx_pref_phy;
    p_cmd->rx_pref_phy = cfg_param->rx_pref_phy;
    p_cmd->tx_path_comp = cfg_param->tx_path_comp;
    p_cmd->rx_path_comp = cfg_param->rx_path_comp;
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_get_dev_info(int cmd)
{
    int operation;
    switch (cmd)
    {
        case GET_INFO_VERSION:
        operation = GAPM_GET_DEV_VERSION;
        break;
        case GET_INFO_BDADDR:
        operation = GAPM_GET_DEV_BDADDR;
        break; 
        case GET_INFO_ADV_TX_POWER:
        operation = GAPM_GET_DEV_ADV_TX_POWER;
        break;
        case GET_INFO_TX_POWER:
        operation = GAPM_GET_DEV_TX_PWR;
        break;
        case GET_INFO_SUGG_DFT_DATA:
        operation = GAPM_GET_SUGGESTED_DFLT_LE_DATA_LEN;
        break;
        case GET_INFO_SUPP_DFT_DATA:
        operation = GAPM_GET_MAX_LE_DATA_LEN;
        break;
        case GET_INFO_MAX_ADV_DATA_LEN:
        operation = GAPM_GET_MAX_LE_ADV_DATA_LEN;
        break;
        case GET_INFO_WLIST_SIZE:
        operation = GAPM_GET_WLIST_SIZE;
        break;
        case GET_INFO_RAL_SIZE:
        operation = GAPM_GET_RAL_SIZE;
        break;
        case GET_INFO_PAL_SIZE:
        operation = GAPM_GET_PAL_SIZE;
        break;
        case GET_INFO_NB_ADV_SETS:
        operation = GAPM_GET_NB_ADV_SETS;
        break;
        default:
            break;
    }
    struct gapm_get_dev_info_cmd *p_cmd = KE_MSG_ALLOC(GAPM_GET_DEV_INFO_CMD,
                                          TASK_GAPM, TASK_APP,
                                          gapm_get_dev_info_cmd);
    p_cmd->operation = operation;
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_set_chn_map(struct ln_gapm_set_channel_map_cmd *p_chn_map)
{
    struct gapm_set_channel_map_cmd *p_cmd = KE_MSG_ALLOC( GAPM_SET_CHANNEL_MAP_CMD,
                                                TASK_GAPM, TASK_APP,
                                                gapm_set_channel_map_cmd);
    p_cmd->operation = GAPM_SET_CHANNEL_MAP;
    memcpy(p_cmd->chmap.map, p_chn_map->chmap.map, GAP_LE_CHNL_MAP_LEN);
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_resolve_addr(struct ln_gapm_resolv_addr_cmd *resolv_addr)
{
    struct gapm_resolv_addr_cmd *p_cmd = KE_MSG_ALLOC_DYN( GAPM_RESOLV_ADDR_CMD,
                                         TASK_GAPM, TASK_APP,
                                         gapm_resolv_addr_cmd, (resolv_addr->nb_key * sizeof(struct gap_sec_key)));
    p_cmd->operation = GAPM_RESOLV_ADDR;
    p_cmd->nb_key = resolv_addr->nb_key;
    memcpy(p_cmd->addr.addr, resolv_addr->addr.addr, GAP_BD_ADDR_LEN);
    for (uint32_t i = 0; i < resolv_addr->nb_key; i++)
    {
        memcpy(p_cmd->irk[i].key, resolv_addr->irk[i].key, GAP_KEY_LEN);
    }
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_gen_random_addr(struct ln_gapm_gen_rand_addr_cmd *p_param)
{
    struct gapm_gen_rand_addr_cmd *p_cmd = KE_MSG_ALLOC(GAPM_GEN_RAND_ADDR_CMD,
                                           TASK_GAPM, TASK_APP,
                                           gapm_gen_rand_addr_cmd);
    p_cmd->operation = GAPM_GEN_RAND_ADDR;
    p_cmd->rnd_type = p_param->rnd_type;
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_get_pub_key(void)
{
    struct gapm_get_pub_key_cmd *p_cmd = KE_MSG_ALLOC(GAPM_GET_PUB_KEY_CMD,
                                         TASK_GAPM, TASK_APP,
                                         gapm_get_pub_key_cmd);
    p_cmd->operation = GAPM_GET_PUB_KEY;
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_gen_random_nb(void)
{
    struct gapm_gen_rand_nb_cmd *p_cmd = KE_MSG_ALLOC(GAPM_GEN_RAND_NB_CMD,
                                         TASK_GAPM, TASK_APP,
                                         gapm_gen_rand_nb_cmd);
    p_cmd->operation = GAPM_GEN_RAND_NB;
    // Send the message
    ln_ke_msg_send(p_cmd);
}


void ln_app_set_irk(struct ln_gapm_set_irk_cmd *p_param)
{
    struct gapm_set_irk_cmd *p_cmd = KE_MSG_ALLOC(GAPM_SET_IRK_CMD,
                                     TASK_GAPM, TASK_APP,
                                     gapm_set_irk_cmd);
    p_cmd->operation = GAPM_SET_IRK;
    memcpy(p_cmd->irk.key, p_param->irk.key, GAP_KEY_LEN);
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_register_lepsm(struct ln_gapm_lepsm_register_cmd *p_param)
{
    struct gapm_lepsm_register_cmd *p_cmd = KE_MSG_ALLOC( GAPM_LEPSM_REGISTER_CMD,
                                            TASK_GAPM, TASK_APP,
                                            gapm_lepsm_register_cmd);
    p_cmd->operation = GAPM_LEPSM_REG;
    p_cmd->le_psm = p_param->le_psm;
    p_cmd->app_task = TASK_APP;
    p_cmd->sec_lvl = p_param->sec_lvl;
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_unregister_lepsm(struct ln_gapm_lepsm_unregister_cmd *p_param)
{
    struct gapm_lepsm_unregister_cmd *p_cmd = KE_MSG_ALLOC( GAPM_LEPSM_UNREGISTER_CMD,
            TASK_GAPM, TASK_APP,
            gapm_lepsm_unregister_cmd);
    p_cmd->operation = GAPM_LEPSM_UNREG;
    p_cmd->le_psm = p_param->le_psm;
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_get_ral_addr(struct ln_gapm_get_ral_addr_cmd *p_param)
{
    struct gapm_get_ral_addr_cmd *p_cmd = KE_MSG_ALLOC( GAPM_GET_RAL_ADDR_CMD,
                                          TASK_GAPM, TASK_APP,
                                          gapm_get_ral_addr_cmd);
    p_cmd->operation =p_param->operation;
    p_cmd->peer_identity.addr_type = p_param->peer_identity.addr_type;
    memcpy(p_cmd->peer_identity.addr.addr, p_param->peer_identity.addr.addr, GAP_BD_ADDR_LEN);
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_set_ral_list(struct ln_gapm_list_set_ral_cmd *p_param)
{
    struct gapm_list_set_ral_cmd *p_cmd = KE_MSG_ALLOC_DYN(GAPM_LIST_SET_CMD,
                                          TASK_GAPM, TASK_APP,
                                          gapm_list_set_ral_cmd, (p_param->size * sizeof(struct gap_ral_dev_info)));
    p_cmd->operation = GAPM_SET_RAL;
    p_cmd->size = p_param->size;
    for (uint32_t i = 0; i < p_param->size; i++)
    {
        p_cmd->ral_info[i].addr.addr_type = p_param->ral_info[i].addr.addr_type;
        memcpy(p_cmd->ral_info[i].addr.addr.addr, p_param->ral_info[i].addr.addr.addr, GAP_BD_ADDR_LEN);
        p_cmd->ral_info[i].priv_mode = p_param->ral_info[0].priv_mode;
        memcpy(p_cmd->ral_info[i].peer_irk, p_param->ral_info[i].peer_irk, GAP_KEY_LEN);
        memcpy(p_cmd->ral_info[i].local_irk, p_param->ral_info[i].local_irk, GAP_KEY_LEN);
    }
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_set_pal_list(struct ln_gapm_list_set_pal_cmd *p_param)
{
    struct gapm_list_set_pal_cmd *p_cmd = KE_MSG_ALLOC_DYN(GAPM_LIST_SET_CMD,
                                          TASK_GAPM, TASK_APP,
                                          gapm_list_set_pal_cmd, (p_param->size * sizeof(struct gapm_period_adv_addr_cfg)));
    p_cmd->operation = GAPM_SET_PAL;
    p_cmd->size = p_param->size;
    for (uint32_t i = 0; i < p_param->size; i++)
    {
        p_cmd->pal_info[i].addr.addr_type = p_param->pal_info[i].addr.addr_type;
        memcpy(p_cmd->pal_info[i].addr.addr.addr, p_param->pal_info[i].addr.addr.addr, GAP_BD_ADDR_LEN);
        p_cmd->pal_info[i].adv_sid = p_param->pal_info[i].adv_sid;
    }
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_set_white_list(struct ln_gapm_list_set_wl_cmd *wlist)
{
    struct gapm_list_set_wl_cmd *p_cmd = KE_MSG_ALLOC_DYN( GAPM_LIST_SET_CMD,
                                         TASK_GAPM, TASK_APP,
                                         gapm_list_set_wl_cmd, (wlist->size * sizeof(struct gap_bdaddr)));
    p_cmd->operation = GAPM_SET_WL;
    p_cmd->size = wlist->size;
    for (uint32_t i = 0; i < wlist->size; i++)
    {
        p_cmd->wl_info[i].addr_type = wlist->wl_info[i].addr_type;
        memcpy(p_cmd->wl_info[i].addr.addr, wlist->wl_info[i].addr.addr, BD_ADDR_LEN);
    }
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_advertise_creat(struct ln_gapm_activity_create_adv_cmd * adv_creat_param)
{
    // Prepare the GAPM_ACTIVITY_CREATE_CMD message
    struct gapm_activity_create_adv_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_CREATE_CMD,
                                                    TASK_GAPM, TASK_APP,
                                                    gapm_activity_create_adv_cmd);
    // Set operation code
    p_cmd->operation = GAPM_CREATE_ADV_ACTIVITY;
    // Fill the allocated kernel message
    p_cmd->own_addr_type = adv_creat_param->own_addr_type;
    p_cmd->adv_param.type = adv_creat_param->adv_param.type;
    p_cmd->adv_param.disc_mode = adv_creat_param->adv_param.disc_mode;
    p_cmd->adv_param.prop = adv_creat_param->adv_param.prop;
    p_cmd->adv_param.max_tx_pwr = adv_creat_param->adv_param.max_tx_pwr;
    p_cmd->adv_param.filter_pol = adv_creat_param->adv_param.filter_pol;
    p_cmd->adv_param.peer_addr.addr_type = adv_creat_param->adv_param.peer_addr.addr_type;
    memcpy(p_cmd->adv_param.peer_addr.addr.addr, adv_creat_param->adv_param.peer_addr.addr.addr, GAP_BD_ADDR_LEN);
    p_cmd->adv_param.prim_cfg.adv_intv_min = adv_creat_param->adv_param.prim_cfg.adv_intv_min;
    p_cmd->adv_param.prim_cfg.adv_intv_max = adv_creat_param->adv_param.prim_cfg.adv_intv_max;
    p_cmd->adv_param.prim_cfg.chnl_map = adv_creat_param->adv_param.prim_cfg.chnl_map;
    p_cmd->adv_param.prim_cfg.phy = adv_creat_param->adv_param.prim_cfg.phy;
    p_cmd->adv_param.second_cfg.max_skip = adv_creat_param->adv_param.second_cfg.max_skip;
    p_cmd->adv_param.second_cfg.phy = adv_creat_param->adv_param.second_cfg.phy;
    p_cmd->adv_param.second_cfg.adv_sid = adv_creat_param->adv_param.second_cfg.adv_sid;
    p_cmd->adv_param.period_cfg.adv_intv_min = adv_creat_param->adv_param.period_cfg.adv_intv_min;
    p_cmd->adv_param.period_cfg.adv_intv_max = adv_creat_param->adv_param.period_cfg.adv_intv_max;
    // Send the message
    ln_ke_msg_send(p_cmd);

}

void ln_app_set_adv_data(struct ln_gapm_set_adv_data_cmd *adv_data)
{
    // Prepare the GAPM_SET_ADV_DATA_CMD message
    struct gapm_set_adv_data_cmd *p_cmd = KE_MSG_ALLOC_DYN(GAPM_SET_ADV_DATA_CMD,
                                          TASK_GAPM, TASK_APP,
                                          gapm_set_adv_data_cmd,
                                          ADV_DATA_LEN);
    // Fill the allocated kernel message
    p_cmd->operation = GAPM_SET_ADV_DATA;
    p_cmd->actv_idx = adv_data->actv_idx;
    p_cmd->length = adv_data->length;
    memcpy(p_cmd->data, adv_data->data, adv_data->length);
    // Send the message
    ln_ke_msg_send(p_cmd);
}


void ln_app_set_scan_rsp_data(struct ln_gapm_set_adv_data_cmd *scan_rsp_data)
{
    // Prepare the GAPM_SET_ADV_DATA_CMD message
    struct gapm_set_adv_data_cmd *p_cmd = KE_MSG_ALLOC_DYN(GAPM_SET_ADV_DATA_CMD,
                                          TASK_GAPM, TASK_APP,
                                          gapm_set_adv_data_cmd,
                                          ADV_DATA_LEN);
    // Fill the allocated kernel message
    p_cmd->operation = GAPM_SET_SCAN_RSP_DATA;
    p_cmd->actv_idx = scan_rsp_data->actv_idx;
    p_cmd->length = scan_rsp_data->length;
    memcpy(p_cmd->data, scan_rsp_data->data, scan_rsp_data->length);
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_advertise_start(struct ln_gapm_activity_start_cmd * adv_start_param)
{
    struct gapm_activity_start_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_START_CMD,
                                            TASK_GAPM, TASK_APP,
                                            gapm_activity_start_cmd);

    p_cmd->operation = GAPM_START_ACTIVITY;
    p_cmd->actv_idx = adv_start_param->actv_idx;
    p_cmd->u_param.adv_add_param.duration = adv_start_param->u_param.adv_add_param.duration;
    p_cmd->u_param.adv_add_param.max_adv_evt =adv_start_param->u_param.adv_add_param.max_adv_evt;
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_scan_creat(struct ln_gapm_activity_create_cmd *scan_creat_param)
{
    // Prepare the GAPM_ACTIVITY_CREATE_CMD message
    struct gapm_activity_create_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_CREATE_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_create_cmd);

    // Set operation code
    p_cmd->operation = GAPM_CREATE_SCAN_ACTIVITY;
    p_cmd->own_addr_type = scan_creat_param->own_addr_type;
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_scan_start(struct ln_gapm_activity_start_cmd * scan_start_param)
{
    // Prepare the GAPM_ACTIVITY_START_CMD message
    struct gapm_activity_start_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_START_CMD,
                                            TASK_GAPM, TASK_APP,
                                            gapm_activity_start_cmd);
    p_cmd->operation = GAPM_START_ACTIVITY;
    p_cmd->actv_idx = scan_start_param->actv_idx;
    p_cmd->u_param.scan_param.type = scan_start_param->u_param.scan_param.type;
    p_cmd->u_param.scan_param.prop = scan_start_param->u_param.scan_param.prop;
    p_cmd->u_param.scan_param.dup_filt_pol = scan_start_param->u_param.scan_param.dup_filt_pol;
    p_cmd->u_param.scan_param.scan_param_1m.scan_intv = scan_start_param->u_param.scan_param.scan_param_1m.scan_intv;
    p_cmd->u_param.scan_param.scan_param_1m.scan_wd = scan_start_param->u_param.scan_param.scan_param_1m.scan_wd;
    p_cmd->u_param.scan_param.duration =scan_start_param->u_param.scan_param.duration;
    p_cmd->u_param.scan_param.scan_param_coded.scan_intv = scan_start_param->u_param.scan_param.scan_param_coded.scan_intv;
    p_cmd->u_param.scan_param.scan_param_coded.scan_wd = scan_start_param->u_param.scan_param.scan_param_coded.scan_wd;
    p_cmd->u_param.scan_param.period = scan_start_param->u_param.scan_param.period;
    // Send the message
    ln_ke_msg_send(p_cmd);

}



void ln_app_init_creat(struct ln_gapm_activity_create_cmd *init_creat_param)
{
    // Prepare the GAPM_ACTIVITY_CREATE_CMD message
    struct gapm_activity_create_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_CREATE_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_create_cmd);

    // Set operation code
    p_cmd->operation = GAPM_CREATE_INIT_ACTIVITY;
    p_cmd->own_addr_type = init_creat_param->own_addr_type;

    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_init_start(struct ln_gapm_activity_start_cmd * init_start_param)
{
    // Prepare the GAPM_ACTIVITY_START_CMD message
    struct gapm_activity_start_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_START_CMD,
                                            TASK_GAPM, TASK_APP,
                                            gapm_activity_start_cmd);
    p_cmd->operation = GAPM_START_ACTIVITY;
    p_cmd->actv_idx = init_start_param->actv_idx;
    p_cmd->u_param.init_param.type = init_start_param->u_param.init_param.type;
    p_cmd->u_param.init_param.prop = init_start_param->u_param.init_param.prop;
    p_cmd->u_param.init_param.conn_to = init_start_param->u_param.init_param.conn_to;
    p_cmd->u_param.init_param.scan_param_1m.scan_intv = init_start_param->u_param.init_param.scan_param_1m.scan_intv;
    p_cmd->u_param.init_param.scan_param_1m.scan_wd = init_start_param->u_param.init_param.scan_param_1m.scan_wd;
    p_cmd->u_param.init_param.scan_param_coded.scan_intv =init_start_param->u_param.init_param.scan_param_coded.scan_intv;
    p_cmd->u_param.init_param.scan_param_coded.scan_wd = init_start_param->u_param.init_param.scan_param_coded.scan_wd;

    p_cmd->u_param.init_param.conn_param_1m.conn_intv_min = init_start_param->u_param.init_param.conn_param_1m.conn_intv_min;
    p_cmd->u_param.init_param.conn_param_1m.conn_intv_max = init_start_param->u_param.init_param.conn_param_1m.conn_intv_max;
    p_cmd->u_param.init_param.conn_param_1m.conn_latency = init_start_param->u_param.init_param.conn_param_1m.conn_latency;
    p_cmd->u_param.init_param.conn_param_1m.ce_len_min = 0;
    p_cmd->u_param.init_param.conn_param_1m.ce_len_max = 0xFFFF;
    p_cmd->u_param.init_param.conn_param_1m.supervision_to= init_start_param->u_param.init_param.conn_param_1m.supervision_to;

    p_cmd->u_param.init_param.conn_param_2m.conn_intv_min = init_start_param->u_param.init_param.conn_param_2m.conn_intv_min;
    p_cmd->u_param.init_param.conn_param_2m.conn_intv_max = init_start_param->u_param.init_param.conn_param_2m.conn_intv_max;
    p_cmd->u_param.init_param.conn_param_2m.conn_latency = init_start_param->u_param.init_param.conn_param_2m.conn_latency;
    p_cmd->u_param.init_param.conn_param_2m.ce_len_min = 0;
    p_cmd->u_param.init_param.conn_param_2m.ce_len_max = 0xFFFF;
    p_cmd->u_param.init_param.conn_param_2m.supervision_to= init_start_param->u_param.init_param.conn_param_2m.supervision_to;

    p_cmd->u_param.init_param.conn_param_coded.conn_intv_min = init_start_param->u_param.init_param.conn_param_coded.conn_intv_min;
    p_cmd->u_param.init_param.conn_param_coded.conn_intv_max = init_start_param->u_param.init_param.conn_param_coded.conn_intv_max;
    p_cmd->u_param.init_param.conn_param_coded.conn_latency = init_start_param->u_param.init_param.conn_param_coded.conn_latency;
    p_cmd->u_param.init_param.conn_param_coded.ce_len_min = 0;
    p_cmd->u_param.init_param.conn_param_coded.ce_len_max = 0xFFFF;
    p_cmd->u_param.init_param.conn_param_coded.supervision_to= init_start_param->u_param.init_param.conn_param_coded.supervision_to;

    p_cmd->u_param.init_param.peer_addr.addr_type = init_start_param->u_param.init_param.peer_addr.addr_type;
    memcpy(p_cmd->u_param.init_param.peer_addr.addr.addr, init_start_param->u_param.init_param.peer_addr.addr.addr, GAP_BD_ADDR_LEN);
    BLIB_LOG_I("app_start_initiating peer_addr_type=%d, peer_addr:0x%x:0x%x:0x%x:0x%x:0x%x:0x%x\r\n",
        p_cmd->u_param.init_param.peer_addr.addr_type,
        p_cmd->u_param.init_param.peer_addr.addr.addr[0],
        p_cmd->u_param.init_param.peer_addr.addr.addr[1],
        p_cmd->u_param.init_param.peer_addr.addr.addr[2],
        p_cmd->u_param.init_param.peer_addr.addr.addr[3],
        p_cmd->u_param.init_param.peer_addr.addr.addr[4],
        p_cmd->u_param.init_param.peer_addr.addr.addr[5]);

    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_activity_stop(int actv_idx)
{
    // Prepare the GAPM_ACTIVITY_STOP_CMD message
    struct gapm_activity_stop_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_STOP_CMD,
                                           TASK_GAPM, TASK_APP,
                                           gapm_activity_stop_cmd);
    // Fill the allocated kernel message
    if (actv_idx >= 0)
        p_cmd->operation = GAPM_STOP_ACTIVITY;
    else
        p_cmd->operation = GAPM_STOP_ALL_ACTIVITIES;
    p_cmd->actv_idx = actv_idx;
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_activity_delete(int actv_idx)
{
    // Prepare the GAPM_ACTIVITY_CREATE_CMD message
    struct gapm_activity_delete_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_DELETE_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_delete_cmd);
    // Set operation code
    if (actv_idx >= 0)
        p_cmd->operation = GAPM_DELETE_ACTIVITY;
    else
        p_cmd->operation = GAPM_DELETE_ALL_ACTIVITIES;
    p_cmd->actv_idx = actv_idx;
    // Send the message
    ln_ke_msg_send(p_cmd);
}



void ln_app_use_enc(struct ln_gapm_use_enc_block_cmd *p_param)
{
    
    struct gapm_use_enc_block_cmd *p_cmd = KE_MSG_ALLOC(GAPM_USE_ENC_BLOCK_CMD,
                                                        TASK_GAPM, TASK_APP,
                                                        gapm_use_enc_block_cmd);
    p_cmd->operation = GAPM_USE_ENC_BLOCK;
    memcpy(p_cmd->operand_1, p_param->operand_1, GAP_KEY_LEN);
    memcpy(p_cmd->operand_2, p_param->operand_2, GAP_KEY_LEN);
    ln_ke_msg_send(p_cmd);
}

void ln_app_set_phy(int conidx,uint8_t tx_phy, uint8_t rx_phy, uint8_t phy_opt)
{
    struct gapc_set_phy_cmd *p_cmd = KE_MSG_ALLOC(GAPC_SET_PHY_CMD,KE_BUILD_ID(TASK_GAPC,conidx),
                                     TASK_APP, gapc_set_phy_cmd);
    p_cmd->operation = GAPC_SET_PHY;
    p_cmd->tx_phy = tx_phy;
    p_cmd->rx_phy = rx_phy;
    p_cmd->phy_opt= phy_opt;
    // Send the message
    ln_ke_msg_send(p_cmd);
}


void ln_app_disconnect(int conidx,uint32_t reason)
{
    struct gapc_disconnect_cmd *p_cmd = KE_MSG_ALLOC(GAPC_DISCONNECT_CMD,
                                        KE_BUILD_ID(TASK_GAPC,conidx), TASK_APP,
                                        gapc_disconnect_cmd);
    p_cmd->operation = GAPC_DISCONNECT;
    p_cmd->reason    = reason;
    // Send the message
    ln_ke_msg_send(p_cmd);
}



void ln_app_update_param(int conidx,struct ln_gapc_conn_param *p_conn_param)
{
    // Prepare the GAPC_PARAM_UPDATE_CMD message
    struct gapc_param_update_cmd *p_cmd = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CMD,
                                          KE_BUILD_ID(TASK_GAPC,conidx), TASK_APP,
                                          gapc_param_update_cmd);
    p_cmd->operation    = GAPC_UPDATE_PARAMS;
    p_cmd->intv_min = p_conn_param->intv_min;
    p_cmd->intv_max = p_conn_param->intv_max;
    p_cmd->latency  = p_conn_param->latency;
    p_cmd->time_out = p_conn_param->time_out;
    // not used by a slave device
    p_cmd->ce_len_min = 0;
    // p_cmd->ce_len_min = 0xFFFF;
    p_cmd->ce_len_max = 0xFFFF;
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_conn_cfm(int conidx, struct ln_gapc_connection_cfm *p_cfm)
{
    struct gapc_connection_cfm *p_cmd = KE_MSG_ALLOC( GAPC_CONNECTION_CFM,
                                        KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                        gapc_connection_cfm);
    memcpy(p_cmd->lcsrk.key, p_cfm->lcsrk.key, GAP_KEY_LEN);
    p_cmd->lsign_counter = p_cfm->lsign_counter;
    memcpy(p_cmd->rcsrk.key, p_cfm->rcsrk.key, GAP_KEY_LEN);
    p_cmd->rsign_counter = p_cfm->rsign_counter;
    p_cmd->auth = p_cfm->auth;
    p_cmd->cli_info = p_cfm->cli_info;
    p_cmd->ltk_present = p_cfm->ltk_present;
    p_cmd->cli_feat = p_cfm->cli_feat;
    p_cmd->gatt_start_handle = p_cfm->gatt_start_handle;
    p_cmd->gatt_end_handle = p_cfm->gatt_end_handle;
    p_cmd->svc_chg_handle = p_cfm->svc_chg_handle;
    // Send the message
    ke_msg_send(p_cmd);
}

void ln_app_get_peer_info(int conidx,struct ln_gapc_get_info_cmd* get_info)
{
    struct gapc_get_info_cmd *p_cmd = KE_MSG_ALLOC( GAPC_GET_INFO_CMD,
                                      KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                      gapc_get_info_cmd);
    switch(get_info->operation)
    {
        case GET_PEER_NAME:
        p_cmd->operation = GAPC_GET_PEER_NAME;
        break;
         case GET_PEER_VERSION:
        p_cmd->operation = GAPC_GET_PEER_VERSION;
        break;
         case GET_PEER_FEATURES:
        p_cmd->operation = GAPC_GET_PEER_FEATURES;
        break;
         case GET_CON_RSSI:
        p_cmd->operation = GAPC_GET_CON_RSSI;
        break;
         case GET_CON_CHANNEL_MAP:
        p_cmd->operation = GAPC_GET_CON_CHANNEL_MAP;
        break;
         case GET_PEER_APPEARANCE:
        p_cmd->operation = GAPC_GET_PEER_APPEARANCE;
        break;
         case GET_PEER_SLV_PREF_PARAMS:
        p_cmd->operation = GAPC_GET_PEER_SLV_PREF_PARAMS;
        break;
         case GET_ADDR_RESOL_SUPP:
        p_cmd->operation = GAPC_GET_ADDR_RESOL_SUPP;
        break;
         case GET_LE_PING_TO:
        p_cmd->operation = GAPC_GET_LE_PING_TO;
        break;
         case GET_PHY:
        p_cmd->operation = GAPC_GET_PHY;
        break;
         case GET_CHAN_SEL_ALGO:
        p_cmd->operation = GAPC_GET_CHAN_SEL_ALGO;
        break;
        default:
            break;
    }
    // Send the message
    ln_ke_msg_send(p_cmd);
}


void ln_app_param_update_cfm(int conidx, struct ln_gapc_param_update_cfm *update_cfm)
{
    struct gapc_param_update_cfm *p_cmd = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CFM,
                                          KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                          gapc_param_update_cfm);
    p_cmd->accept = update_cfm->accept;
    p_cmd->ce_len_min = 0;
    p_cmd->ce_len_max = 0xFFFF;
    // Send the message
    ke_msg_send(p_cmd);

}


void ln_app_param_set_pkt_size(int conidx, struct ln_gapc_set_le_pkt_size_cmd *pkt_size)
{
    struct gapc_set_le_pkt_size_cmd *p_cmd = KE_MSG_ALLOC(GAPC_SET_LE_PKT_SIZE_CMD,
                                          KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                          gapc_set_le_pkt_size_cmd);
    p_cmd->operation = GAPC_SET_LE_PKT_SIZE;
    p_cmd->tx_octets = pkt_size->tx_octets;
    p_cmd->tx_time = pkt_size->tx_time;
    // Send the message
    ln_ke_msg_send(p_cmd);

}

void  ln_app_bond(int conidx,struct ln_gapc_bond_cmd *p_param )
{
    struct gapc_bond_cmd *p_cmd = KE_MSG_ALLOC( GAPC_BOND_CMD,
                                  KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                  gapc_bond_cmd);
    p_cmd->operation = GAPC_BOND;
    p_cmd->pairing.iocap = p_param->pairing.iocap;
    p_cmd->pairing.oob = p_param->pairing.oob;
    p_cmd->pairing.auth = p_param->pairing.auth;
    p_cmd->pairing.key_size = p_param->pairing.key_size;
    p_cmd->pairing.ikey_dist = p_param->pairing.ikey_dist;
    p_cmd->pairing.rkey_dist = p_param->pairing.rkey_dist;
    p_cmd->pairing.sec_req = p_param->pairing.sec_req;
    // Send the message
    ln_ke_msg_send(p_cmd);

}


void ln_app_bond_cfm(int conidx,struct ln_gapc_bond_cfm *p_param)
{
    struct gapc_bond_cfm *p_cmd = KE_MSG_ALLOC( GAPC_BOND_CFM,
                                  KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                  gapc_bond_cfm);
    if (p_param->request == GAPC_PAIRING_RSP)
    {
        p_cmd->accept = p_param->accept;
        memcpy(&p_cmd->data.pairing_feat, &p_param->data.pairing_feat, sizeof(struct gapc_pairing));
    }
    else if (p_param->request == GAPC_LTK_EXCH)
    {
        p_cmd->accept = p_param->accept;
        memcpy(&p_cmd->data.ltk, &p_param->data.ltk, sizeof(struct gapc_ltk));
    }
    else if (p_param->request == GAPC_CSRK_EXCH)
    {
        p_cmd->accept = p_param->accept;
        memcpy(p_cmd->data.csrk.key, p_param->data.csrk.key, GAP_KEY_LEN);
    }
    else if (p_param->request == GAPC_TK_EXCH)
    {
        p_cmd->accept = p_param->accept;
        memcpy(p_cmd->data.tk.key, p_param->data.tk.key, GAP_KEY_LEN);
    }
    else if (p_param->request == GAPC_IRK_EXCH)
    {
        p_cmd->accept = p_param->accept;
        memcpy(&p_cmd->data.irk, &p_param->data.irk, sizeof(struct gapc_irk));
    }
    else if (p_param->request == GAPC_OOB_EXCH)
    {
        p_cmd->accept = p_param->accept;
        memcpy(p_cmd->data.oob.conf, p_param->data.oob.conf, GAP_KEY_LEN);
        memcpy(p_cmd->data.oob.rand, p_param->data.oob.rand, GAP_KEY_LEN);
    }
    else if (p_param->request == GAPC_NC_EXCH)
    {
        p_cmd->accept = p_param->accept;
    }
    // Send the message
    ke_msg_send(p_cmd);
}

void ln_app_keypress_notify(int conidx, struct ln_gapc_key_press_notif_cmd *p_param)
{
    
    struct gapc_key_press_notif_cmd *p_cmd = KE_MSG_ALLOC(GAPC_KEY_PRESS_NOTIFICATION_CMD,
                                                          KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                                          gapc_key_press_notif_cmd);    
    p_cmd->operation = GAPC_KEY_PRESS_NOTIFICATION;
    p_cmd->notification_type = p_param->notification_type;
    // Send the message
    ln_ke_msg_send(p_cmd);
    
}

void ln_app_get_dev_info_cfm(int conidx, struct ln_gapc_get_dev_info_cfm *p_param)
{
    struct gapc_get_dev_info_cfm *p_cmd = KE_MSG_ALLOC_DYN(GAPC_GET_DEV_INFO_CFM,
                                                       KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                                       gapc_get_dev_info_cfm,18);
    if (p_param->req == GAPC_DEV_NAME) {
        p_cmd->req=p_param->req;
        p_cmd->info.name.length=p_param->info.name.length;
        memcpy(p_cmd->info.name.value,p_param->info.name.value,p_param->info.name.length);
        
    } else if (p_param->req == GAPC_DEV_APPEARANCE) {
        p_cmd->req = p_param->req;
        p_cmd->info.appearance=p_param->info.appearance;
    } else if (p_param->req == GAPC_DEV_SLV_PREF_PARAMS) {
        p_cmd->req = p_param->req;
        p_cmd->info.slv_pref_params.con_intv_min= p_param->info.slv_pref_params.con_intv_min;
        p_cmd->info.slv_pref_params.con_intv_max=p_param->info.slv_pref_params.con_intv_max;
        p_cmd->info.slv_pref_params.conn_timeout=p_param->info.slv_pref_params.conn_timeout;
        p_cmd->info.slv_pref_params.slave_latency=p_param->info.slv_pref_params.slave_latency;
    } 
    // Send the message
    ke_msg_send(p_cmd);
}


void ln_app_set_dev_info_cfm(int conidx, struct ln_gapc_set_dev_info_cfm *p_param)
{
    struct gapc_set_dev_info_cfm *p_cmd = KE_MSG_ALLOC(GAPC_SET_DEV_INFO_CFM,
                                                      KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                                      gapc_set_dev_info_cfm);
    p_cmd->status = p_param->status;
    p_cmd->req = p_param->req;
    // Send the message
    ke_msg_send(p_cmd);
}


void ln_app_encrypt(int conidx,struct ln_gapc_encrypt_cmd *p_param)
{
    struct gapc_encrypt_cmd *p_cmd = KE_MSG_ALLOC(GAPC_ENCRYPT_CMD,
                                                  KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                                  gapc_encrypt_cmd);
    p_cmd->operation = GAPC_ENCRYPT;
    memcpy(p_cmd->ltk.ltk.key, p_param->ltk.ltk.key, GAP_KEY_LEN);
    p_cmd->ltk.ediv = p_param->ltk.ediv;
    memcpy(p_cmd->ltk.randnb.nb, p_param->ltk.randnb.nb, GAP_RAND_NB_LEN);
    p_cmd->ltk.key_size = p_param->ltk.key_size;
    // Send the message
    ln_ke_msg_send(p_cmd);
}

/// issue from master
void ln_app_encrypt_cfm(int conidx, struct ln_gapc_encrypt_cfm *p_param)
{
    struct gapc_encrypt_cfm *p_cmd = KE_MSG_ALLOC(GAPC_ENCRYPT_CFM,
                                                  KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                                  gapc_encrypt_cfm);
    p_cmd->found = p_param->found;
    memcpy(p_cmd->ltk.key, p_param->ltk.key, GAP_KEY_LEN);
    p_cmd->key_size = p_param->key_size;
    // Send the message
    ke_msg_send(p_cmd);
}

/// issue from slave
void ln_app_req_security(int conidx, struct ln_gapc_security_cmd *p_param)
{  
    struct gapc_security_cmd *p_cmd = KE_MSG_ALLOC(GAPC_SECURITY_CMD,
                                                    KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                                    gapc_security_cmd);
    p_cmd->operation = GAPC_SECURITY_REQ;
    p_cmd->auth = p_param->auth;
    // Send the message
    ln_ke_msg_send(p_cmd);
}




void ln_app_set_ping_tmo(int conidx, struct ln_gapc_set_le_ping_to_cmd *p_param)
{
    struct gapc_set_le_ping_to_cmd *p_cmd = KE_MSG_ALLOC(GAPC_SET_LE_PING_TO_CMD,
                                                        KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                                        gapc_set_le_ping_to_cmd);
    p_cmd->operation = GAPC_SET_LE_PING_TO;
    p_cmd->timeout = p_param->timeout;
    // Send the message
    ln_ke_msg_send(p_cmd);
}

/// @} APP
