set(WIFI_CFG_SRC
    ${COMP_WIFI_DIR}/wifi_cfg/wifi_cfg_protocol.c
    ${COMP_WIFI_DIR}/wifi_cfg/wifi_cfg_wpa.c
    ${COMP_WIFI_DIR}/wifi_cfg/wifi_cfg.c
    ${COMP_WIFI_DIR}/wifi_cfg/wifi_common.c
)

set(WIFI_IF_SRC
    ${COMP_WIFI_DIR}/wifi_if/wifi_if.c
    ${COMP_WIFI_DIR}/wifi_if/wlib_version.c
)

set(WIFI_MAC_COMMON_SRC
    ${COMP_WIFI_DIR}/wifi_mac/Common/common.c
)

set(WIFI_MAC_CONTROLLER_SRC
    ${COMP_WIFI_DIR}/wifi_mac/Controller/AP/event_parser_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Controller/AP/sme_ap.c

    ${COMP_WIFI_DIR}/wifi_mac/Controller/AP-STA/default_config.c
    ${COMP_WIFI_DIR}/wifi_mac/Controller/AP-STA/event_manager.c
    ${COMP_WIFI_DIR}/wifi_mac/Controller/AP-STA/event_parser.c
    ${COMP_WIFI_DIR}/wifi_mac/Controller/AP-STA/iconfig.c
    ${COMP_WIFI_DIR}/wifi_mac/Controller/AP-STA/maccontroller.c

    # ${COMP_WIFI_DIR}/wifi_mac/Controller/ModeIF/ # no src file.

    # ${COMP_WIFI_DIR}/wifi_mac/Controller/STA/P2P # not support.
    # ${COMP_WIFI_DIR}/wifi_mac/Controller/STA/ProtIF/ # no src file.
    ${COMP_WIFI_DIR}/wifi_mac/Controller/STA/event_parser_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Controller/STA/sme_sta.c
)

set(WIFI_MAC_CORE_SRC
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_1x/ieee_1x_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_1x/ieee_1x_auth_bak.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_1x/ieee_1x_auth.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_1x/ieee_1x_reauth.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_1x/radius_client.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_1x/radius_timers.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_1x/radius.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_1x/x_timers_auth.c

    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_11e/ap_frame_11e.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_11e/ap_management_11e.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_11e/ap_uapsd.c

    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_11i/ieee_11i_ap.c

    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_11n/ap_frame_11n.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_11n/ap_management_11n.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_11n/ap_wlan_rx_mgmt_11n.c

    # ${COMP_WIFI_DIR}/wifi_mac/Core/AP/P2P  # not support

    # ${COMP_WIFI_DIR}/wifi_mac/Core/AP/ProtIF/  # no src file.

    # ${COMP_WIFI_DIR}/wifi_mac/Core/AP/WPS  # not support

    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/autorate_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/cglobals_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/chan_mgmt_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/frame_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/fsm_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/host_rx_mlme_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/host_rx_msdu_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/management_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/pm_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/wep_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/wlan_misc_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/wlan_rx_data_ap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/wlan_rx_mgmt_ap.c

    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11e/frame_11e.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11e/management_11e.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11e/mib_11e.c

    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/aes.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/aeskeywrap.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/auth_frame_11i.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/eapol_key_auth.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/eapol_key.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/hmac_md5.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/ieee_11i_auth.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/md5.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/mib_11i.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/prf.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/rc4.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/rkmal_auth.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/rsna_auth_4way.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/rsna_auth_glk.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/rsna_auth_grpkey.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/rsna_auth_km.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/rsna_km.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/tkip_auth.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/tkip.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i/utils.c

    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11n/amsdu_aggr.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11n/amsdu_deaggr.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11n/blockack.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11n/frame_11n.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11n/management_11n.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11n/mib_11n.c

    # ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/P2P  # not support

    # ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/ProtIF  # no src file.

    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/autorate.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/buff_desc.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/cglobals.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/ch_info.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/frame.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/fsm.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/index_util.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/mac_init.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/management.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/metrics.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/mib.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/receive.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/reg_domains.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/transmit.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/wep.c

    # ${COMP_WIFI_DIR}/wifi_mac/Core/ModeIF  # no src file.

    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_1x/ieee_1x_md5_supp.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_1x/ieee_1x_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_1x/ieee_1x_supp.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_1x/x_timers_supp.c

    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_11e/sta_management_11e.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_11e/sta_uapsd.c

    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_11i/ieee_11i_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_11i/rkmal_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_11i/rsna_supp_4way.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_11i/rsna_supp_km.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_11i/sta_frame_11i.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_11i/tkip_sta.c

    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_11n/sta_frame_11n.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_11n/sta_management_11n.c

    # ${COMP_WIFI_DIR}/wifi_mac/Core/STA/P2P  # not support

    # ${COMP_WIFI_DIR}/wifi_mac/Core/STA/ProtIF # no src file.

    # ${COMP_WIFI_DIR}/wifi_mac/Core/STA/WPS/  # not support

    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/autorate_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/cglobals_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/channel_sw.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/frame_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/fsm_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/host_rx_mlme_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/host_rx_msdu_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/management_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/pm_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/wlan_misc_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/wlan_rx_data_sta.c
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/wlan_rx_mgmt_sta.c
)

set(WIFI_MAC_CSL_SRC
    ${COMP_WIFI_DIR}/wifi_mac/CSL/command.c
    ${COMP_WIFI_DIR}/wifi_mac/CSL/csl_if.c
    ${COMP_WIFI_DIR}/wifi_mac/CSL/pvtcmd_evm.c
)

set(WIFI_MAC_EDGE_SRC
    ${COMP_WIFI_DIR}/wifi_mac/Edge/Host/virtual_host.c

    ${COMP_WIFI_DIR}/wifi_mac/Edge/IF/host_if.c
    ${COMP_WIFI_DIR}/wifi_mac/Edge/IF/phy_hw_if.c
    ${COMP_WIFI_DIR}/wifi_mac/Edge/IF/proc_if.c
    ${COMP_WIFI_DIR}/wifi_mac/Edge/IF/rf_if.c

    ${COMP_WIFI_DIR}/wifi_mac/Edge/MACHW/ce_lut.c
    ${COMP_WIFI_DIR}/wifi_mac/Edge/MACHW/mh.c

    ${COMP_WIFI_DIR}/wifi_mac/Edge/PHY/PHYHW/LN/phy_LN.c
    ${COMP_WIFI_DIR}/wifi_mac/Edge/PHY/PHYProt/P802_11n/mib_802_11n.c
    ${COMP_WIFI_DIR}/wifi_mac/Edge/PHY/PHYProt/P802_11n/phy_802_11n.c

    # ${COMP_WIFI_DIR}/wifi_mac/Edge/Processor  # no src file.

    ${COMP_WIFI_DIR}/wifi_mac/Edge/RF/LN/reg_domain_ln88xx.c

    ${COMP_WIFI_DIR}/wifi_mac/Edge/RF_HW/rf_ln882x.c

    ${COMP_WIFI_DIR}/wifi_mac/Edge/RF_IMG_CAL/hw_img_cal/hw_img_cal_ln882x.c
    ${COMP_WIFI_DIR}/wifi_mac/Edge/RF_IMG_CAL/image_cal/img_cal_lib.c
    ${COMP_WIFI_DIR}/wifi_mac/Edge/RF_IMG_CAL/image_cal/img_cal_main.c
    ${COMP_WIFI_DIR}/wifi_mac/Edge/RF_IMG_CAL/image_cal/img_cal_tx_sm.c

    ${COMP_WIFI_DIR}/wifi_mac/Edge/TEMP_CAL/temp_cal.c
)

# (WPS) not support.
# set(WIFI_MAC_LIBRARY_SRC
# )

# no need.
# set(WIFI_MAC_TEST_SRC
# )

set(WIFI_MAC_UTILS_SRC
    ${COMP_WIFI_DIR}/wifi_mac/Utils/MM/imem_if.c
    ${COMP_WIFI_DIR}/wifi_mac/Utils/MM/imem.c
    ${COMP_WIFI_DIR}/wifi_mac/Utils/MM/scratch_mem.c
    ${COMP_WIFI_DIR}/wifi_mac/Utils/QM/qmu_rx.c
    ${COMP_WIFI_DIR}/wifi_mac/Utils/QM/qmu_tx.c
    ${COMP_WIFI_DIR}/wifi_mac/Utils/QM/qmu.c
)

set(WIFI_MAC_SRC
    ${WIFI_MAC_COMMON_SRC}
    ${WIFI_MAC_CONTROLLER_SRC}
    ${WIFI_MAC_CORE_SRC}
    ${WIFI_MAC_CSL_SRC}
    ${WIFI_MAC_EDGE_SRC}
    ${WIFI_MAC_UTILS_SRC}
)

set(WIFI_SRC
    ${WIFI_CFG_SRC}
    ${WIFI_IF_SRC}
    ${WIFI_MAC_SRC}
)

set(wifi_static_target "wifi")
add_library(${wifi_static_target} STATIC ${WIFI_SRC})
add_library(components::wifi ALIAS ${wifi_static_target})

add_custom_command(TARGET ${wifi_static_target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "regenerate static library: wifi"
    COMMAND ${CMAKE_COMMAND} -E copy lib${wifi_static_target}.a  ${CMAKE_SOURCE_DIR}/lib/gcclib/
    WORKING_DIRECTORY  ${LIBRARY_OUTPUT_PATH}
)

target_include_directories(${wifi_static_target}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/project/${USER_PROJECT}/cfg
    ${MCU_LN882X_DIR}/ln88xx
    ${MCU_LN882X_DIR}/CMSIS_5.3.0
    ${MCU_LN882X_DIR}/driver_ln882x/inc
    ${CMAKE_SOURCE_DIR}/components
    ${COMP_KERNEL_DIR}
    ${COMP_KERNEL_DIR}/FreeRTOS/Source/include
    ${COMP_KERNEL_DIR}/FreeRTOS/Source/portable/GCC/ARM_CM4F
    ${COMP_LWIP_DIR}/include
    ${COMP_LWIP_DIR}/port
    ${COMP_LWIP_DIR}/port/arch
    ${COMP_DHCPD_DIR}
    ${COMP_KV_DIR}/kv
    ${COMP_KV_DIR}/kv_port
    ${COMP_ATCMD_DIR}
    ${COMP_WIFI_DIR}/wifi_port
    ${COMP_WIFI_DIR}/wifi_mac/include
    ${COMP_WIFI_DIR}/wifi_mac/Common
    ${COMP_WIFI_DIR}/wifi_mac/Controller/AP
    ${COMP_WIFI_DIR}/wifi_mac/Controller/AP-STA
    ${COMP_WIFI_DIR}/wifi_mac/Controller/ModeIF
    ${COMP_WIFI_DIR}/wifi_mac/Controller/STA
    ${COMP_WIFI_DIR}/wifi_mac/Controller/STA/ProtIF
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11e
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11i
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/M802_11n
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP-STA/ProtIF
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_11e
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_11i
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_11n
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/M802_1x
    ${COMP_WIFI_DIR}/wifi_mac/Core/STA/ProtIF
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_11e
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_11i
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_11n
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/M802_1x
    ${COMP_WIFI_DIR}/wifi_mac/Core/AP/ProtIF
    ${COMP_WIFI_DIR}/wifi_mac/Core/ModeIF
    ${COMP_WIFI_DIR}/wifi_mac/Edge/Host
    ${COMP_WIFI_DIR}/wifi_mac/Edge/IF
    ${COMP_WIFI_DIR}/wifi_mac/Edge/MACHW
    ${COMP_WIFI_DIR}/wifi_mac/Edge/RF/LN
    ${COMP_WIFI_DIR}/wifi_mac/Edge/PHY/PHYHW/LN
    ${COMP_WIFI_DIR}/wifi_mac/Edge/PHY/PHYProt/P802_11n
    ${COMP_WIFI_DIR}/wifi_mac/Edge/TEMP_CAL
    ${COMP_WIFI_DIR}/wifi_mac/Edge/RF_HW
    ${COMP_WIFI_DIR}/wifi_mac/Edge/RF_IMG_CAL/hw_img_cal
    ${COMP_WIFI_DIR}/wifi_mac/Edge/RF_IMG_CAL/image_cal
    ${COMP_WIFI_DIR}/wifi_mac/Test/MacSwTest
    ${COMP_WIFI_DIR}/wifi_mac/Utils/MM
    ${COMP_WIFI_DIR}/wifi_mac/Utils/QM
    ${COMP_WIFI_DIR}/wifi_mac/CSL
    ${COMP_WIFI_DIR}/wifi_if
    ${COMP_WIFI_DIR}/wifi_cfg/include
    ${COMP_WIFI_DIR}/wifi_manager
)
