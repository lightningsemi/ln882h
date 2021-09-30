#ifndef __REG_MAC_PCU_REG_H__
#define __REG_MAC_PCU_REG_H__
//Auto-gen by fr
#include "ln882h.h"
//mac_pa_version
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                             y4 :  8; /* 7: 0, Indicates the tag number. This number is incremented whenever a new tag is done for the same source. Different tags are used when the same source is used to create H/W with different configurations*/
        uint32_t                             y3 :  8; /*15: 8, Incremented when bugs are fixed. Shall be reset when new Phase is starting.Reset when new feature is added without any bug fixes. Shall be set to 1 when new feature is added along with bug fixes. Maximum value is 255.*/
        uint32_t                             y2 :  8; /*23:16, Incremented for feature additions. Shall be reset when new Phase is starting.Maximum value is 255.*/
        uint32_t                             y1 :  4; /*27:24,           Phase of the project*/
        uint32_t                             bn :  4; /*31:28,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PA_VERSION;

//mac_pa_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        pa_mode :  1; /* 0: 0, 0 �C PA and PHY are powered down.
1 �C PA and PHY are powered up.*/
        uint32_t                       bss_ibss :  1; /* 1: 1, 1 �C This station is part of Independent BSS.
0 �C This station is part of Infrastructure BSS*/
        uint32_t                         ap_sta :  1; /* 2: 2, 0 �C In Infrastructure BSS this is an access point.
1 �C In Infrastructure BSS this is a station*/
        uint32_t                        edca_en :  1; /* 3: 3, 0 �C EDCA feature disabled. DCF procedure defined in 802.11 is followed.
1 �C EDCA feature is enabled.*/
        uint32_t                        hcca_en :  1; /* 4: 4, 0 �C HCCA feature disabled.
1 �C HCCA feature enabled.*/
        uint32_t                         pcf_en :  1; /* 5: 5, 0 �C PCF feature is disabled.
1 �C PCF feature is enabled.*/
        uint32_t                           qack :  1; /* 6: 6, 0 �C CF-ACK bit set in received non-directed Data frame will not be interpreted.
1 �C CF-ACK bit set in received non-directed Data frame will be
interpreted.CF-Ack bit set in received directed frame will be interpreted regardless of this setting.*/
        uint32_t                   rx_fifo_ctrl :  1; /* 7: 7, 0 �C The FULL indication given by Bus FIFO on RX path is used by PA Data path to transfer the bytes from PHY FIFO to Bus FIFO.
1 �C The FULL indication is disregarded. Bus FIFO may be
overwritten if not read out at the required rate.*/
        uint32_t            tx_head_update_ctrl :  1; /* 8: 8, 0 �C TX head pointer register (pointer to first frame in queue) with the next pointer read from current TX descriptor at completion of transmission.
1 �C TX head pointer register is always updated with NULL at
completion of transmission.*/
        uint32_t            rx_link_update_ctrl :  1; /* 9: 9, 0 �C RX link pointer register (pointer to next RX link element in
queue) with the next pointer read from current RX link element at completion of reception.                                                          1 �C RX link pointer register is always updated with NULL at
completion of reception.*/
        uint32_t        msdu_life_time_check_en :  1; /*10:10, 1 �C Lifetime of the MSDU is validated
0 �C Lifetime of the MSDU is not validated*/
        uint32_t                          ce_en :  1; /*11:11, 1 �C Cipher engine is enabled for encryption/decryption
0 �C Cipher engine is disabled.*/
        uint32_t                  pa_tx_suspend :  1; /*12:12, 1 �C Suspend request. This value triggers the suspension of
Protocol accelerator transmission after the current frame
exchange. SIFS response and Beacon transmission however are not stopped.
0 �C Protocol accelerator is not suspended.*/
        uint32_t          disable_duplicate_det :  1; /*13:13, 1 �C Disables duplicate detection in hardware.
0 �C Default. Duplicate detection in hardware is enabled.*/
        uint32_t     phy_register_address_width :  1; /*14:14, 0 �C Lower 7 bits of PHY Register Address only are valid.
1 �C All 8 bits of PHY Register Address are valid and are shifted out of SPI.*/
        uint32_t            fcs_failure_eifs_en :  1; /*15:15, 0 �C Disable performing of EIFS when FCS fails for a frame which was received when PHY CCA was idle.
1 - Enable performing of EIFS when FCS fails for a frame which was received when PHY CCA was idle.*/
        uint32_t               rx_error_eifs_en :  1; /*16:16, 0 �C Disable performing of EIFS when PHY indicates
RX_END.error and CCA was idle.
1 �C Enable performing of EIFS when PHY indicates RX_END.error and CCA was idle.*/
        uint32_t    cca_based_protocol_timer_en :  1; /*17:17, 0 �C Disable CCA based protocol timers. When disabled the slot timers are disabled when reception happens and CCA is idle.
1- Enable CCA based protocol timers. When enabled then the slot timers are enabled even when reception happens without CCA assertion.*/
        uint32_t                 disable_ack_tx :  1; /*18:18, 0 �C Normal mode. ACK is transmitted on reception of FCS valid directed unicast packets.
1 �C ACK transmission is disabled.*/
        uint32_t                 disable_cts_tx :  1; /*19:19, 0 �C Normal mode. CTS is transmitted on reception of FCS valid directed RTS frame when NAV is idle.
1 �C CTS transmission is disabled.*/
        uint32_t      pri_channel_cca_bypass_en :  1; /*20:20, 0 �C MAC uses the CCA indication provided for the Primary
channel by PHY for channel access
1 �C MAC by-passes the CCA indication for the Primary channel by PHY and uses the value provided by software*/
        uint32_t   pri_channel_cca_bypass_value :  1; /*21:21, CCA value to be used for By-passing the CCA indication for
Primary channel
0 �C CCA idle
1 �C CCA busy*/
        uint32_t      sec_channel_cca_bypass_en :  1; /*22:22, 0 �C MAC uses the CCA indication provided for the Secondary
channel by PHY for channel access
1 �C MAC by-passes the CCA indication for the Secondary channel by PHY and uses the value provided by software*/
        uint32_t   sec_channel_cca_bypass_value :  1; /*23:23, CCA value to be used for By-passing the CCA indication for
Secondary channel
0 �C CCA idle
1 �C CCA busy*/
        uint32_t                      ht_sta_en :  1; /*24:24, 1 �C HT STA enable
0 �C Non-HT STA*/
        uint32_t                   mac_clock_en :  1; /*25:25, 1 �C Clock is enabled for MAC H/W blocks
0 �C Clock is disabled (gated) for MAC H/W blocks*/
        uint32_t             defragmentation_en :  1; /*26:26, 1 �C Defragmentation will be done by H/W
0 �C Defragmentation will not be done in H/W*/
        uint32_t              de_aggregation_en :  1; /*27:27, 1 �C De-aggregation will be done by H/W
0 �C De-aggregation will not be done by H/W*/
        uint32_t                       tx_abort :  1; /*28:28, 1 �C Transmission will be aborted and PA will enter aborted state in which no transmission will happen. Any transmission which is in progress will stop abruptly.
0 �C PA will work normally based on the channel access function that is active.*/
        uint32_t           self_cts_on_tx_abort :  1; /*29:29, 1 �C PA will transmit self-CTS with in PIFS after a TX abort
0 �C PA will not transmit self-CTS after a TX abort*/
        uint32_t                  tx_abort_mode :  1; /*30:30, 1 �C TX abort can be initiated only by H/W
0 �C TX abort can be initiated only by S/W*/
        uint32_t              hw_auto_rating_en :  1; /*31:31,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PA_CTRL;

//mac_pa_status
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            fcs :  1; /* 0: 0, 0 �C FCS check failed.
1 �C FCS check passed.*/
        uint32_t                    cca_primary :  1; /* 1: 1,           0 �C Idle.
1 �C Busy*/
        uint32_t                  cca_secondary :  1; /* 2: 2,          0 �C Idle.
1 �C Busy.*/
        uint32_t             duplicate_detected :  1; /* 3: 3, 0 �C Received frame was not a duplicate.
1 �C Received frame was detected as duplicate.*/
        uint32_t                    beacon_txed :  1; /* 4: 4, 0 �C This means that beacon was not transmitted in this beacon
interval.
1 �C This means that beacon was transmitted in this beacon interval.*/
        uint32_t                    slot_in_use :  1; /* 5: 5, Slot currently in use in the STA.
0 �C Short slot is being used
1 �C Long slot is being used*/
        uint32_t             pa_in_suspend_mode :  1; /* 6: 6, Suspend status of Protocol accelerator
1 �C Protocol accelerator is in suspension mode. It will not initiate any frame exchange. However SIFS response and Beacon transmission are not stopped.
0 �C Protocol accelerator is active and can initiate frame exchanges.*/
        uint32_t      hi_priority_q_access_lock :  1; /* 7: 7, 0 �C The registers storing the pointer to the first frame in Hi Priority Q can be accessed.
1 �C The registers storing the pointer to the first frame in Hi Priority Q should not be accessed*/
        uint32_t            ac_bk_q_access_lock :  1; /* 8: 8, 0 �C The registers storing the pointer to the first frame in AC_BK Q can be accessed.
1 �C The registers storing the pointer to the first frame in AC_BK Q should not be accessed*/
        uint32_t            ac_be_q_access_lock :  1; /* 9: 9, 0 �C The registers storing the pointer to the first frame in AC_BE Q can be accessed.
1 �C The registers storing the pointer to the first frame in AC_BE Q should not be accessed*/
        uint32_t            ac_vi_q_access_lock :  1; /*10:10, 0 �C The registers storing the pointer to the first frame in AC_VI Q can be accessed.
1 �C The registers storing the pointer to the first frame in AC_VI Q should not be accessed*/
        uint32_t            ac_vo_q_access_lock :  1; /*11:11, 0 �C The registers storing the pointer to the first frame in AC_VO Q can be accessed.
1 �C The registers storing the pointer to the first frame in AC_VO Q should not be accessed*/
        uint32_t hcca_sta_priority_0_q_access_lock :  1; /*12:12, 0 �C The registers storing the pointer to the first frame in HCCA STA Priority 0 Q can be accessed.
1 �C The registers storing the pointer to the first frame in HCCA STA Priority 0 Q should not be accessed*/
        uint32_t hcca_sta_priority_1_q_access_lock :  1; /*13:13, 0 �C The registers storing the pointer to the first frame in HCCA STA Priority 1 Q can be accessed.
1 �C The registers storing the pointer to the first frame in HCCA STA Priority 1 Q should not be accessed*/
        uint32_t hcca_sta_priority_2_q_access_lock :  1; /*14:14, 0 �C The registers storing the pointer to the first frame in HCCA STA Priority 2 Q can be accessed.
1 �C The registers storing the pointer to the first frame in HCCA STA Priority 2 Q should not be accessed*/
        uint32_t hcca_sta_priority_3_q_access_lock :  1; /*15:15, 0 �C The registers storing the pointer to the first frame in HCCA STA Priority 3 Q can be accessed.
1 �C The registers storing the pointer to the first frame in HCCA STA Priority 3 Q should not be accessed*/
        uint32_t hcca_sta_priority_4_q_access_lock :  1; /*16:16, 0 �C The registers storing the pointer to the first frame in HCCA STA Priority 4 Q can be accessed.
1 �C The registers storing the pointer to the first frame in HCCA STA Priority 4 Q should not be accessed*/
        uint32_t hcca_sta_priority_5_q_access_lock :  1; /*17:17, 0 �C The registers storing the pointer to the first frame in HCCA STA Priority 5 Q can be accessed.
1 �C The registers storing the pointer to the first frame in HCCA STA Priority 5 Q should not be accessed*/
        uint32_t hcca_sta_priority_6_q_access_lock :  1; /*18:18, 0 �C The registers storing the pointer to the first frame in HCCA STA Priority 6 Q can be accessed.
1 �C The registers storing the pointer to the first frame in HCCA STA Priority 6 Q should not be accessed*/
        uint32_t hcca_sta_priority_7_q_access_lock :  1; /*19:19, 0 �C The registers storing the pointer to the first frame in HCCA STA Priority 7 Q can be accessed.
1 �C The registers storing the pointer to the first frame in HCCA STA Priority 7 Q should not be accessed*/
        uint32_t          hcca_ap_q_access_lock :  1; /*20:20, 0 �C The Software can access the Q pointer LUT from shared
memory.
1 �C The Software should not access the Q pointer LUT from shared memory.*/
        uint32_t              pcf_q_access_lock :  1; /*21:21, 0 �C The Software can access the Q pointer LUT from shared
memory.
1 �C The Software should not access the Q pointer LUT from shared memory.*/
        uint32_t beacon_txed_in_pre_beacon_interval :  1; /*22:22, 0 �C Indicates that Beacon was not transmitted in the previous
Beacon interval.
1 �C Indicates that Beacon was transmitted in the previous Beacon interval.*/
        uint32_t               pa_tx_abort_mode :  1; /*23:23, 0 �C Indicates that PA is not in TX abort mode
1 �C indicates that PA is in TX abort mode*/
        uint32_t   self_cts_tx_done_on_tx_abort :  1; /*24:24, 0 �C Self-CTS transmission is not done after TX abort
1 �C Self-CTS transmission is done after TX abort*/
        uint32_t                     reserved_0 :  7; /*31:25,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PA_STATUS;

//mac_address_high
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, The higher bytes of the MAC Address of this STA*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_ADDRESS_HIGH;

//mac_address_low
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, The lower bytes of the MAC Address of this STA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_ADDRESS_LOW;

//mac_bssid_high
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Higher 16-bits of the 48-bit BSSID value.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_BSSID_HIGH;

//mac_bssid_low
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Lower 32-bits of the 48-bit BSSID value.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_BSSID_LOW;

//mac_prbs_seed_value
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          ac_vo :  8; /* 7: 0, 8 bit PRBS Seed value for AC_VO access category*/
        uint32_t                          ac_vi :  8; /*15: 8, 8 bit PRBS Seed value for AC_VI access category*/
        uint32_t                          ac_be :  8; /*23:16, 8 bit PRBS Seed value for AC_BE access category*/
        uint32_t                          ac_bk :  8; /*31:24, 8 bit PRBS Seed value for AC_BK access category*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PRBS_SEED_VALUE;

//mac_dma_burst_size
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  5; /* 4: 0, Range is 1 �C 16. This is the number of words transferred in a
single atomic transfer by DMA.*/
        uint32_t                     reserved_0 : 27; /*31: 5,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_DMA_BURST_SIZE;

//mac_tx_rx_complete_cnt
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         tx_cnt : 16; /*15: 0, Number of TX complete interrupts issued by Hardware*/
        uint32_t                         rx_cnt : 16; /*31:16, Number of RX complete interrupts issued by Hardware*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TX_RX_COMPLETE_CNT;

//mac_prbs_read_value
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                      read_flag :  1; /* 0: 0, When this flag is set to 1 it initiates a PRBS value generation.
This flag is reset by hardware after a new PRBS value is loaded into the register.*/
        uint32_t                          value :  8; /* 8: 1, 8-bit PRBS value generated by hardware. The PRBS follows
uniform distribution.*/
        uint32_t                     reserved_0 : 23; /*31: 9,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PRBS_READ_VALUE;

//mac_null_frame_rate
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                null_frame_rate :  8; /* 7: 0, Data rate to be used for the NULL frame.*/
        uint32_t      null_frame_tx_power_level :  8; /*15: 8, Power level at which the NULL frame should be transmitted*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_NULL_FRAME_RATE;

//mac_null_frame_phy_tx_mode
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, b2:b0 000: 11b Short Preamble
001: 11b Long Preamble
010: 11a
011: Reserved
100: Reserved
101: Reserved
110: HT Mixed Mode Frame 
111: HT Green Field Frame                                                  
b4:b3 00: HT_BW20 �C 20 MHz
01: HT_BW40 �C 40 MHz
10: HT_BW_20DL �C Duplicate Legacy
11: HT_BW_20DH �C Duplicate HT
b6:b5 00: 40 MHz Band
01: Upper 20 MHz in 40 MHz Band
10: Reserved
11: Lower 20 MHz in 40 MHz Band
b7 0: Smoothing is not recommended
1: Smoothing is recommended
b8 0: A sounding packet
1: Not a sounding packet
b9 0: A packet without MPDU aggregation
1: A packet with MPDU aggregation
b10 0: Binary Convolution code
1: Low Density Parity Check code
b11 0: Short GI is not used for this packet
1: Short GI is used for this packet
b13:b12 Indicates the difference between the number of space
time streams NSTS and the number of spatial streams
NSS as indicated by MCS
00: No STBC (NSS = NSTS)
01: Reserved
10: Reserved
11: Reserved
b15:b14 Number of Extension spatial streams that will be
sounded during the extension part of the high
throughput training
b23:b16 Antenna set. This is an 8 bit set. A bit set in nth position
indicates that nth antenna is to be selected. At most 4
out of 8 bits can be set at any time
b31:b24 Reserved. Set to zero*/
    } bit_field;
} T_MAC_PCU_REG_MAC_NULL_FRAME_PHY_TX_MODE;

//mac_test_mode
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                 corrupt_tx_fcs :  1; /* 0: 0, 1 �C FCS of transmitted frame will be corrupted by H/W
0 �C FCS of transmitted frame will not be corrupted by H/W*/
        uint32_t                        skip_sn :  1; /* 1: 1, 1 �C Sequence number will not be generated for TX frames.
0 �C Sequence number will be generated for TX frames.*/
        uint32_t                     reserved_0 : 30; /*31: 2,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TEST_MODE;

//mac_hw_id
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Identifier for MAC H/W. Set to ��NMAC�� in ASCII*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HW_ID;

//mac_hw_reset_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                   mac_hw_reset :  1; /* 0: 0, Toggling of this bit causes the entire MAC H/W to be reset including
the register banks of PA and CE*/
        uint32_t                  pa_core_reset :  1; /* 1: 1, Toggling of this bit causes the PA core logic to be reset. The register bank of PA, TSF control block, Defrag LUT, Duplicate detection and interrupt FIFOs are not reset*/
        uint32_t                  ce_core_reset :  1; /* 2: 2, Toggling of this bit causes the CE core logic to be reset. The register bank of CE is not reset*/
        uint32_t                      tsf_reset :  1; /* 3: 3, Toggling of this bit causes the TSF control block to be reset*/
        uint32_t               defrag_lut_reset :  1; /* 4: 4, Toggling of this bit causes the Defrag LUT to be reset*/
        uint32_t      duplicate_detection_reset :  1; /* 5: 5, Toggling of this bit causes the duplicate detection block to be reset*/
        uint32_t                     reserved_0 : 26; /*31: 6,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HW_RESET_CTRL;

//mac_tx_abort_duration_timeout
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                    duration_id : 16; /*15: 0, Value to be inserted into the Duration/ID field in the Self-CTS frame used for TX abort. This is expressed in microseconds.*/
        uint32_t                        timeout : 16; /*31:16, The timeout period for the transmission of the Self-CTS frame after the TX abort request is asserted. This is expressed in microseconds.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TX_ABORT_DURATION_TIMEOUT;

//mac_tx_abort_frame_rate
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                           rate :  8; /* 7: 0, Data rate to be used for the TX abort frame.*/
        uint32_t                 tx_power_level :  8; /*15: 8, Power level at which the TX abort frame should be transmitted*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TX_ABORT_FRAME_RATE;

//mac_tx_abort_frame_phy_tx_mode
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, b2:b0 000: 11b Short Preamble
001: 11b Long Preamble
010: 11a
011: Reserved
100: Reserved
101: Reserved
110: HT Mixed Mode Frame
111: HT Green Field Frame
b4:b3 00: HT_BW20 �C 20 MHz
01: HT_BW40 �C 40 MHz
10: HT_BW_20DL �C Duplicate Legacy
11: HT_BW_20DH �C Duplicate HT
b6:b5 00: 40 MHz Band
01: Upper 20 MHz in 40 MHz Band
10: Reserved
11: Lower 20 MHz in 40 MHz Band
b7 0: Smoothing is not recommended
1: Smoothing is recommended
b8 0: A sounding packet
1: Not a sounding packet
b9 0: A packet without MPDU aggregation
1: A packet with MPDU aggregation
b10 0: Binary Convolution code
1: Low Density Parity Check code
b11 0: Short GI is not used for this packet
1: Short GI is used for this packet
b13:b12 Indicates the difference between the number of space
time streams NSTS and the number of spatial streams
NSS as indicated by MCS
00: No STBC (NSS = NSTS)
01: Reserved
10: Reserved
11: Reserved
b15:b14 Number of Extension spatial streams that will be
sounded during the extension part of the high
throughput training
b23:b16 Antenna set. This is an 8 bit set. A bit set in nth position
indicates that nth antenna is to be selected. At most 4
out of 8 bits can be set at any time
b31:b24 Reserved. Set to zero*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TX_ABORT_FRAME_PHY_TX_MODE;

//mac_extended_pa_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     touch_mpdu :  1; /* 0: 0, 0 �C H/W will insert relevant field in the MPDUs during transmission
1 �C H/W will not insert any field in the MPDU during transmission*/
        uint32_t            rx_buffer_based_ack :  1; /* 1: 1, 0 �C Rx Buffer Based ACK is not enabled.
1 �C Rx Buffer Based ACK is enabled*/
        uint32_t                sync_bus_pa_clk :  1; /* 2: 2, 0 �C Pa Clk and the AHB Master clocks are not synchronous
1 �C Pa Clk and the AHB Master clocks are synchronous*/
        uint32_t  sifs_responses_at_non_ht_rate :  1; /* 3: 3, 0 �C SIFS responses generated by H/W will be at non-HT Reference rate
1 �C SIFS responses generated by H/W will be at non-HT Basic rate*/
        uint32_t           disable_defrag_timer :  1; /* 4: 4,                             NA*/
        uint32_t                     reserved_0 : 27; /*31: 5,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_EXTENDED_PA_CTRL;

//mac_rx_frame_filter
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            ack :  1; /* 0: 0, 1 �C Enable filtering ACK frames.
0 �C Disable filtering ACK frames.*/
        uint32_t                            cts :  1; /* 1: 1, 1 �C Enable filtering CTS frames.
0 �C Disable filtering CTS frames.*/
        uint32_t                            rts :  1; /* 2: 2, 1 �C Enable filtering RTS frames.
0 �C Disable filtering RTS frames.*/
        uint32_t                         beacon :  1; /* 3: 3, 1 �C Enable filtering beacon frames.
0 �C Disable filtering beacon frames.*/
        uint32_t                           atim :  1; /* 4: 4, 1 �C Enable filtering ATIM frames.
0 �C Disable filtering ATIM frames.*/
        uint32_t                         cf_end :  1; /* 5: 5, 1 �C Enable filtering CF_END frames.
0 �C Disable filtering CF_END frames.*/
        uint32_t                       qcf_poll :  1; /* 6: 6, 1 �C Enable filtering QCF_POLL frames.
0 �C Disable filtering QCF_POLL frames.*/
        uint32_t              non_directed_data :  1; /* 7: 7, 1 �C Filter DATA frames not directed to this station.
0 �C Report DATA frames not directed to this station subject to
other filters.*/
        uint32_t           bc_mc_data_other_bss :  1; /* 8: 8, 1 �C Filter BC/MC DATA frames belonging to other BSS.
0 �C Report BC/MC DATA frames from other BSS.*/
        uint32_t               duplicate_frames :  1; /* 9: 9, 1 �C Enable filtering of duplicate frames
0 �C Disable filtering of duplicate frames*/
        uint32_t                     fcs_failed :  1; /*10:10, 1 �C Enable filtering of frames whose FCS has failed
0 �C Disable filtering of frames whose FCS has failed*/
        uint32_t              de_authentication :  1; /*11:11, 1 �C Enable filtering of De-authentication frame
0 �C Disable filtering of De-authentication frame*/
        uint32_t          discarded_ibss_beacon :  1; /*12:12, 1 �C Filter Beacon frames (in IBSS mode) which are not used for adoption because the timestamp field is lower than TSF timer.
0 �C Report all Beacon frames subject to other filters*/
        uint32_t              non_directed_mgmt :  1; /*13:13, 1 �C Filter Management frames which are not directed to current STA
0 �C Non-directed Management frames are reported subject to
other filters*/
        uint32_t              non_directed_ctrl :  1; /*14:14, 1 �C Filter Control frames which are not directed to current STA
0 �C Non-directed Control frames are reported subject to other
filters.*/
        uint32_t             sec_channel_frames :  1; /*15:15, 1 �C Filter frames which are received in secondary channel (20
MHz PPDU from Secondary channel)
0 �C Frames received in secondary channel are reported
irrespective of other filters*/
        uint32_t           bc_mc_mgmt_other_bss :  1; /*16:16, 1 �C Filter BC/MC MGMT frames belonging to other BSS.
0 �C Report BC/MC MGMT frames from other BSS.*/
        uint32_t     non_sifs_response_ba_frame :  1; /*17:17, 1 �C Filter BA frames which are not received as SIFS response
0 �C Report BA frames which are not received as SIFS response*/
        uint32_t          sifs_respone_ba_frame :  1; /*18:18, 1 �C Filter BA frames which are received as SIFS response
0 �C Report BA frames which are received as SIFS response*/
        uint32_t                     reserved_0 : 13; /*31:19,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RX_FRAME_FILTER;

//mac_frame_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t               protocol_version :  2; /* 1: 0, Specifies the version of current MAC protocol version supported. This is used in the header validation of the received
MAC frames.*/
        uint32_t                     reserved_0 : 30; /*31: 2,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_FRAME_CTRL;

//mac_rx_buffer_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, The base address to store the received packet and its descriptor. It is updated by PA on reception of a packet.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RX_BUFFER_ADDR;

//mac_fcs_fail_cnt
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Total number of frames received with FCS failure. This can be
reset by software by writing into this register.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_FCS_FAIL_CNT;

//mac_rx_max_frame_length_filter
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Default value is 2100 bytes. When a frame is received, whose
data portion length is greater than the value in this register than
that entire frame is transferred to only 1 memory location. In
effect that frame is dropped. A setting of zero will drop all
frames.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RX_MAX_FRAME_LENGTH_FILTER;

//mac_duplicate_frame_cnt
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Total number of Duplicate frames received in the current STA.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_DUPLICATE_FRAME_CNT;

//mac_rx_end_cnt
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Total number of frames received without PHY level errors*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RX_END_CNT;

//mac_rx_end_error_cnt
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Total number of PHY receive header error indications*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RX_END_ERROR_CNT;

//mac_ampdu_rxed_cnt
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Total number of AMPDUs received*/
    } bit_field;
} T_MAC_PCU_REG_MAC_AMPDU_RXED_CNT;

//mac_mpdu_in_ampdu_cnt
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Total number of MPDUs received in the A-MPDU when an AMPDU is received*/
    } bit_field;
} T_MAC_PCU_REG_MAC_MPDU_IN_AMPDU_CNT;

//mac_rxed_octets_in_ampdu
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, The number of octets in the A-MPDU frame when an A-MPDU
frame is received*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RXED_OCTETS_IN_AMPDU;

//mac_ampdu_del_crc_error_cnt
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, This register holds the value of CRC errors of an AMPDU
delimiter when this is the first CRC error in the received AMPDU or when the previous delimiter has been decoded
correctly*/
    } bit_field;
} T_MAC_PCU_REG_MAC_AMPDU_DEL_CRC_ERROR_CNT;

//mac_defrag_lifetime_limit
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                    rx_lifetime : 10; /* 9: 0, RX lifetime value used during defragmentation. It is expressed
in milliseconds*/
        uint32_t                     reserved_0 : 22; /*31:10,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_DEFRAG_LIFETIME_LIMIT;

//mac_hi_priority_rx_q_buffer_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, The base address to store the received packet and its descriptor in Hi-priority Queue. It is updated by PA on reception of a packet with the address of the next buffer in the linked list.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HI_PRIORITY_RX_Q_BUFFER_ADDR;

//mac_hi_priority_rx_q_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t               hi_priority_q_en :  1; /* 0: 0, 1 �C Enable High Priority Queue
0 �C Disable High Priority Queue*/
        uint32_t                         beacon :  1; /* 1: 1, 1 �C keep Beacon frame in hi-priority RX Queue
0 �C keep it in normal Queue*/
        uint32_t                  probe_request :  1; /* 2: 2, 1 �C keep Probe Request frame in hi-priority RX Queue
0 �C keep it in normal Queue*/
        uint32_t                 probe_response :  1; /* 3: 3, 1 �C keep Probe Response frame in hi-priority RX Queue
0 �C keep it in normal Queue*/
        uint32_t                           atim :  1; /* 4: 4, 1 �C keep ATIM frame in hi-priority RX Queue
0 �C keep it in normal Queue*/
        uint32_t                     reserved_0 : 27; /*31: 5,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HI_PRIORITY_RX_Q_CTRL;

//mac_sub_msdu_gap
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Gap between the MSDUs of an AMSDU in bytes.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_SUB_MSDU_GAP;

//mac_mac_rx_buffer_length
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Maximum length of the Rx Buffer in the Shared Memory to be programmed by MACSW*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_MAC_RX_BUFFER_LENGTH;

//mac_aifsn
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          ac_vo :  4; /* 3: 0, AIFSN value for the AC_VO Category. This is the number of
slots of idle time after SIFS during which channel has to remain
 idle before next EDCA TXOP can start.*/
        uint32_t                          ac_vi :  4; /* 7: 4, AIFSN value for the AC_VI Category. This is the number of slots of idle time after SIFS during which channel has to remain idle before next EDCA TXOP can start.*/
        uint32_t                          ac_be :  4; /*11: 8, AIFSN value for the AC_BE Category. This is the number of
slots of idle time after SIFS during which channel has to remain
idle before next EDCA TXOP can start.*/
        uint32_t                          ac_bk :  4; /*15:12, AIFSN value for the AC_BK Category. This is the number of
slots of idle time after SIFS during which channel has to remain
idle before next EDCA TXOP can start.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_AIFSN;

//mac_cwminmax_ac_bk
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            min :  4; /* 3: 0, Min = 1; Max = 10
Minimum contention window limit for Access Category BK.*/
        uint32_t                            max :  4; /* 7: 4, Min = 1; Max = 10
Maximum contention window limit for Access Category BK.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_CWMINMAX_AC_BK;

//mac_cwminmax_ac_be
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            min :  4; /* 3: 0, Min = 1; Max = 10
Minimum contention window limit for Access Category BE.*/
        uint32_t                            max :  4; /* 7: 4, Min = 1; Max = 10
Maximum contention window limit for Access Category BE.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_CWMINMAX_AC_BE;

//mac_cwminmax_ac_vi
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            min :  4; /* 3: 0, Min = 1; Max = 10
Minimum contention window limit for Access Category VI.*/
        uint32_t                            max :  4; /* 7: 4, Min = 1; Max = 10
Maximum contention window limit for Access Category VI.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_CWMINMAX_AC_VI;

//mac_cwminmax_ac_vo
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            min :  4; /* 3: 0, Min = 1; Max = 10
Minimum contention window limit for Access Category VO.*/
        uint32_t                            max :  4; /* 7: 4, Min = 1; Max = 10
Maximum contention window limit for Access Category VO.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_CWMINMAX_AC_VO;

//mac_edca_txop_limit_ac_bk_be
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          ac_bk : 16; /*15: 0, Min = 1; Max = 65535
This is the TXOP limit for Access Category BK in microseconds.*/
        uint32_t                          ac_be : 16; /*31:16, Min = 1; Max = 65535
This is the TXOP limit for Access Category BE in microseconds.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_EDCA_TXOP_LIMIT_AC_BK_BE;

//mac_edca_txop_limit_ac_vi_vo
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          ac_vi : 16; /*15: 0, Min = 1; Max = 65535
This is the TXOP limit for Access Category VI in microseconds.*/
        uint32_t                          ac_vo : 16; /*31:16, Min = 1; Max = 65535
This is the TXOP limit for Access Category VO in microseconds.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_EDCA_TXOP_LIMIT_AC_VI_VO;

//mac_ac_bk_first_frame_pointer
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the first MSDU in AC_BK Q*/
    } bit_field;
} T_MAC_PCU_REG_MAC_AC_BK_FIRST_FRAME_POINTER;

//mac_ac_bk_q_retry_cnts
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                short_retry_cnt : 16; /*15: 0,     Short retry count of the Q*/
        uint32_t                 long_retry_cnt : 16; /*31:16,      Long retry count of the Q*/
    } bit_field;
} T_MAC_PCU_REG_MAC_AC_BK_Q_RETRY_CNTS;

//mac_ac_be_first_frame_pointer
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the first MSDU in AC_BE Q*/
    } bit_field;
} T_MAC_PCU_REG_MAC_AC_BE_FIRST_FRAME_POINTER;

//mac_ac_be_q_retry_cnts
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                short_retry_cnt : 16; /*15: 0,     Short retry count of the Q*/
        uint32_t                 long_retry_cnt : 16; /*31:16,      Long retry count of the Q*/
    } bit_field;
} T_MAC_PCU_REG_MAC_AC_BE_Q_RETRY_CNTS;

//mac_ac_vi_first_frame_pointer
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the first MSDU in AC_VI Q*/
    } bit_field;
} T_MAC_PCU_REG_MAC_AC_VI_FIRST_FRAME_POINTER;

//mac_ac_vi_q_retry_cnts
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                short_retry_cnt : 16; /*15: 0,     Short retry count of the Q*/
        uint32_t                 long_retry_cnt : 16; /*31:16,      Long retry count of the Q*/
    } bit_field;
} T_MAC_PCU_REG_MAC_AC_VI_Q_RETRY_CNTS;

//mac_ac_vo_first_frame_pointer
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the first MSDU in AC_VO Q*/
    } bit_field;
} T_MAC_PCU_REG_MAC_AC_VO_FIRST_FRAME_POINTER;

//mac_ac_vo_q_retry_cnts
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                short_retry_cnt : 16; /*15: 0,     Short retry count of the Q*/
        uint32_t                 long_retry_cnt : 16; /*31:16,      Long retry count of the Q*/
    } bit_field;
} T_MAC_PCU_REG_MAC_AC_VO_Q_RETRY_CNTS;

//mac_hi_prioriy_first_frame_pointer
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the first MSDU in High Priority Q*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HI_PRIORIY_FIRST_FRAME_POINTER;

//mac_msdu_max_tx_lifetime_limit
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Maximum transmission lifetime limit. This register provides the
lifetime limit in milli seconds for the transmission of the MSDU.
The lifetime is validated from the time at which the first
transmission attempt is made for the MSDU.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_MSDU_MAX_TX_LIFETIME_LIMIT;

//mac_ac_be_bk_edca_lifetime_limit
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                 ac_bk_lifetime : 16; /*15: 0, EDCA lifetime limit for BK access category. This register
provides the lifetime limit in milli seconds for the transmission of
the MSDU. The lifetime is validated from the time at which the
MSDU is delivered to MAC.*/
        uint32_t                 ac_be_lifetime : 16; /*31:16, EDCA lifetime limit for BE access category. This register
provides the lifetime limit in milli seconds for the transmission of
the MSDU. The lifetime is validated from the time at which the
MSDU is delivered to MAC.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_AC_BE_BK_EDCA_LIFETIME_LIMIT;

//mac_ac_vo_vi_edca_lifetime_limit
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                 ac_vi_lifetime : 16; /*15: 0, EDCA lifetime limit for VI access category. This register
provides the lifetime limit in milli seconds for the transmission of
the MSDU. The lifetime is validated from the time at which the
MSDU is delivered to MAC.*/
        uint32_t                 ac_vo_lifetime : 16; /*31:16, EDCA lifetime limit for VO access category. This register
provides the lifetime limit in milli seconds for the transmission of
the MSDU. The lifetime is validated from the time at which the
MSDU is delivered to MAC.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_AC_VO_VI_EDCA_LIFETIME_LIMIT;

//mac_hcca_sta_priority0_first_frame_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the first frame in HCCA STA Priority 0 Queue.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY0_FIRST_FRAME_ADDR;

//mac_hcca_sta_priority1_first_frame_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the first frame in HCCA STA Priority 1 Queue.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY1_FIRST_FRAME_ADDR;

//mac_hcca_sta_priority2_first_frame_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the first frame in HCCA STA Priority 2 Queue.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY2_FIRST_FRAME_ADDR;

//mac_hcca_sta_priority3_first_frame_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the first frame in HCCA STA Priority 3 Queue.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY3_FIRST_FRAME_ADDR;

//mac_hcca_sta_priority4_first_frame_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the first frame in HCCA STA Priority 4 Queue.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY4_FIRST_FRAME_ADDR;

//mac_hcca_sta_priority5_first_frame_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the first frame in HCCA STA Priority 5 Queue.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY5_FIRST_FRAME_ADDR;

//mac_hcca_sta_priority6_first_frame_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the first frame in HCCA STA Priority 6 Queue.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY6_FIRST_FRAME_ADDR;

//mac_hcca_sta_priority7_first_frame_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the first frame in HCCA STA Priority 7 Queue.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY7_FIRST_FRAME_ADDR;

//mac_tsf_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         tsf_en :  1; /* 0: 0, 0 �C This function is disabled. TBTT interrupt is not generated.
1 �C This function is enabled. In IBSS STA case, beacon
transmission at TBTT and timestamp adoption is also enabled.
In BSS STA case timestamp adoption is enabled. In AP case,
only beacon transmission is enabled*/
        uint32_t                     start_join :  1; /* 1: 1, This bit is interpreted when TSF Enable is set to 1. It is also
subject to whether it is STA or AP and network is BSS or IBSS.
0 �C After being enabled, the TSF waits to receive Beacon or
Probe Response frame with matching BSSID, adopts the
parameters and starts the TSF timer and TBTT generation.
1 �C After being enabled, the TSF immediately starts the TSF
timer and TBTT generation.*/
        uint32_t              beacon_tx_suspend :  1; /* 2: 2, 0 �C Beacon transmission will be performed after TBTT (if
required for the current STA)                                                               1 �C Beacon transmission will be suspended*/
        uint32_t                     reserved_0 : 29; /*31: 3,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TSF_CTRL;

//mac_tsf_timer_read_value_high
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, PA will write the higher 32-bits of the value read from TSF timer.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TSF_TIMER_READ_VALUE_HIGH;

//mac_tsf_timer_read_value_low
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, PA will write the lower 32-bits of the value read from TSF timer.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TSF_TIMER_READ_VALUE_LOW;

//mac_beacon_period
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Time between two Beacon transmissions.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_BEACON_PERIOD;

//mac_dtim_period
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, Number of beacon intervals between the transmissions of beacon frames containing TIM element whose DTIM count is zero.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_DTIM_PERIOD;

//mac_beacon_packet_pointer
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the location of Beacon packet descriptor.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_BEACON_PACKET_POINTER;

//mac_beacon_tx_param
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         length : 16; /*15: 0,            Range is 1 �C 65535*/
        uint32_t                      data_rate :  8; /*23:16, See Section 0 for data rate encoding*/
        uint32_t                    power_level :  8; /*31:24, The power level at which the Beacon frame needs to be transmitted.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_BEACON_TX_PARAM;

//mac_dtim_count_status
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, Value of the internal DTIM counter in MAC Hardware.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_DTIM_COUNT_STATUS;

//mac_dtim_count_update_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, Value to update internal DTIM counter*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_DTIM_COUNT_UPDATE_CTRL;

//mac_beacon_phy_tx_mode
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, b2:b0  000: 11b Short Preamble 001: 11b Long Preamble 010: 11a 011: Reserved 100: Reserved 101: Reserved 110: HT Mixed Mode Frame 111: HT Green Field Frame
b4:b3  00: HT_BW20 �C 20 MHz 01: HT_BW40 �C 40 MHz 10: HT_BW_20DL �C Duplicate Legacy 11: HT_BW_20DH �C Duplicate HT
b6:b5  00: 40 MHz Band 01: Upper 20 MHz in 40 MHz Band 10: Reserved 11: Lower 20 MHz in 40 MHz Band
b7  0: Smoothing is not recommended 1: Smoothing is recommended
b8  0: A sounding packet 1: Not a sounding packet
b9  0: A packet without MPDU aggregation 1: A packet with MPDU aggregation
b10  0: Binary Convolution code 1: Low Density Parity Check code
b11  0: Short GI is not used for this packet 1: Short GI is used for this packet
b13:b12  Indicates the difference between the number of space time streams NSTS and the number of spatial streams NSS as indicated by MCS 00: No STBC (NSS = NSTS) 01: Reserved 10: Reserved 11: Reserved
b15:b14  Number of Extension spatial streams that will be sounded during the extension part of the high throughput training
b23:b16  Antenna set. This is an 8 bit set. A bit set in nth position indicates that nth antenna is to be selected. At most 4 out of 8 bits can be set at any time
b31:b24  Reserved. Set to zero
*/
    } bit_field;
} T_MAC_PCU_REG_MAC_BEACON_PHY_TX_MODE;

//mac_tbtt_timer_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 26; /*25: 0, TBTT timer read/write value. This value represents the time left for TBTT. When a value is written into this register, the internal TBTT timer is updated with this value. When this register is read, the internal TBTT timer value is read*/
        uint32_t                     reserved_0 :  6; /*31:26,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TBTT_TIMER_CTRL;

//mac_protection_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                           mode :  2; /* 1: 0, 00 �C 802.11b physical layer 01 �C 802.11a physical layer 10 �C 802.11g Compatibility 11 �C 802.11g High Rate*/
        uint32_t              enhanced_nav_prot :  1; /* 2: 2, 0 �C No enhanced protection 1 �C This overrides RTS threshold and mode bits. Protection frame is transmitted for every burst (fragmentation burst or TXOP burst) This is valid only for 11e (any Mode)*/
        uint32_t                  nav_port_type :  1; /* 3: 3, 0 �C RTS/CTS 1 �C Self CTS This is valid only if Enhanced NAV Protection is set.*/
        uint32_t                       erp_prot :  1; /* 4: 4, 0 �C No ERP protection 1 �C Protection required for OFDM frames if Mode = 10 This is valid only for Mode = 10*/
        uint32_t                  erp_prot_type :  1; /* 5: 5, 0 �C RTS/CTS 1 �C Self CTS This is valid only if ERP Protection is set.*/
        uint32_t                     reserved_0 : 26; /*31: 6,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PROTECTION_CTRL;

//mac_rts_thresh
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Minimum value �C 0. This will enable RTS transmission for all lengths. Maximum value �C 3000. This will disable RTS transmission for all lengths.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RTS_THRESH;

//mac_protection_data_rate
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                 prot_data_rate :  8; /* 7: 0, One of the basic data rates to be used for transmission of protection frame. The interpretation of the field is given in 0.*/
        uint32_t            prot_tx_power_level :  8; /*15: 8, The power level at which the protection frame should be transmitted.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PROTECTION_DATA_RATE;

//mac_txop_holder_addr_low
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, 32 lower bits of the 48-bit MAC address of the STA that holds the current TXOP. If the TA of the received RTS matches the 48-bit value then CTS is transmitted after SIFS time irrespective of NAV status.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TXOP_HOLDER_ADDR_LOW;

//mac_txop_holder_addr_high
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, 16 Higher bits of the 48-bit MAC address of the STA that holds the current TXOP. If the TA of the received RTS matches the 48-bit value then CTS is transmitted after SIFS time irrespective of NAV status.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TXOP_HOLDER_ADDR_HIGH;

//mac_resp_rate_lut0
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0,                         no use*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RESP_RATE_LUT0;

//mac_resp_rate_lut1
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0,                         no use*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RESP_RATE_LUT1;

//mac_fragmentation_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                    frag_thresh : 16; /*15: 0, The fragmentation threshold to be used for deciding fragment length. The threshold should not be less than minimum possible MPDU length and the maximum number of fragments should not exceed 16. If any of these conditions are violated the behavior of Protocol accelerator is unpredictable.*/
        uint32_t          mini_txop_frag_length : 16; /*31:16, This value sets the lower limit on the fragment length, which is created to fit the TXOP. When the maximum MPDU length that can be transmitted in the given TXOP time is less than this limit, the TXOP is relinquished. Otherwise the given MSDU is fragmented to fit the TXOP time.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_FRAGMENTATION_CTRL;

//mac_protection_tx_mode
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, b2:b0  000: 11b Short Preamble 001: 11b Long Preamble 010: 11a 011: Reserved 100: Reserved 101: Reserved 110: HT Mixed Mode Frame 111: HT Green Field Frameb4:b3  00: HT_BW20 �C 20 MHz 01: HT_BW40 �C 40 MHz 10: HT_BW_20DL �C Duplicate Legacy 11: HT_BW_20DH �C Duplicate HT
b6:b5  00: 40 MHz Band 01: Upper 20 MHz in 40 MHz Band 10: Reserved 11: Lower 20 MHz in 40 MHz Band
b7  0: Smoothing is not recommended 1: Smoothing is recommended
b8  0: A sounding packet 1: Not a sounding packet
b9  0: A packet without MPDU aggregation 1: A packet with MPDU aggregation
b10  0: Binary Convolution code 1: Low Density Parity Check code
b11  0: Short GI is not used for this packet 1: Short GI is used for this packet
b13:b12  Indicates the difference between the number of space time streams NSTS and the number of spatial streams NSS as indicated by MCS 00: No STBC (NSS = NSTS) 01: Reserved 10: Reserved 11: Reserved
b15:b14  Number of Extension spatial streams that will be sounded during the extension part of the high throughput training
b23:b16  Antenna set. This is an 8 bit set. A bit set in nth position indicates that nth antenna is to be selected. At most 4 out of 8 bits can be set at any time
b31:b24  Reserved. Set to zero
*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PROTECTION_TX_MODE;

//mac_ht_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t              ht_operation_mode :  2; /* 1: 0, Indicates the HT operation mode*/
        uint32_t               long_nav_support :  1; /* 2: 2, 0 �� Long NAV supported 1 �� Long NAV not supported*/
        uint32_t                   ht_prot_type :  2; /* 4: 3, 0 �� RTS-CTS at non-HT rate 1 �� First frame exchange at non-HT rate 2 �� LSIG TXOP 3 �� Mixed format at non-HT rate*/
        uint32_t             non_gf_sta_present :  1; /* 5: 5, 0 �� Non GF HT STA present 1 �� Non GF HT STA not present*/
        uint32_t                  rifs_burst_en :  1; /* 6: 6, 0 �� MAC HW will not perform RIFS burst 1 �� MAC HW will perform RIFS burst*/
        uint32_t                disable_rts_cts :  1; /* 7: 7, 0 ��MAC H/W will perform RTS-CTS frame exchange if the first frame in the TXOP is of Block ACK or NO_ACK type 1 ��MAC H/W will not perform RTS-CTS frame exchange if the first frame in the TXOP is of Block ACK or NO_ACK type*/
        uint32_t                      fc_sta_en :  1; /* 8: 8, 0 �� 20/40 MHz Co-existence mode of operation is disabled 1 �� 20/40 MHz Co-existence mode of operation is enabled.*/
        uint32_t           txop_promotion_40mhz :  1; /* 9: 9, This bit is applicable only when FC STA is enabled 0 ��TXOP promotions are not enabled. MAC H/W will not try to promote a 20 MHz TXOP into a 40 MHz TXOP 1 �� MAC H/W tries to promote every TXOP into a 40 MHz TXOP. If secondary channel is idle for programmed number of slots when backoff is complete in primary channel then MAC H/W starts a 40 MHz TXOP irrespective of the channel bandwidth of the first frame in the TX queue. In order to do this, MAC H/W uses non-HT duplicate (either the given frame or RTS-CTS) frames if the first frame is not already a 40 MHz PPDU.*/
        uint32_t ppdu_demotion_in_start_txop_40mhz :  1; /*10:10, This bit is applicable only when FC STA is enabled 0 �C TXOP demotions are not enabled. MAC H/W will relinquish a TXOP is the first frame in the TX queue is a 40 MHz frame and secondary channel is busy when backoff is complete in primary channel 1 �C MAC H/W will try to start a 20 MHz TXOP when secondary channel is busy and the first frame in the TXOP is a 40 MHz frame. It does this by changing the channel bandwidth of the frame from 40 MHz to 20 MHz. (In order to establish the type of TXOP, H/W senses the Secondary channel CCA one slot prior to the actual backoff slot. It decides the CH_BW of the first frame in the TXOP based on this. It validates the decision during the actual backoff slot and starts a TXOP accordingly).*/
        uint32_t ppdu_demotion_in_mid_txop_40mhz :  1; /*11:11, This bit is applicable only when FC STA is enabled 0 �C PPDU demotions are not performed. When a 20 MHz TXOP is in progress and the next frame in the TX queue is a 40 MHz frame then the TXOP is relinquished. 1 �C When a 20 MHz TXOP is in progress then MAC H/W demotes all 40 MHz PPDU to 20 MHz PPDU and continues the TXOP burst without relinquishing.*/
        uint32_t                   fc_prot_type :  1; /*12:12, The protection type specified below will be used when a protection frame exchange needs to be performed to promote the TXOP to a 40MHz TXOP. The FC protection mechanism will be used when the first frame in the TX Queue is a 20 MHz HT frame. 0 �C RTS/CTS 1 �C Self CTS*/
        uint32_t             sec_channel_offset :  1; /*13:13, 0 �C Secondary channel corresponds to Upper 20 MHz in 40 MHz Band 1 �C Secondary channel corresponds to Lower 20 MHz in 40 MHz Band (This field is not used by H/W when FC is disabled)*/
        uint32_t   first_frame_txop_limit_check :  1; /*14:14, 0 �C TXOP limit check is not done for the first frame in the TXOP 1- First frame in the TXOP is subjected to TXOP limit*/
        uint32_t             lsig_nav_update_en :  1; /*15:15, 0 �C MAC HW does not update NAV based on information from LSIG field from HT_MF frames 1 �C MAC HW updates NAV based on information from LSIG field from HT_MF frames*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HT_CTRL;

//mac_ra_ampdu_specific_lut_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t               lut_operation_en :  1; /* 0: 0, 0 to 1 transition in this signal causes H/W to start a new LUT operation. Cleared by H/W after the end of LUT operation.*/
        uint32_t                     read_write :  1; /* 1: 1, 0 �C LUT write operation 1 �C LUT read operation*/
        uint32_t                      lut_index :  8; /* 9: 2, Index to be used for the LUT operation*/
        uint32_t        mini_mpdu_start_spacing :  3; /*12:10, Minimum MPDU start spacing supported by the receiver Set to 0 for no restriction Set to 1 for 1/4 ��s Set to 2 for 1/2 ��s Set to 3 for 1 ��s Set to 4 for 2 ��s Set to 5 for 4 ��s Set to 6 for 8 ��s Set to 7 for 16 ��s*/
        uint32_t            max_rx_amdpu_factor :  2; /*14:13, The Maximum RX A-MPDU defined by this field is equal to 2(13 + Maximum Rx A-MPDU Factor) �C 1 octets. Maximum Rx A-MPDU Factor is an integer in the range 0 to 3.*/
        uint32_t                   max_num_aggr :  8; /*22:15, Maximum number of aggregations H/W will perform in an AMPDU*/
        uint32_t                   mpdu_aggr_en :  1; /*23:23, 0 �C MPDU aggregation will not be performed 1 �C MPDU aggregation will be performed*/
        uint32_t                     reserved_0 :  8; /*31:24,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RA_AMPDU_SPECIFIC_LUT_CTRL;

//mac_tx_ampdu_cnt
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, This register holds the value of the number of AMPDUs transmitted*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TX_AMPDU_CNT;

//mac_tx_mpdu_in_ampdu
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, This register holds the value of the number of MPDUs transmitted in an AMPDU*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TX_MPDU_IN_AMPDU;

//mac_tx_octets_in_ampdu
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, This register holds the number of octets in an AMPDU when an AMPDU is transmitted*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TX_OCTETS_IN_AMPDU;

//mac_max_aggr_non_ht_frames
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, This register gives the maximum number of aggregations for an AMPDU which is immediately coming after a non HT MSDU.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_MAX_AGGR_NON_HT_FRAMES;

//mac_num_20mhz_txop
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0,                         no use*/
    } bit_field;
} T_MAC_PCU_REG_MAC_NUM_20MHZ_TXOP;

//mac_num_40mhz_txop
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0,                         no use*/
    } bit_field;
} T_MAC_PCU_REG_MAC_NUM_40MHZ_TXOP;

//mac_num_20mhz_mpdu_in_40mhz_txop
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0,                         no use*/
    } bit_field;
} T_MAC_PCU_REG_MAC_NUM_20MHZ_MPDU_IN_40MHZ_TXOP;

//mac_num_promoted_mpdu
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0,                         no use*/
    } bit_field;
} T_MAC_PCU_REG_MAC_NUM_PROMOTED_MPDU;

//mac_slot_time
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       slot_sel :  1; /* 0: 0, The function of this flag depends on the current mode of operation of the STA (A/B/G). For A/B (BSS/IBSS) STA and 11G IBSS STA this flag is sampled only when PA is enabled. The selected slot time is used throughout. For an AP this flag is sampled after transmission of a Beacon. For 11G BSS STA this flag is sampled when PA is enabled. The selected flag is used as default. However the actual slot to be used is also determined by the Slot specified in the received Beacon, Probe response, Association response and Re association response frames. When this flag is zero, Short Slot time is selected. When this flag is one, Long Slot time is selected.*/
        uint32_t                short_slot_time :  5; /* 5: 1, Short Slot time in micro seconds*/
        uint32_t                 long_slot_time :  5; /*10: 6, Long Slot time in micro seconds*/
        uint32_t                     reserved_0 : 21; /*31:11,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_SLOT_TIME;

//mac_sifs_time
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t             dsss_cck_sifs_time :  5; /* 4: 0, SIFS time in microseconds for DSSS/CCK PHY*/
        uint32_t                     reserved_2 :  3; /* 7: 5,                             NA*/
        uint32_t                 ofdm_sifs_time :  5; /*12: 8, SIFS time in microseconds for OFDM PHY*/
        uint32_t                     reserved_1 :  3; /*15:13,                             NA*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_SIFS_TIME;

//mac_eifs_time
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  9; /* 8: 0, Value in microseconds to be used in EIFS timer at the end of failed frame reception (FCS error). The value to be programmed is EIFS = PHY preamble time + ACK time (at lowest basic rate of current PHY)*/
        uint32_t                     reserved_0 : 23; /*31: 9,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_EIFS_TIME;

//mac_ppdu_max_time
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, The value specifies the PPDU MAX time in micro seconds*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PPDU_MAX_TIME;

//mac_sec_channel_slot_cnt
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       slot_cnt :  8; /* 7: 0, The number of slots for which the secondary channel should be idle before a 40 MHz TXOP can be started in FC mode. The default value of 1 is equal to a PIFS time.*/
        uint32_t                       slot_sel :  2; /* 9: 8, 00 - Long Slot, 01 - Short Slot, 11 - Same as the slot time used in Primary Channel*/
        uint32_t                     reserved_0 : 22; /*31:10,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_SEC_CHANNEL_SLOT_CNT;

//mac_sifs_time2
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t             dsss_cck_sifs_time :  8; /* 7: 0, SIFS time in multiples of 0.1 microseconds for DSSS/CCK PHY*/
        uint32_t                 ofdm_sifs_time :  8; /*15: 8, SIFS time in multiples of 0.1 microseconds for OFDM PHY*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_SIFS_TIME2;

//mac_rifs_time_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t         data_path_rifs_timeout :  8; /* 7: 0, Timeout value in terms of number of PA clock cycles for which TX data path waits before discontinuing the RIFS burst*/
        uint32_t       ctrl_path_rifs_wait_time :  8; /*15: 8, The inter frame space in terms of number of PA clock cycle that the control path maintains in between frames transmitted in RIFS burst*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RIFS_TIME_CTRL;

//mac_long_retry_limit
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, Limit on number of retransmissions of MPDU/MMPDU packets of length greater than RTS Threshold.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_LONG_RETRY_LIMIT;

//mac_short_retry_limit
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, Limit on number of retransmissions of MPDU/MMPDU packets of length lesser than RTS Threshold. This applies to retransmissions of RTS frame also.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_SHORT_RETRY_LIMIT;

//mac_seqnum_dup_det_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t             table_operation_en :  1; /* 0: 0, 1 �C A zero to one transition of this bit will start the table update procedure. After table is updated this bit is reset to zero. 0 �C No action. Table updates procedure over.*/
        uint32_t                     read_write :  1; /* 1: 1, 0 �C Read from table. 1 �C Write to table.*/
        uint32_t                     reserved_3 :  1; /* 2: 2,                             NA*/
        uint32_t                     reserved_2 :  1; /* 3: 3,                             NA*/
        uint32_t                            tid :  4; /* 7: 4, Range is 0 �C 15. This is used while reading from table. The entry corresponding to the TID is read out.*/
        uint32_t                          index :  5; /*12: 8, Range is 0 �C 127. 128 STA entries are possible in the table*/
        uint32_t                     reserved_1 :  2; /*14:13,                             NA*/
        uint32_t                     reserved_0 : 17; /*31:15,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_SEQNUM_DUP_DET_CTRL;

//mac_sta_addr_high
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, 16 Higher bits of the 48-bit MAC address of the STA entry to be added to the table.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_STA_ADDR_HIGH;

//mac_sta_addr_low
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, 32 lower bits of the 48-bit MAC address of the STA entry to be added to table.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_STA_ADDR_LOW;

//mac_tx_sequence_number
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 12; /*11: 0, 12-bit sequence number read from the table.*/
        uint32_t                     reserved_0 : 20; /*31:12,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TX_SEQUENCE_NUMBER;

//mac_pcf_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       pcf_mode :  2; /* 1: 0, 00 �C Non CF-Pollable STA. 01 �C CF-Pollable STA. 10 �C Point Coordinator is resident at this STA. 11 �C Reserved.*/
        uint32_t                   piggyback_en :  1; /* 2: 2, 0 �C Subtypes containing ��+CF-Poll�� or ��+CF-Ack�� will not be used. 1 �C Above subtypes are used.*/
        uint32_t                     reserved_0 : 29; /*31: 3,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PCF_CTRL;

//mac_cfp_max_duration
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Min = 1; Max = 65535 Limit of Contention Free Period Duration.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_CFP_MAX_DURATION;

//mac_cfp_interval
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, Number of DTIM intervals between start of two CFPs.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_CFP_INTERVAL;

//mac_cfp_para_set_element_byte_num
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, This is the byte number in the beacon frame starting at which the fields of CFP Parameter Set Element have to be updated by PA.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_CFP_PARA_SET_ELEMENT_BYTE_NUM;

//mac_medium_occupancy_limit
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Limit on the continuous stretch of channel usage by this STA*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_MEDIUM_OCCUPANCY_LIMIT;

//mac_pcf_q_pointer
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of first descriptor in PCF queue. In PC, after beacon transmission that begins the CFP, packets are transmitted from this queue. CF-Poll frames and piggybacked frames are also arranged in the queue in increasing order of AID. In CF-Pollable STA, the queue contains packets that will be transmitted in response to a poll from the PC. In non-CF-Pollable STA, the queue will be empty.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PCF_Q_POINTER;

//mac_cfp_count
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, The status of CFP count in current DTIM interval*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_CFP_COUNT;

//mac_cfp_unused_duration
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, The amount of unused CFP duration in TUs*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_CFP_UNUSED_DURATION;

//mac_power_management_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                      pm_status :  1; /* 0: 0,       0 �C Active 1 �C PS Mode*/
        uint32_t                       rx_dtims :  1; /* 1: 1, 1 �C Wake up at every Listen Interval only 0 �C Wake up to receive all DTIM beacons*/
        uint32_t                     tx_pwrmgmt :  1; /* 2: 2, power management field for tx mac header*/
        uint32_t                  ps_clk_switch :  1; /* 3: 3, 0-TSF module using 40M clk, 1- TSF module switch to 32k clk*/
        uint32_t                     reserved_0 : 28; /*31: 4,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_POWER_MANAGEMENT_CTRL;

//mac_atim_win
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Size of the ATIM Window following TBTT.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_ATIM_WIN;

//mac_listen_interval
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, The station operating in the power save mode has the 16-bit listen interval stored in this register.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_LISTEN_INTERVAL;

//mac_offset_interval
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, The time in microseconds before which the TBTT interrupt is delivered at every listen interval is stored in this register.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_OFFSET_INTERVAL;

//mac_sapsd_ssp
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Starting Service Period of Scheduled APSD-SP. The QSTA wakes up for receiving packets from AP when this value matches lower 4 bytes of TSF timer and at subsequent intervals as indicated in formula, (TSF - Service Start Time) mod Minimum Service Interval = 0.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_SAPSD_SSP;

//mac_sapsd_si
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Service Interval of Scheduled APSD-SP. The QSTA wakes up for downlink transmissions from AP at times indicated in formula, (TSF - Service Start Time) mod Minimum Service Interval = 0.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_SAPSD_SI;

//mac_smps_mode
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            mcs :  8; /* 7: 0,            SMPS power save mcs*/
        uint32_t                           mode :  2; /* 9: 8, Specifies the SMPS power save mode of the current STA 0 �C Reserved 1 �C Current STA will be put in Static SMPS mode 2 �C Current STA will be put in Dynamic SMPS mode 3 �C SMPS is disabled in current STA*/
        uint32_t                     reserved_0 : 22; /*31:10,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_SMPS_MODE;

//mac_listen_interval_timer_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, When this register is written, the listen interval value is updated into the internal listen interval timer. When this value is read, the value of the internal listen interval timer value is read back*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_LISTEN_INTERVAL_TIMER_CTRL;

//mac_interrupt_status
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                    rx_complete :  1; /* 0: 0, 0 �C Interrupt not raised. 1 �C Interrupt is raised when reception of data bits is completed and transferred to Shared Memory with RX descriptor. Reception of certain frame types will not generate interrupt. This depends on RX frame filter register setting. This interrupt is raised also when memory is not available to store the received packet.*/
        uint32_t                    tx_complete :  1; /* 1: 1, 0 �C Interrupt not raised 1 �C Interrupt is raised when successful transmission occurs or when retry limit is reached or if packet lifetime expires. The status is available in the TX Descriptor of the packet.*/
        uint32_t                           tbtt :  1; /* 2: 2, 0 �C Interrupt not raised. 1 �C Interrupt is raised before beacon transmission is expected.*/
        uint32_t            hcca_txop_interrupt :  1; /* 3: 3, 0 �C Interrupt not raised. 1 �C Interrupt is raised at start of TXOP in HC, or when CF-Poll is received in QSTA. In QSTA it is raised also at end of TXOP.*/
        uint32_t                 atim_interrupt :  1; /* 4: 4, 0 �C Interrupt not raised. 1 �C Interrupt is raised at the end of ATIM window.*/
        uint32_t              wake_up_interrupt :  1; /* 5: 5, 0 �C Interrupt is not raised. 1 �C Interrupt is raised at start of S-APSD SP.*/
        uint32_t                          error :  1; /* 6: 6, 0 �C Interrupt is not raised. 1 �C Interrupt is raised when error condition occurs in Hardware. The Error code register has the cause for error. This bit is set when one of the bits is set in Error Interrupt Status register and its corresponding mask bit is not set in the Error Interrupt Mask register. To clear the interrupt, first clear the bit in the Error Interrupt Status register by writing a ?1? to it. Then clear this bit by writing a ?1? to it.*/
        uint32_t              cap_end_interrupt :  1; /* 7: 7, 0 �C Interrupt is not raised. 1 �C Interrupt is raised at the end of a CAP.*/
        uint32_t               cf_end_interrupt :  1; /* 8: 8, 0 �C Interrupt is not raised. 1 �C Interrupt is raised at the end of a CAP.*/
        uint32_t        rf_vco_unlock_interrupt :  1; /* 9: 9, 0 �C Interrupt is not raised. 1 �C Interrupt is raised when RF VCO Unlock indication is obtained through an input from RF.*/
        uint32_t      hi_priority_q_rx_complete :  1; /*10:10, 0 �C Interrupt is not raised. 1 �C Interrupt is raised at the end of reception of a frame in Hi priority RX queue*/
        uint32_t      hw_tx_abort_request_start :  1; /*11:11, 0 �C Interrupt is not raised. 1 �C External H/W trigger for TX abort is asserted*/
        uint32_t        hw_tx_abort_request_end :  1; /*12:12, 0 �C Interrupt is not raised. 1 �C External H/W trigger for TX abort is de-asserted*/
        uint32_t                 radar_detected :  1; /*13:13, 0 �C Interrupt is not raised. 1 �C Radar signal is detected*/
        uint32_t                  pa_tx_suspend :  1; /*14:14, 0 �C Interrupt is not raised. 1 �C PA went into TX suspend mode*/
        uint32_t              de_authentication :  1; /*15:15, 0 �C Interrupt is not raised. 1 �C The current STA is de-authenticated by the AP*/
        uint32_t         start_of_absent_period :  1; /*16:16, 0 �C Interrupt is not raised. 1 �C Absent period has started (P2P)*/
        uint32_t           end_of_absent_period :  1; /*17:17, 0 �C Interrupt is not raised. 1 �C Absent period has ended (P2P)*/
        uint32_t                end_of_ctwindow :  1; /*18:18, 0 �C Interrupt is not raised. 1 �C CTWindow has ended (P2P)*/
        uint32_t                     reserved_0 : 13; /*31:19,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_INTERRUPT_STATUS;

//mac_interrupt_mask
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                    rx_complete :  1; /* 0: 0, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t                    tx_complete :  1; /* 1: 1, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t                           tbtt :  1; /* 2: 2, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t            hcca_txop_interrupt :  1; /* 3: 3, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t                 atim_interrupt :  1; /* 4: 4, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t              wake_up_interrupt :  1; /* 5: 5, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t                          error :  1; /* 6: 6, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t              cap_end_interrupt :  1; /* 7: 7, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t               cf_end_interrupt :  1; /* 8: 8, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t        rf_vco_unlock_interrupt :  1; /* 9: 9, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t      hi_priority_q_rx_complete :  1; /*10:10, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t      hw_tx_abort_request_start :  1; /*11:11, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t        hw_tx_abort_request_end :  1; /*12:12, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t                 radar_detected :  1; /*13:13, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t                  pa_tx_suspend :  1; /*14:14, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t              de_authentication :  1; /*15:15, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t         start_of_absent_period :  1; /*16:16, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t           end_of_absent_period :  1; /*17:17, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t                end_of_ctwindow :  1; /*18:18, 0 �C Interrupt is not masked 1 �C Interrupt is masked*/
        uint32_t                     reserved_0 : 13; /*31:19,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_INTERRUPT_MASK;

//mac_tx_frame_pointer
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the descriptor of the frame that was transmitted and for which the current TX Complete interrupt was generated.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TX_FRAME_POINTER;

//mac_rx_frame_pointer
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the descriptor of the frame that was transmitted and for which the current RX Complete interrupt was generated. If interrupt was raised due to lack of memory then this address will have NULL pointer.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RX_FRAME_POINTER;

//mac_error_code
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  5; /* 4: 0, 0 �C NO_ERROR 
1 �C UNEXPECTED_TX_MAC_HANG 
2 �C UNEXPECTED_TX_Q_EMPTY 
3 �C TXBURSTPROC_ERROR
4 �C BUS_ERROR 
5 �C BUS_WRAP_SIG_ERROR 
10 �C UNEXPECTED_NON_PENDING_MSDU
11 �C UNEXPECTED_MSDU_ADDR 
12 �C UNEXPECTED_RX_Q_EMPTY 
13 �C TX_INTR_FIFO_OVERRUN 
14 �C RX_INTR_FIFO_OVERRUN 
15 �C HIRX_INTR_FIFO_OVERRUN 
16 �C UNEXPECTED_HIRX_Q_EMPTY 
17 �C RX_DATAPATH_HANG_FCS_FAIL_TIMEOUT 
18 �C RX_DATAPATH_HANG_FCS_PASS_TIMEOUT 
19 �C UNEXPECTED_TX_PHY_HANG Others �C Reserved*/
        uint32_t                     reserved_0 : 27; /*31: 5,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_ERROR_CODE;

//mac_tx_mpdu_cnt
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, Number of MPDUs in current TX complete interrupt*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TX_MPDU_CNT;

//mac_rx_mpdu_cnt
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, Number of MPDUs in current RX complete interrupt*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RX_MPDU_CNT;

//mac_hi_priority_rx_frame_pointer
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the RX buffer corresponding to the interrupt*/
    } bit_field;
} T_MAC_PCU_REG_MAC_HI_PRIORITY_RX_FRAME_POINTER;

//mac_de_authen_reason_code
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  4; /* 3: 0, De-authentication reason code parsed from the received de authentication frame from AP*/
        uint32_t                     reserved_0 : 28; /*31: 4,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_DE_AUTHEN_REASON_CODE;

//mac_error_interrupt_status
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_1 :  1; /* 0: 0,                             NA*/
        uint32_t                           ec_1 :  1; /* 1: 1, 0 �C Interrupt not raised. 1 �C Interrupt is raised when UNEXPECTED_TX_MAC_HANG internal error occurs.*/
        uint32_t                           ec_2 :  1; /* 2: 2, 0 �C Interrupt not raised 1 �C Interrupt is raised when UNEXPECTED_TX_Q_EMPTY internal error occurs.*/
        uint32_t                           ec_3 :  1; /* 3: 3, 0 �C Interrupt not raised. 1 �C Interrupt is raised when TXBURSTPROC_ERROR internal error occurs.*/
        uint32_t                           ec_4 :  1; /* 4: 4, 0 �C Interrupt not raised. 1 �C Interrupt is raised when BUS_ERROR internal error occurs.*/
        uint32_t                           ec_5 :  1; /* 5: 5, 0 �C Interrupt not raised. 1 �C Interrupt is raised when BUS_WRAP_SIG_ERROR internal error occurs.*/
        uint32_t                           ec_6 :  1; /* 6: 6,                   DMA_NO_ERROR*/
        uint32_t                           ec_7 :  1; /* 7: 7,                TX_CE_NOT_READY*/
        uint32_t                           ec_8 :  1; /* 8: 8,                RX_CE_NOT_READY*/
        uint32_t                           ec_9 :  1; /* 9: 9,    SEQNUM_GEN_ADDR_SEARCH_FAIL*/
        uint32_t                          ec_10 :  1; /*10:10, 0 �C Interrupt is not raised. 1 �C Interrupt is raised when UNEXPECTED_NON_PENDING_MSDU internal error occurs.*/
        uint32_t                          ec_11 :  1; /*11:11, 0 �C Interrupt is not raised. 1 �C Interrupt is raised when UNEXPECTED_MSDU_ADDR internal error occurs.*/
        uint32_t                          ec_12 :  1; /*12:12, 0 �C Interrupt is not raised. 1 �C Interrupt is raised when UNEXPECTED_RX_Q_EMPTY internal error occurs.*/
        uint32_t                          ec_13 :  1; /*13:13, 0 �C Interrupt is not raised. 1 �C Interrupt is raised when TX_INTR_FIFO_OVERRUN internal error occurs.*/
        uint32_t                          ec_14 :  1; /*14:14, 0 �C Interrupt is not raised. 1 �C Interrupt is raised when RX_INTR_FIFO_OVERRUN internal error occurs.*/
        uint32_t                          ec_15 :  1; /*15:15, 0 �C Interrupt is not raised. 1 �C Interrupt is raised when HIRX_INTR_FIFO_OVERRUN internal error occurs.*/
        uint32_t                          ec_16 :  1; /*16:16, 0 �C Interrupt is not raised. 1 �C Interrupt is raised when UNEXPECTED_HIRX_Q_EMPTY internal error occurs.*/
        uint32_t                          ec_17 :  1; /*17:17, 0 �C Interrupt is not raised. 1 �C Interrupt is raised when RX_DATAPATH_HANG_FCS_FAIL_TIMEOUT internal error occurs*/
        uint32_t                          ec_18 :  1; /*18:18, 0 �C Interrupt is not raised. 1 �C Interrupt is raised when RX_DATAPATH_HANG_FCS_PASS_TIMEOUT internal error occurs.*/
        uint32_t                          ec_19 :  1; /*19:19, 0 �C Interrupt not raised. 1 �C Interrupt is raised when UNEXPECTED_TX_PHY_HANG internal error occurs.*/
        uint32_t                     reserved_0 : 12; /*31:20,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_ERROR_INTERRUPT_STATUS;

//mac_error_interrupt_mask
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_1 :  1; /* 0: 0,                             NA*/
        uint32_t                           ec_1 :  1; /* 1: 1, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                           ec_2 :  1; /* 2: 2, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                           ec_3 :  1; /* 3: 3, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                           ec_4 :  1; /* 4: 4, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                           ec_5 :  1; /* 5: 5, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                           ec_6 :  1; /* 6: 6, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                           ec_7 :  1; /* 7: 7, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                           ec_8 :  1; /* 8: 8, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                           ec_9 :  1; /* 9: 9, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                          ec_10 :  1; /*10:10, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                          ec_11 :  1; /*11:11, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                          ec_12 :  1; /*12:12, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                          ec_13 :  1; /*13:13, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                          ec_14 :  1; /*14:14, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                          ec_15 :  1; /*15:15, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                          ec_16 :  1; /*16:16, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                          ec_17 :  1; /*17:17, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                          ec_18 :  1; /*18:18, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                          ec_19 :  1; /*19:19, 0-interrupt not masked
1-interrupt is masked*/
        uint32_t                     reserved_0 : 12; /*31:20,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_ERROR_INTERRUPT_MASK;

//mac_phy_spi_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t              phy_register_data :  8; /* 7: 0, Write mode - written into the PHY register bank. Read mode - will be filled with the data from PHY register bank*/
        uint32_t        phy_register_read_write :  1; /* 8: 8, 0 - Indicates the write operation to be initiated. 1 �C Indicates the read operation to be initiated*/
        uint32_t              phy_register_addr :  8; /*16: 9, 8 - bit field to specify the address of the register in PHY register bank.*/
        uint32_t                     reserved_0 : 15; /*31:17,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PHY_SPI_CTRL;

//mac_phy_spi_lock
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  1; /* 0: 0, 0 �C Software can access PHY registers through SPI. PA will not have access. 1 �C Software cannot access PHY registers through SPI. PA will have the access.*/
        uint32_t                     reserved_0 : 31; /*31: 1,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PHY_SPI_LOCK;

//mac_phy_spi_status
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       spi_busy :  1; /* 0: 0, 0 �C SPI is ready for another read/write. 1 �C SPI is busy with current transaction.*/
        uint32_t                     reserved_0 : 31; /*31: 1,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PHY_SPI_STATUS;

//mac_rf_vco_spi_ctrl_width
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  5; /* 4: 0, The width of control word to program the VCO in the RF Section.*/
        uint32_t                     reserved_0 : 27; /*31: 5,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RF_VCO_SPI_CTRL_WIDTH;

//mac_rf_vco_spi_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, The control word to program the VCO in the RF Section.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RF_VCO_SPI_CTRL;

//mac_rf_vco_spi_status
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                    rf_spi_busy :  1; /* 0: 0, 0 �C RF SPI is ready for another read/write. 1 �C RF SPI is busy with current transaction. This is set when RF VCO SPI Control register is written.*/
        uint32_t                     reserved_0 : 31; /*31: 1,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RF_VCO_SPI_STATUS;

//mac_phy_11a_reg_base_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, Base address of 11a registers in PHY register bank. Used only in case of non-MIMO Interface.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PHY_11A_REG_BASE_ADDR;

//mac_phy_11b_reg_base_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, Base address of 11b registers in PHY register bank. Used only in case of non-MIMO Interface.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PHY_11B_REG_BASE_ADDR;

//mac_phy_11g_reg_base_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, Base address of 11g registers in PHY register bank. Used only in case of non-MIMO Interface.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PHY_11G_REG_BASE_ADDR;

//mac_phy_rf_reg_base_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value :  8; /* 7: 0, Base address of RF registers in PHY register bank.*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PHY_RF_REG_BASE_ADDR;

//mac_tx_plcp_delay
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t         dsss_cck_tx_plcp_delay :  4; /* 3: 0, The delay in the DSSS/CCK PHY transmit path in microseconds.*/
        uint32_t             ofdm_tx_plcp_delay :  4; /* 7: 4, The delay in the OFDM PHY transmit path in microseconds.*/
        uint32_t               ht_tx_plcp_delay :  4; /*11: 8, The delay in the HT PHY transmit path in micro seconds.*/
        uint32_t                     reserved_0 : 20; /*31:12,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TX_PLCP_DELAY;

//mac_rx_plcp_delay
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t         dsss_cck_tx_plcp_delay :  4; /* 3: 0, The delay in the DSSS/CCK PHY receive path in microseconds.*/
        uint32_t             ofdm_tx_plcp_delay :  4; /* 7: 4, The delay in the OFDM PHY receive path in microseconds.*/
        uint32_t               ht_tx_plcp_delay :  4; /*11: 8, The delay in the HT PHY receive path in micro seconds.*/
        uint32_t                     reserved_0 : 20; /*31:12,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RX_PLCP_DELAY;

//mac_rxtx_turnaround_time
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       rx_to_tx :  8; /* 7: 0, Time in multiples of 0.1 microseconds for PHY to switch from RX mode to TX mode plus the TX PLCP delay. This is the time delay between assertion of TX_EN~ signal and the first preamble sample coming out of the antenna. This time is assumed to be constant for all data rates and modulation classes. The time to send control header is factored into this value in MAC.*/
        uint32_t                       tx_to_rx :  8; /*15: 8, Time in multiples of 0.1 microseconds for PHY to switch from TX mode to RX mode. This is the time after the end of transmission after which the Primary channel CCA signal will indicate the actual status of the medium.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RXTX_TURNAROUND_TIME;

//mac_phy_timeout_adjust
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_1 :  8; /* 7: 0,                             NA*/
        uint32_t            watchdog_timeout_en :  1; /* 8: 8, This enables the timer for Data Path reset.*/
        uint32_t         watchdog_timeout_value :  8; /*16: 9, The timeout value programmed by S/W. After the timeout the data path of MACHW is reset. The timeout is declared in us*/
        uint32_t                     reserved_0 : 15; /*31:17,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PHY_TIMEOUT_ADJUST;

//mac_phy_reg_access_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t            phy_reg_access_flag :  1; /* 0: 0, This flag acts as a control signal which initiates a register access and also as a status flag. A zero to one transition in this field starts a new register access. This bit is reset to zero by hardware after the register access is complete.*/
        uint32_t                     read_write :  1; /* 1: 1, This flag indicates whether the requested register access is of read/write type. 0 �C Write operation 1 �C Read operation*/
        uint32_t                   phy_reg_addr :  8; /* 9: 2, PHY address to be used for the register access.*/
        uint32_t                     reserved_0 : 22; /*31:10,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PHY_REG_ACCESS_CTRL;

//mac_phy_reg_access_data
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                read_write_data : 32; /*31: 0, When the PHY register transfer is of write type then this register contains the data to be written into the PHY. When the PHY register transfer is of read type then this register contains the data read from PHY.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PHY_REG_ACCESS_DATA;

//mac_phy_tx_service
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t             ofdm_service_field : 16; /*15: 0, TX Service field for OFDM frames (legacy as well as HT frames)*/
        uint32_t             dsss_service_field : 16; /*31:16, TX Service field for DSSS frames*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PHY_TX_SERVICE;

//mac_tx_power_level
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t            dsss_tx_power_level :  8; /* 7: 0, Power level to be used for DSSS frames. This value is used for SIFS response frames.*/
        uint32_t            ofdm_tx_power_level :  8; /*15: 8, Power level to be used for OFDM frames. This value is used for SIFS response frames.*/
        uint32_t              ht_tx_power_level :  8; /*23:16, Power level to be used for HT frames. This value is used for SIFS response frames.*/
        uint32_t                     reserved_0 :  8; /*31:24,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TX_POWER_LEVEL;

//mac_phy_cca_delay
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t           delay_in_pri_channel :  8; /* 7: 0, CCA Delay of PHY in Primary channel. The time is specified as a multiple of 0.1 micro seconds*/
        uint32_t           delay_in_sec_channel :  8; /*15: 8, CCA Delay of PHY in Secondary channel. The time is specified as a multiple of 0.1 micro seconds*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PHY_CCA_DELAY;

//mac_tx_plcp_adjust
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t        dsss_cck_tx_plcp_adjust :  8; /* 7: 0, The minimum number of TX_CLK cycles between the assertion of TX_EN~ signal and the start of TX control header transfer during DSSS/CCK frame transmission*/
        uint32_t      non_ht_ofdm_txplcp_adjust :  8; /*15: 8, The minimum number of TX_CLK cycles between the assertion of TX_EN~ signal and the start of TX control header transfer during non-HT OFDM frame transmission*/
        uint32_t         ht_ofdm_txplcp1_adjust :  8; /*23:16, The minimum number of TX_CLK cycles between the assertion of TX_EN~ signal and the start of TX control header transfer during HT frame transmission*/
        uint32_t         ht_ofdm_txplcp2_adjust :  8; /*31:24, The minimum number of TX_CLK cycles between the assertion of TX_RST~ signal and the start of TX control header transfer during HT frame transmission*/
    } bit_field;
} T_MAC_PCU_REG_MAC_TX_PLCP_ADJUST;

//mac_rx_plcp_delay2
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t         dsss_cck_rx_plcp_delay :  8; /* 7: 0, The delay in the DSSS/CCK PHY receive path in multiples of 0.1 microseconds.*/
        uint32_t       non_ht_ofdm_rxplcp_delay :  8; /*15: 8, The delay in the OFDM PHY receive path in multiples of 0.1 microseconds.*/
        uint32_t           ht_ofdm_rxplcp_delay :  8; /*23:16, The delay in the HT OFDM PHY receive path in multiples of 0.1 micro seconds.*/
        uint32_t                     reserved_0 :  8; /*31:24,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RX_PLCP_DELAY2;

//mac_rx_start_delay
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t dsss_phy_rx_start_long_preamble :  8; /* 7: 0, Delay in micro seconds for the 11b PHY to give Rx start indication after the reception of the Long Preamble*/
        uint32_t dsss_phy_rx_start_short_preamble :  8; /*15: 8, Delay in micro seconds for the 11b PHY to give Rx Start indication after the reception of the short Preamble*/
        uint32_t        ofdm_phy_rx_start_delay :  8; /*23:16, Delay in micro seconds for the 11a PHY to give Rx start indication after the reception of the preamble*/
        uint32_t          ht_phy_tx_start_delay :  8; /*31:24, Delay in micro seconds for the HT PHY to give Rx start indication after the reception of HT or GF preamble*/
    } bit_field;
} T_MAC_PCU_REG_MAC_RX_START_DELAY;

//mac_antenna_set
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                           nss1 :  8; /* 7: 0, The antenna set that should be used in the Tx PHY mode for frames of data rate 11b, 11a and MCS 0-7 and MCS 32*/
        uint32_t                           nss2 :  8; /*15: 8, The antenna set that should be used in the Tx PHY mode for frames of data rate MCS 8-15 and MCS 33-38*/
        uint32_t                           nss3 :  8; /*23:16, The antenna set that should be used in the Tx PHY mode for frames of data rate MCS 16-23 and MCS 39-52*/
        uint32_t                           nss4 :  8; /*31:24, The antenna set that should be used in the Tx PHY mode for frames of data rate MCS 24-31 and MCS 53-76*/
    } bit_field;
} T_MAC_PCU_REG_MAC_ANTENNA_SET;

//mac_oddr_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t              invert_phy_tx_clk :  1; /* 0: 0, 0 �C Do not invert Phy Tx Clk 1 �C Invert Phy Tx Clk*/
        uint32_t           invert_phy_regif_clk :  1; /* 1: 1, 0 �C Do not invert Phy RegIf Clk 1 �C Invert Phy RegIf Clk*/
        uint32_t                     reserved_0 : 30; /*31: 2,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_ODDR_CTRL;

//mac_peer_station_addr_msb
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Bits 47 �C 32 of the peer station address with which block Ack mechanism is established. When read operation is enabled, HW will read the address programmed in LUT for given index and put bits 47 �C 32 of the read address in this REG.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PEER_STATION_ADDR_MSB;

//mac_peer_station_addr_lsb
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Bits 31 �C 0 of the peer station address with which block Ack mechanism is established. When read operation is enabled, HW will read the address programmed in LUT for given index and put bits 31 �C 0 of the read address in this REG.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PEER_STATION_ADDR_LSB;

//mac_sequence_number_tid
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            tid :  4; /* 3: 0, The TID for which the block Ack mechanism is established. When read operation is enabled, HW will write the TID read from LUT in this REG.*/
        uint32_t                             sn : 12; /*15: 4, Starting sequence number of data frame to be received from peer station under block Ack establishment. For read operation this is the sequence number read from LUT.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_SEQUENCE_NUMBER_TID;

//mac_ba_frame_header_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Address of the memory where BA frame header is buffered. Used for uncompressed bitmap BA.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_BA_FRAME_HEADER_ADDR;

//mac_bitmap_location_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Pointer to the location for maintaining bitmap (received frame status). The HW will read bitmap address from LUT and put in this REG when read operation is enabled. Used for compressed bitmap BA.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_BITMAP_LOCATION_ADDR;

//mac_ba_lut_update
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         lut_en :  1; /* 0: 0, This bit, when set will enable LUT updating operation. HW will clear this bit after updating LUT.*/
        uint32_t                  lut_operation :  2; /* 2: 1, 00 �� LUT write operation. 01 �� LUT read operation. 10 �� Entry removal operation. 11 �� Reserved.*/
        uint32_t                      lut_index :  4; /* 6: 3,     Index of LUT for updating.*/
        uint32_t                     reserved_0 : 25; /*31: 7,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_BA_LUT_UPDATE;

//mac_ba_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          ba_en :  1; /* 0: 0, 0 �C Feature is disabled 1 �C Feature is enabled*/
        uint32_t                         lut_en :  1; /* 1: 1, This bit, when set will enable LUT updating operation. HW will clear this bit after updating LUT. 0 �C Idle. 1 �C Will trigger LUT operation.*/
        uint32_t                  lut_operation :  1; /* 2: 2, 1 �� Write entry 0 �� Read entry.*/
        uint32_t                      lut_index :  4; /* 6: 3,     Index of LUT for updating.*/
        uint32_t                     reserved_0 : 25; /*31: 7,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_BA_CTRL;

//mac_peer_sta_addr_msb
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, Bits 47 �C 32 of the peer station address with which block Ack mechanism is established.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PEER_STA_ADDR_MSB;

//mac_peer_sta_addr_lsb
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Bits 31 �C 0 of the peer station address with which block Ack mechanism is established.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_PEER_STA_ADDR_LSB;

//mac_ba_para_size
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            tid :  4; /* 3: 0, The TID for which the block Ack mechanism is established.*/
        uint32_t                             sn : 12; /*15: 4, Starting sequence number of data frame to be received from peer station under block Ack establishment.*/
        uint32_t                  partial_state :  6; /*21:16, Indicates if the state mode is partial or full. 0 �� Full state operation. 1 �� Partial state operation.*/
        uint32_t                     reserved_1 :  2; /*23:22,                             NA*/
        uint32_t                       win_size :  1; /*24:24, Window size of the BA session in terms of number of MSDUs. This is used to determine the region of sequence number comparisons. Maximum is 64 and minimum is 1.*/
        uint32_t                     reserved_0 :  7; /*31:25,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_BA_PARA_SIZE;

//mac_ba_compressed_bitmap_msw
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Bits 63 �C 32 of the bitmap for the BA session.. When read operation is enabled, HW will read the bitmap in LUT for given index and put bits 63 �C 32 of the read value in this register.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_BA_COMPRESSED_BITMAP_MSW;

//mac_ba_compressed_bitmap_lsw
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Bits 31 �C 0 of the bitmap for the BA session.. When read operation is enabled, HW will read the bitmap in LUT for given index and put bits 31 �C 0 of the read value in this register.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_BA_COMPRESSED_BITMAP_LSW;

//mac_schedule_link_addr
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, Pointer to the schedule linked list.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_SCHEDULE_LINK_ADDR;

//mac_cap_start_time
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t         start_time_before_tbtt :  8; /* 7: 0, CAP start time in milliseconds expressed as the time before the next TBTT. For example if this value is set to 10 then a CAP will start 10 ms before a TBTT.*/
        uint32_t                     reserved_1 :  8; /*15: 8,                             NA*/
        uint32_t          pre_start_offset_time :  8; /*23:16, CAP pre-start time in microseconds. This time represents the time before the actual CAP start during which the Protocol accelerator prepares for a CAP.*/
        uint32_t                     reserved_0 :  8; /*31:24,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_CAP_START_TIME;

//mac_p2p_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         p2p_en :  1; /* 0: 0, 0 �C P2P is not enabled. 1 �C P2P is enabled.*/
        uint32_t                         ops_en :  1; /* 1: 1, 0 �C Opportunistic power save is not enabled. 1 �C Opportunistic power save is enabled.*/
        uint32_t                           mode :  1; /* 2: 2,   0 �C Group Owner 1 �C Client*/
        uint32_t          ctwindow_txop_allowed :  1; /* 3: 3, 0 �C TXOP is not allowed to cross the end of CTWindow. 1 �C TXOP is allowed to cross the end of CTWindow.*/
        uint32_t                       ctwindow :  8; /*11: 4, Client Traffic Window (CTWindow). A period of time in TU after a TBTT during which the P2P Group Owner is present. 0 indicates that there shall be no CTWindow.*/
        uint32_t              rx_beacon_timeout : 16; /*27:12, The timeout value in the case where client does not receive GO?s beacon.*/
        uint32_t                     cancel_noa :  1; /*28:28, 0 �C No action 1 �C Immediately stops existing NOA schedules and HW updates their status as ��CANCELLED��.*/
        uint32_t                     reserved_0 :  3; /*31:29,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_P2P_CTRL;

//mac_noa_count_status
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       noa1_cnt :  8; /* 7: 0, Count in Notice of Absence Descriptors sent by a P2P Group Owner; indicates the number of absence intervals. 255 shall mean a continuous schedule; 0 is reserved and shall not be used.*/
        uint32_t                       noa2_cnt :  8; /*15: 8, Count in Notice of Absence Descriptors sent by a P2P Group Owner; indicates the number of absence intervals. 255 shall mean a continuous schedule; 0 is reserved and shall not be used.*/
        uint32_t                    noa1_status :  2; /*17:16, Status of NOA1 schedule descriptor. SW should set it to ��NEW�� when programming a new descriptor. 00 �C EXPIRED 01 �C CURRENT 10 �C NEW 11 �C CANCELLED*/
        uint32_t                    noa2_status :  2; /*19:18, Status of NOA2 schedule descriptor. SW should set it to ��NEW�� when programming a new descriptor. 00 �C EXPIRED 01 �C CURRENT 10 �C NEW 11 �C CANCELLED*/
        uint32_t                     reserved_0 : 12; /*31:20,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_NOA_COUNT_STATUS;

//mac_noa1_duration
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       duration : 32; /*31: 0, Indicates the maximum duration in units of microseconds that the P2P Group Owner can remain absent following the start of a Notice of Absence*/
    } bit_field;
} T_MAC_PCU_REG_MAC_NOA1_DURATION;

//mac_noa1_interval
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       interval : 32; /*31: 0, Indicates the length of the Notice of Absence interval in units of microseconds.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_NOA1_INTERVAL;

//mac_noa1_start_time
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, The start time for the schedule expressed in terms of the lower 4 bytes of the TSF timer.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_NOA1_START_TIME;

//mac_noa2_duration
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       duration : 32; /*31: 0, Indicates the maximum duration in units of microseconds that the P2P Group Owner can remain absent following the start of a Notice of Absence interval.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_NOA2_DURATION;

//mac_noa2_interval
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       interval : 32; /*31: 0, Indicates the length of the Notice of Absence interval in units of microseconds.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_NOA2_INTERVAL;

//mac_noa2_start_time
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 32; /*31: 0, The start time for the schedule expressed in terms of the lower 4 bytes of the TSF timer.*/
    } bit_field;
} T_MAC_PCU_REG_MAC_NOA2_START_TIME;

//mac_eoa_offset
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          value : 16; /*15: 0, The time in microseconds before which the End of Absent Period interrupt is generated before actual End of Absence of the schedule.*/
        uint32_t                     reserved_0 : 16; /*31:16,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_EOA_OFFSET;

//mac_p2p_status
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                  noa1_progress :  1; /* 0: 0, 0 - NOA schedule-1 absent period is not in progress 1 - NOA schedule-1 absent period is in progress*/
        uint32_t                  noa1_periodic :  1; /* 1: 1, 0 - NOA schedule-1 absent period is periodic
1 - NOA schedule-1 absent period is non-periodic*/
        uint32_t                  noa2_progress :  1; /* 2: 2, 0 - NOA schedule-2 absent period is not in progress 1 - NOA schedule-2 absent period is in progress*/
        uint32_t                  noa2_periodic :  1; /* 3: 3, 0 - NOA schedule-2 absent period is periodic 1 - NOA schedule-2 absent period is non-periodic*/
        uint32_t              ctwindow_progress :  1; /* 4: 4, 0 - CTWindow is not in progress 1 - CTWindow is in progress*/
        uint32_t                     reserved_0 : 27; /*31: 5,                             NA*/
    } bit_field;
} T_MAC_PCU_REG_MAC_P2P_STATUS;

//Registers Mapping to the same address

typedef struct
{
    volatile    T_MAC_PCU_REG_MAC_PA_VERSION                 mac_pa_version; /*  0x0,    RO, 0x01041600,                             NA*/
    volatile       T_MAC_PCU_REG_MAC_PA_CTRL                    mac_pa_ctrl; /*  0x4,    RW, 0x00000000,                             NA*/
    volatile     T_MAC_PCU_REG_MAC_PA_STATUS                  mac_pa_status; /*  0x8,    RO, 0x00000000,                             NA*/
    volatile  T_MAC_PCU_REG_MAC_ADDRESS_HIGH               mac_address_high; /*  0xc,    RW, 0x00000000,                             NA*/
    volatile   T_MAC_PCU_REG_MAC_ADDRESS_LOW                mac_address_low; /* 0x10,    RW, 0x00000000,                             NA*/
    volatile    T_MAC_PCU_REG_MAC_BSSID_HIGH                 mac_bssid_high; /* 0x14,    RW, 0x00000000,                             NA*/
    volatile     T_MAC_PCU_REG_MAC_BSSID_LOW                  mac_bssid_low; /* 0x18,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PRBS_SEED_VALUE            mac_prbs_seed_value; /* 0x1c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_DMA_BURST_SIZE             mac_dma_burst_size; /* 0x20,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TX_RX_COMPLETE_CNT         mac_tx_rx_complete_cnt; /* 0x24,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PRBS_READ_VALUE            mac_prbs_read_value; /* 0x28,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_NULL_FRAME_RATE            mac_null_frame_rate; /* 0x2c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_NULL_FRAME_PHY_TX_MODE     mac_null_frame_phy_tx_mode; /* 0x30,    RW, 0x00000000,                             NA*/
    volatile     T_MAC_PCU_REG_MAC_TEST_MODE                  mac_test_mode; /* 0x34,    RW, 0x00000000,                             NA*/
    volatile         T_MAC_PCU_REG_MAC_HW_ID                      mac_hw_id; /* 0x38,    RO, 0x4E4D4143,                             NA*/
    volatile T_MAC_PCU_REG_MAC_HW_RESET_CTRL              mac_hw_reset_ctrl; /* 0x3c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TX_ABORT_DURATION_TIMEOUT  mac_tx_abort_duration_timeout; /* 0x40,    RW, 0x0FFF0FFF,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TX_ABORT_FRAME_RATE        mac_tx_abort_frame_rate; /* 0x44,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TX_ABORT_FRAME_PHY_TX_MODE mac_tx_abort_frame_phy_tx_mode; /* 0x48,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_EXTENDED_PA_CTRL           mac_extended_pa_ctrl; /* 0x4c,    RW, 0x00000000,                             NA*/
    volatile                        uint32_t                 reserved_0[12];
    volatile T_MAC_PCU_REG_MAC_RX_FRAME_FILTER            mac_rx_frame_filter; /* 0x80,    RW, 0x00000000,                             NA*/
    volatile    T_MAC_PCU_REG_MAC_FRAME_CTRL                 mac_frame_ctrl; /* 0x84,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RX_BUFFER_ADDR             mac_rx_buffer_addr; /* 0x88,    RW, 0x00000000,                             NA*/
    volatile  T_MAC_PCU_REG_MAC_FCS_FAIL_CNT               mac_fcs_fail_cnt; /* 0x8c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RX_MAX_FRAME_LENGTH_FILTER mac_rx_max_frame_length_filter; /* 0x90,    RW, 0x00000834,                             NA*/
    volatile T_MAC_PCU_REG_MAC_DUPLICATE_FRAME_CNT        mac_duplicate_frame_cnt; /* 0x94,    RW, 0x00000000,                             NA*/
    volatile    T_MAC_PCU_REG_MAC_RX_END_CNT                 mac_rx_end_cnt; /* 0x98,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RX_END_ERROR_CNT           mac_rx_end_error_cnt; /* 0x9c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_AMPDU_RXED_CNT             mac_ampdu_rxed_cnt; /* 0xa0,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_MPDU_IN_AMPDU_CNT          mac_mpdu_in_ampdu_cnt; /* 0xa4,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RXED_OCTETS_IN_AMPDU       mac_rxed_octets_in_ampdu; /* 0xa8,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_AMPDU_DEL_CRC_ERROR_CNT    mac_ampdu_del_crc_error_cnt; /* 0xac,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_DEFRAG_LIFETIME_LIMIT      mac_defrag_lifetime_limit; /* 0xb0,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_HI_PRIORITY_RX_Q_BUFFER_ADDR mac_hi_priority_rx_q_buffer_addr; /* 0xb4,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_HI_PRIORITY_RX_Q_CTRL      mac_hi_priority_rx_q_ctrl; /* 0xb8,    RW, 0x00000000,                             NA*/
    volatile  T_MAC_PCU_REG_MAC_SUB_MSDU_GAP               mac_sub_msdu_gap; /* 0xbc,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_MAC_RX_BUFFER_LENGTH       mac_mac_rx_buffer_length; /* 0xc0,    RW, 0x00000640,                             NA*/
    volatile                        uint32_t                 reserved_1[15];
    volatile         T_MAC_PCU_REG_MAC_AIFSN                      mac_aifsn; /*0x100,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_CWMINMAX_AC_BK             mac_cwminmax_ac_bk; /*0x104,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_CWMINMAX_AC_BE             mac_cwminmax_ac_be; /*0x108,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_CWMINMAX_AC_VI             mac_cwminmax_ac_vi; /*0x10c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_CWMINMAX_AC_VO             mac_cwminmax_ac_vo; /*0x110,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_EDCA_TXOP_LIMIT_AC_BK_BE   mac_edca_txop_limit_ac_bk_be; /*0x114,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_EDCA_TXOP_LIMIT_AC_VI_VO   mac_edca_txop_limit_ac_vi_vo; /*0x118,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_AC_BK_FIRST_FRAME_POINTER  mac_ac_bk_first_frame_pointer; /*0x11c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_AC_BK_Q_RETRY_CNTS         mac_ac_bk_q_retry_cnts; /*0x120,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_AC_BE_FIRST_FRAME_POINTER  mac_ac_be_first_frame_pointer; /*0x124,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_AC_BE_Q_RETRY_CNTS         mac_ac_be_q_retry_cnts; /*0x128,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_AC_VI_FIRST_FRAME_POINTER  mac_ac_vi_first_frame_pointer; /*0x12c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_AC_VI_Q_RETRY_CNTS         mac_ac_vi_q_retry_cnts; /*0x130,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_AC_VO_FIRST_FRAME_POINTER  mac_ac_vo_first_frame_pointer; /*0x134,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_AC_VO_Q_RETRY_CNTS         mac_ac_vo_q_retry_cnts; /*0x138,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_HI_PRIORIY_FIRST_FRAME_POINTER mac_hi_prioriy_first_frame_pointer; /*0x13c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_MSDU_MAX_TX_LIFETIME_LIMIT mac_msdu_max_tx_lifetime_limit; /*0x140,    RW, 0x0000FFFF,                             NA*/
    volatile T_MAC_PCU_REG_MAC_AC_BE_BK_EDCA_LIFETIME_LIMIT mac_ac_be_bk_edca_lifetime_limit; /*0x144,    RW, 0xFFFFFFFF,                             NA*/
    volatile T_MAC_PCU_REG_MAC_AC_VO_VI_EDCA_LIFETIME_LIMIT mac_ac_vo_vi_edca_lifetime_limit; /*0x148,    RW, 0xFFFFFFFF,                             NA*/
    volatile                        uint32_t                 reserved_2[13];
    volatile T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY0_FIRST_FRAME_ADDR mac_hcca_sta_priority0_first_frame_addr; /*0x180,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY1_FIRST_FRAME_ADDR mac_hcca_sta_priority1_first_frame_addr; /*0x184,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY2_FIRST_FRAME_ADDR mac_hcca_sta_priority2_first_frame_addr; /*0x188,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY3_FIRST_FRAME_ADDR mac_hcca_sta_priority3_first_frame_addr; /*0x18c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY4_FIRST_FRAME_ADDR mac_hcca_sta_priority4_first_frame_addr; /*0x190,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY5_FIRST_FRAME_ADDR mac_hcca_sta_priority5_first_frame_addr; /*0x194,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY6_FIRST_FRAME_ADDR mac_hcca_sta_priority6_first_frame_addr; /*0x198,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY7_FIRST_FRAME_ADDR mac_hcca_sta_priority7_first_frame_addr; /*0x19c,    RW, 0x00000000,                             NA*/
    volatile                        uint32_t                 reserved_3[24];
    volatile      T_MAC_PCU_REG_MAC_TSF_CTRL                   mac_tsf_ctrl; /*0x200,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TSF_TIMER_READ_VALUE_HIGH  mac_tsf_timer_read_value_high; /*0x204,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TSF_TIMER_READ_VALUE_LOW   mac_tsf_timer_read_value_low; /*0x208,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_BEACON_PERIOD              mac_beacon_period; /*0x20c,    RW, 0x00000000,                             NA*/
    volatile   T_MAC_PCU_REG_MAC_DTIM_PERIOD                mac_dtim_period; /*0x210,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_BEACON_PACKET_POINTER      mac_beacon_packet_pointer; /*0x214,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_BEACON_TX_PARAM            mac_beacon_tx_param; /*0x218,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_DTIM_COUNT_STATUS          mac_dtim_count_status; /*0x21c,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_DTIM_COUNT_UPDATE_CTRL     mac_dtim_count_update_ctrl; /*0x220,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_BEACON_PHY_TX_MODE         mac_beacon_phy_tx_mode; /*0x224,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TBTT_TIMER_CTRL            mac_tbtt_timer_ctrl; /*0x228,    RW, 0x00000000,                             NA*/
    volatile                        uint32_t                 reserved_4[21];
    volatile T_MAC_PCU_REG_MAC_PROTECTION_CTRL            mac_protection_ctrl; /*0x280,    RW, 0x00000000,                             NA*/
    volatile    T_MAC_PCU_REG_MAC_RTS_THRESH                 mac_rts_thresh; /*0x284,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PROTECTION_DATA_RATE       mac_protection_data_rate; /*0x288,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TXOP_HOLDER_ADDR_LOW       mac_txop_holder_addr_low; /*0x28c,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TXOP_HOLDER_ADDR_HIGH      mac_txop_holder_addr_high; /*0x290,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RESP_RATE_LUT0             mac_resp_rate_lut0; /*0x294,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RESP_RATE_LUT1             mac_resp_rate_lut1; /*0x298,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_FRAGMENTATION_CTRL         mac_fragmentation_ctrl; /*0x29c,    RW, 0x01000100,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PROTECTION_TX_MODE         mac_protection_tx_mode; /*0x2a0,    RW, 0x00000000,                             NA*/
    volatile       T_MAC_PCU_REG_MAC_HT_CTRL                    mac_ht_ctrl; /*0x2a4,    RW, 0x00008000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RA_AMPDU_SPECIFIC_LUT_CTRL mac_ra_ampdu_specific_lut_ctrl; /*0x2a8,    RW, 0x00000000,                             NA*/
    volatile  T_MAC_PCU_REG_MAC_TX_AMPDU_CNT               mac_tx_ampdu_cnt; /*0x2ac,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TX_MPDU_IN_AMPDU           mac_tx_mpdu_in_ampdu; /*0x2b0,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TX_OCTETS_IN_AMPDU         mac_tx_octets_in_ampdu; /*0x2b4,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_MAX_AGGR_NON_HT_FRAMES     mac_max_aggr_non_ht_frames; /*0x2b8,    RW, 0x0000000B,                             NA*/
    volatile T_MAC_PCU_REG_MAC_NUM_20MHZ_TXOP             mac_num_20mhz_txop; /*0x2bc,    RW, 0x00000000,                             NA*/
    volatile                        uint32_t                  reserved_5[4];
    volatile T_MAC_PCU_REG_MAC_NUM_40MHZ_TXOP             mac_num_40mhz_txop; /*0x2d0,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_NUM_20MHZ_MPDU_IN_40MHZ_TXOP mac_num_20mhz_mpdu_in_40mhz_txop; /*0x2d4,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_NUM_PROMOTED_MPDU          mac_num_promoted_mpdu; /*0x2d8,    RW, 0x00000000,                             NA*/
    volatile                        uint32_t                  reserved_6[9];
    volatile     T_MAC_PCU_REG_MAC_SLOT_TIME                  mac_slot_time; /*0x300,    RW, 0x00000000,                             NA*/
    volatile     T_MAC_PCU_REG_MAC_SIFS_TIME                  mac_sifs_time; /*0x304,    RW, 0x00000000,                             NA*/
    volatile     T_MAC_PCU_REG_MAC_EIFS_TIME                  mac_eifs_time; /*0x308,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PPDU_MAX_TIME              mac_ppdu_max_time; /*0x30c,    RW, 0x0000FFFF,                             NA*/
    volatile T_MAC_PCU_REG_MAC_SEC_CHANNEL_SLOT_CNT       mac_sec_channel_slot_cnt; /*0x310,    RW, 0x00000101,                             NA*/
    volatile    T_MAC_PCU_REG_MAC_SIFS_TIME2                 mac_sifs_time2; /*0x314,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RIFS_TIME_CTRL             mac_rifs_time_ctrl; /*0x318,    RW, 0x00000050,                             NA*/
    volatile                        uint32_t                 reserved_7[25];
    volatile T_MAC_PCU_REG_MAC_LONG_RETRY_LIMIT           mac_long_retry_limit; /*0x380,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_SHORT_RETRY_LIMIT          mac_short_retry_limit; /*0x384,    RW, 0x00000000,                             NA*/
    volatile                        uint32_t                 reserved_8[30];
    volatile T_MAC_PCU_REG_MAC_SEQNUM_DUP_DET_CTRL        mac_seqnum_dup_det_ctrl; /*0x400,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_STA_ADDR_HIGH              mac_sta_addr_high; /*0x404,    RW, 0x00000000,                             NA*/
    volatile  T_MAC_PCU_REG_MAC_STA_ADDR_LOW               mac_sta_addr_low; /*0x408,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TX_SEQUENCE_NUMBER         mac_tx_sequence_number; /*0x40c,    RO, 0x00000000,                             NA*/
    volatile                        uint32_t                 reserved_9[28];
    volatile      T_MAC_PCU_REG_MAC_PCF_CTRL                   mac_pcf_ctrl; /*0x480,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_CFP_MAX_DURATION           mac_cfp_max_duration; /*0x484,    RW, 0x00000000,                             NA*/
    volatile  T_MAC_PCU_REG_MAC_CFP_INTERVAL               mac_cfp_interval; /*0x488,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_CFP_PARA_SET_ELEMENT_BYTE_NUM mac_cfp_para_set_element_byte_num; /*0x48c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_MEDIUM_OCCUPANCY_LIMIT     mac_medium_occupancy_limit; /*0x490,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PCF_Q_POINTER              mac_pcf_q_pointer; /*0x494,    RW, 0x00000000,                             NA*/
    volatile     T_MAC_PCU_REG_MAC_CFP_COUNT                  mac_cfp_count; /*0x498,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_CFP_UNUSED_DURATION        mac_cfp_unused_duration; /*0x49c,    RO, 0x00000000,                             NA*/
    volatile                        uint32_t                reserved_10[24];
    volatile T_MAC_PCU_REG_MAC_POWER_MANAGEMENT_CTRL      mac_power_management_ctrl; /*0x500,    RW, 0x00000000,                             NA*/
    volatile      T_MAC_PCU_REG_MAC_ATIM_WIN                   mac_atim_win; /*0x504,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_LISTEN_INTERVAL            mac_listen_interval; /*0x508,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_OFFSET_INTERVAL            mac_offset_interval; /*0x50c,    RW, 0x00000000,                             NA*/
    volatile     T_MAC_PCU_REG_MAC_SAPSD_SSP                  mac_sapsd_ssp; /*0x510,    RW, 0x00000000,                             NA*/
    volatile      T_MAC_PCU_REG_MAC_SAPSD_SI                   mac_sapsd_si; /*0x514,    RW, 0x00000000,                             NA*/
    volatile     T_MAC_PCU_REG_MAC_SMPS_MODE                  mac_smps_mode; /*0x518,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_LISTEN_INTERVAL_TIMER_CTRL mac_listen_interval_timer_ctrl; /*0x51c,    RW, 0x00000000,                             NA*/
    volatile                        uint32_t                reserved_11[24];
    volatile T_MAC_PCU_REG_MAC_INTERRUPT_STATUS           mac_interrupt_status; /*0x580,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_INTERRUPT_MASK             mac_interrupt_mask; /*0x584,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TX_FRAME_POINTER           mac_tx_frame_pointer; /*0x588,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RX_FRAME_POINTER           mac_rx_frame_pointer; /*0x58c,    RO, 0x00000000,                             NA*/
    volatile    T_MAC_PCU_REG_MAC_ERROR_CODE                 mac_error_code; /*0x590,    RO, 0x00000000,                             NA*/
    volatile   T_MAC_PCU_REG_MAC_TX_MPDU_CNT                mac_tx_mpdu_cnt; /*0x594,    RO, 0x00000000,                             NA*/
    volatile   T_MAC_PCU_REG_MAC_RX_MPDU_CNT                mac_rx_mpdu_cnt; /*0x598,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_HI_PRIORITY_RX_FRAME_POINTER mac_hi_priority_rx_frame_pointer; /*0x59c,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_DE_AUTHEN_REASON_CODE      mac_de_authen_reason_code; /*0x5a0,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_ERROR_INTERRUPT_STATUS     mac_error_interrupt_status; /*0x5a4,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_ERROR_INTERRUPT_MASK       mac_error_interrupt_mask; /*0x5a8,    RW, 0x00000000,                             NA*/
    volatile                        uint32_t                reserved_12[21];
    volatile  T_MAC_PCU_REG_MAC_PHY_SPI_CTRL               mac_phy_spi_ctrl; /*0x600,    RW, 0x00000000,                             NA*/
    volatile  T_MAC_PCU_REG_MAC_PHY_SPI_LOCK               mac_phy_spi_lock; /*0x604,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PHY_SPI_STATUS             mac_phy_spi_status; /*0x608,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RF_VCO_SPI_CTRL_WIDTH      mac_rf_vco_spi_ctrl_width; /*0x60c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RF_VCO_SPI_CTRL            mac_rf_vco_spi_ctrl; /*0x610,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RF_VCO_SPI_STATUS          mac_rf_vco_spi_status; /*0x614,    RO, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PHY_11A_REG_BASE_ADDR      mac_phy_11a_reg_base_addr; /*0x618,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PHY_11B_REG_BASE_ADDR      mac_phy_11b_reg_base_addr; /*0x61c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PHY_11G_REG_BASE_ADDR      mac_phy_11g_reg_base_addr; /*0x620,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PHY_RF_REG_BASE_ADDR       mac_phy_rf_reg_base_addr; /*0x624,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TX_PLCP_DELAY              mac_tx_plcp_delay; /*0x628,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RX_PLCP_DELAY              mac_rx_plcp_delay; /*0x62c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RXTX_TURNAROUND_TIME       mac_rxtx_turnaround_time; /*0x630,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PHY_TIMEOUT_ADJUST         mac_phy_timeout_adjust; /*0x634,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PHY_REG_ACCESS_CTRL        mac_phy_reg_access_ctrl; /*0x638,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PHY_REG_ACCESS_DATA        mac_phy_reg_access_data; /*0x63c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PHY_TX_SERVICE             mac_phy_tx_service; /*0x640,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TX_POWER_LEVEL             mac_tx_power_level; /*0x644,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PHY_CCA_DELAY              mac_phy_cca_delay; /*0x648,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_TX_PLCP_ADJUST             mac_tx_plcp_adjust; /*0x64c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RX_PLCP_DELAY2             mac_rx_plcp_delay2; /*0x650,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_RX_START_DELAY             mac_rx_start_delay; /*0x654,    RW, 0x00000000,                             NA*/
    volatile   T_MAC_PCU_REG_MAC_ANTENNA_SET                mac_antenna_set; /*0x658,    RW, 0x0F070301,                             NA*/
    volatile     T_MAC_PCU_REG_MAC_ODDR_CTRL                  mac_oddr_ctrl; /*0x65c,    RW, 0x00000000,                             NA*/
    volatile                        uint32_t                 reserved_13[8];
    volatile T_MAC_PCU_REG_MAC_PEER_STATION_ADDR_MSB      mac_peer_station_addr_msb; /*0x680,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PEER_STATION_ADDR_LSB      mac_peer_station_addr_lsb; /*0x684,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_SEQUENCE_NUMBER_TID        mac_sequence_number_tid; /*0x688,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_BA_FRAME_HEADER_ADDR       mac_ba_frame_header_addr; /*0x68c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_BITMAP_LOCATION_ADDR       mac_bitmap_location_addr; /*0x690,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_BA_LUT_UPDATE              mac_ba_lut_update; /*0x694,    RW, 0x00000000,                             NA*/
    volatile       T_MAC_PCU_REG_MAC_BA_CTRL                    mac_ba_ctrl; /*0x698,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PEER_STA_ADDR_MSB          mac_peer_sta_addr_msb; /*0x69c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_PEER_STA_ADDR_LSB          mac_peer_sta_addr_lsb; /*0x6a0,    RW, 0x00000000,                             NA*/
    volatile  T_MAC_PCU_REG_MAC_BA_PARA_SIZE               mac_ba_para_size; /*0x6a4,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_BA_COMPRESSED_BITMAP_MSW   mac_ba_compressed_bitmap_msw; /*0x6a8,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_BA_COMPRESSED_BITMAP_LSW   mac_ba_compressed_bitmap_lsw; /*0x6ac,    RW, 0x00000000,                             NA*/
    volatile                        uint32_t                reserved_14[20];
    volatile T_MAC_PCU_REG_MAC_SCHEDULE_LINK_ADDR         mac_schedule_link_addr; /*0x700,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_CAP_START_TIME             mac_cap_start_time; /*0x704,    RW, 0x00000000,                             NA*/
    volatile                        uint32_t                reserved_15[62];
    volatile      T_MAC_PCU_REG_MAC_P2P_CTRL                   mac_p2p_ctrl; /*0x800,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_NOA_COUNT_STATUS           mac_noa_count_status; /*0x804,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_NOA1_DURATION              mac_noa1_duration; /*0x808,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_NOA1_INTERVAL              mac_noa1_interval; /*0x80c,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_NOA1_START_TIME            mac_noa1_start_time; /*0x810,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_NOA2_DURATION              mac_noa2_duration; /*0x814,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_NOA2_INTERVAL              mac_noa2_interval; /*0x818,    RW, 0x00000000,                             NA*/
    volatile T_MAC_PCU_REG_MAC_NOA2_START_TIME            mac_noa2_start_time; /*0x81c,    RW, 0x00000000,                             NA*/
    volatile    T_MAC_PCU_REG_MAC_EOA_OFFSET                 mac_eoa_offset; /*0x820,    RW, 0x00000000,                             NA*/
    volatile    T_MAC_PCU_REG_MAC_P2P_STATUS                 mac_p2p_status; /*0x824,    RO, 0x00000000,                             NA*/
} T_HWP_MAC_PCU_REG_T;

#define hwp_mac_pcu_reg ((T_HWP_MAC_PCU_REG_T*)MAC_PCU_REG_BASE)


__STATIC_INLINE uint32_t mac_pcu_reg_mac_pa_version_get(void)
{
    return hwp_mac_pcu_reg->mac_pa_version.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_version_unpack(uint8_t* bn, uint8_t* y1, uint8_t* y2, uint8_t* y3, uint8_t* y4)
{
    T_MAC_PCU_REG_MAC_PA_VERSION localVal = hwp_mac_pcu_reg->mac_pa_version;

    *bn = localVal.bit_field.bn;
    *y1 = localVal.bit_field.y1;
    *y2 = localVal.bit_field.y2;
    *y3 = localVal.bit_field.y3;
    *y4 = localVal.bit_field.y4;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_version_bn_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_version.bit_field.bn;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_version_y1_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_version.bit_field.y1;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_version_y2_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_version.bit_field.y2;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_version_y3_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_version.bit_field.y3;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_version_y4_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_version.bit_field.y4;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_pa_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_pack(uint8_t hw_auto_rating_en, uint8_t tx_abort_mode, uint8_t self_cts_on_tx_abort, uint8_t tx_abort, uint8_t de_aggregation_en, uint8_t defragmentation_en, uint8_t mac_clock_en, uint8_t ht_sta_en, uint8_t sec_channel_cca_bypass_value, uint8_t sec_channel_cca_bypass_en, uint8_t pri_channel_cca_bypass_value, uint8_t pri_channel_cca_bypass_en, uint8_t disable_cts_tx, uint8_t disable_ack_tx, uint8_t cca_based_protocol_timer_en, uint8_t rx_error_eifs_en, uint8_t fcs_failure_eifs_en, uint8_t phy_register_address_width, uint8_t disable_duplicate_det, uint8_t pa_tx_suspend, uint8_t ce_en, uint8_t msdu_life_time_check_en, uint8_t rx_link_update_ctrl, uint8_t tx_head_update_ctrl, uint8_t rx_fifo_ctrl, uint8_t qack, uint8_t pcf_en, uint8_t hcca_en, uint8_t edca_en, uint8_t ap_sta, uint8_t bss_ibss, uint8_t pa_mode)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.val = (((uint32_t)hw_auto_rating_en << 31) | ((uint32_t)tx_abort_mode << 30) | ((uint32_t)self_cts_on_tx_abort << 29) | ((uint32_t)tx_abort << 28) | ((uint32_t)de_aggregation_en << 27) | ((uint32_t)defragmentation_en << 26) | ((uint32_t)mac_clock_en << 25) | ((uint32_t)ht_sta_en << 24) | ((uint32_t)sec_channel_cca_bypass_value << 23) | ((uint32_t)sec_channel_cca_bypass_en << 22) | ((uint32_t)pri_channel_cca_bypass_value << 21) | ((uint32_t)pri_channel_cca_bypass_en << 20) | ((uint32_t)disable_cts_tx << 19) | ((uint32_t)disable_ack_tx << 18) | ((uint32_t)cca_based_protocol_timer_en << 17) | ((uint32_t)rx_error_eifs_en << 16) | ((uint32_t)fcs_failure_eifs_en << 15) | ((uint32_t)phy_register_address_width << 14) | ((uint32_t)disable_duplicate_det << 13) | ((uint32_t)pa_tx_suspend << 12) | ((uint32_t)ce_en << 11) | ((uint32_t)msdu_life_time_check_en << 10) | ((uint32_t)rx_link_update_ctrl << 9) | ((uint32_t)tx_head_update_ctrl << 8) | ((uint32_t)rx_fifo_ctrl << 7) | ((uint32_t)qack << 6) | ((uint32_t)pcf_en << 5) | ((uint32_t)hcca_en << 4) | ((uint32_t)edca_en << 3) | ((uint32_t)ap_sta << 2) | ((uint32_t)bss_ibss << 1) | ((uint32_t)pa_mode << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_unpack(uint8_t* hw_auto_rating_en, uint8_t* tx_abort_mode, uint8_t* self_cts_on_tx_abort, uint8_t* tx_abort, uint8_t* de_aggregation_en, uint8_t* defragmentation_en, uint8_t* mac_clock_en, uint8_t* ht_sta_en, uint8_t* sec_channel_cca_bypass_value, uint8_t* sec_channel_cca_bypass_en, uint8_t* pri_channel_cca_bypass_value, uint8_t* pri_channel_cca_bypass_en, uint8_t* disable_cts_tx, uint8_t* disable_ack_tx, uint8_t* cca_based_protocol_timer_en, uint8_t* rx_error_eifs_en, uint8_t* fcs_failure_eifs_en, uint8_t* phy_register_address_width, uint8_t* disable_duplicate_det, uint8_t* pa_tx_suspend, uint8_t* ce_en, uint8_t* msdu_life_time_check_en, uint8_t* rx_link_update_ctrl, uint8_t* tx_head_update_ctrl, uint8_t* rx_fifo_ctrl, uint8_t* qack, uint8_t* pcf_en, uint8_t* hcca_en, uint8_t* edca_en, uint8_t* ap_sta, uint8_t* bss_ibss, uint8_t* pa_mode)
{
    T_MAC_PCU_REG_MAC_PA_CTRL localVal = hwp_mac_pcu_reg->mac_pa_ctrl;

    *hw_auto_rating_en = localVal.bit_field.hw_auto_rating_en;
    *tx_abort_mode = localVal.bit_field.tx_abort_mode;
    *self_cts_on_tx_abort = localVal.bit_field.self_cts_on_tx_abort;
    *tx_abort = localVal.bit_field.tx_abort;
    *de_aggregation_en = localVal.bit_field.de_aggregation_en;
    *defragmentation_en = localVal.bit_field.defragmentation_en;
    *mac_clock_en = localVal.bit_field.mac_clock_en;
    *ht_sta_en = localVal.bit_field.ht_sta_en;
    *sec_channel_cca_bypass_value = localVal.bit_field.sec_channel_cca_bypass_value;
    *sec_channel_cca_bypass_en = localVal.bit_field.sec_channel_cca_bypass_en;
    *pri_channel_cca_bypass_value = localVal.bit_field.pri_channel_cca_bypass_value;
    *pri_channel_cca_bypass_en = localVal.bit_field.pri_channel_cca_bypass_en;
    *disable_cts_tx = localVal.bit_field.disable_cts_tx;
    *disable_ack_tx = localVal.bit_field.disable_ack_tx;
    *cca_based_protocol_timer_en = localVal.bit_field.cca_based_protocol_timer_en;
    *rx_error_eifs_en = localVal.bit_field.rx_error_eifs_en;
    *fcs_failure_eifs_en = localVal.bit_field.fcs_failure_eifs_en;
    *phy_register_address_width = localVal.bit_field.phy_register_address_width;
    *disable_duplicate_det = localVal.bit_field.disable_duplicate_det;
    *pa_tx_suspend = localVal.bit_field.pa_tx_suspend;
    *ce_en = localVal.bit_field.ce_en;
    *msdu_life_time_check_en = localVal.bit_field.msdu_life_time_check_en;
    *rx_link_update_ctrl = localVal.bit_field.rx_link_update_ctrl;
    *tx_head_update_ctrl = localVal.bit_field.tx_head_update_ctrl;
    *rx_fifo_ctrl = localVal.bit_field.rx_fifo_ctrl;
    *qack = localVal.bit_field.qack;
    *pcf_en = localVal.bit_field.pcf_en;
    *hcca_en = localVal.bit_field.hcca_en;
    *edca_en = localVal.bit_field.edca_en;
    *ap_sta = localVal.bit_field.ap_sta;
    *bss_ibss = localVal.bit_field.bss_ibss;
    *pa_mode = localVal.bit_field.pa_mode;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_hw_auto_rating_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.hw_auto_rating_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_hw_auto_rating_en_setf(uint8_t hw_auto_rating_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.hw_auto_rating_en = hw_auto_rating_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_tx_abort_mode_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.tx_abort_mode;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_tx_abort_mode_setf(uint8_t tx_abort_mode)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.tx_abort_mode = tx_abort_mode;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_self_cts_on_tx_abort_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.self_cts_on_tx_abort;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_self_cts_on_tx_abort_setf(uint8_t self_cts_on_tx_abort)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.self_cts_on_tx_abort = self_cts_on_tx_abort;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_tx_abort_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.tx_abort;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_tx_abort_setf(uint8_t tx_abort)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.tx_abort = tx_abort;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_de_aggregation_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.de_aggregation_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_de_aggregation_en_setf(uint8_t de_aggregation_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.de_aggregation_en = de_aggregation_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_defragmentation_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.defragmentation_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_defragmentation_en_setf(uint8_t defragmentation_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.defragmentation_en = defragmentation_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_mac_clock_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.mac_clock_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_mac_clock_en_setf(uint8_t mac_clock_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.mac_clock_en = mac_clock_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_ht_sta_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.ht_sta_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_ht_sta_en_setf(uint8_t ht_sta_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.ht_sta_en = ht_sta_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_sec_channel_cca_bypass_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.sec_channel_cca_bypass_value;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_sec_channel_cca_bypass_value_setf(uint8_t sec_channel_cca_bypass_value)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.sec_channel_cca_bypass_value = sec_channel_cca_bypass_value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_sec_channel_cca_bypass_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.sec_channel_cca_bypass_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_sec_channel_cca_bypass_en_setf(uint8_t sec_channel_cca_bypass_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.sec_channel_cca_bypass_en = sec_channel_cca_bypass_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_pri_channel_cca_bypass_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.pri_channel_cca_bypass_value;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_pri_channel_cca_bypass_value_setf(uint8_t pri_channel_cca_bypass_value)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.pri_channel_cca_bypass_value = pri_channel_cca_bypass_value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_pri_channel_cca_bypass_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.pri_channel_cca_bypass_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_pri_channel_cca_bypass_en_setf(uint8_t pri_channel_cca_bypass_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.pri_channel_cca_bypass_en = pri_channel_cca_bypass_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_disable_cts_tx_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.disable_cts_tx;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_disable_cts_tx_setf(uint8_t disable_cts_tx)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.disable_cts_tx = disable_cts_tx;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_disable_ack_tx_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.disable_ack_tx;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_disable_ack_tx_setf(uint8_t disable_ack_tx)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.disable_ack_tx = disable_ack_tx;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_cca_based_protocol_timer_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.cca_based_protocol_timer_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_cca_based_protocol_timer_en_setf(uint8_t cca_based_protocol_timer_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.cca_based_protocol_timer_en = cca_based_protocol_timer_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_rx_error_eifs_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.rx_error_eifs_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_rx_error_eifs_en_setf(uint8_t rx_error_eifs_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.rx_error_eifs_en = rx_error_eifs_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_fcs_failure_eifs_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.fcs_failure_eifs_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_fcs_failure_eifs_en_setf(uint8_t fcs_failure_eifs_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.fcs_failure_eifs_en = fcs_failure_eifs_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_phy_register_address_width_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.phy_register_address_width;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_phy_register_address_width_setf(uint8_t phy_register_address_width)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.phy_register_address_width = phy_register_address_width;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_disable_duplicate_det_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.disable_duplicate_det;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_disable_duplicate_det_setf(uint8_t disable_duplicate_det)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.disable_duplicate_det = disable_duplicate_det;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_pa_tx_suspend_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.pa_tx_suspend;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_pa_tx_suspend_setf(uint8_t pa_tx_suspend)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.pa_tx_suspend = pa_tx_suspend;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_ce_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.ce_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_ce_en_setf(uint8_t ce_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.ce_en = ce_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_msdu_life_time_check_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.msdu_life_time_check_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_msdu_life_time_check_en_setf(uint8_t msdu_life_time_check_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.msdu_life_time_check_en = msdu_life_time_check_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_rx_link_update_ctrl_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.rx_link_update_ctrl;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_rx_link_update_ctrl_setf(uint8_t rx_link_update_ctrl)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.rx_link_update_ctrl = rx_link_update_ctrl;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_tx_head_update_ctrl_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.tx_head_update_ctrl;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_tx_head_update_ctrl_setf(uint8_t tx_head_update_ctrl)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.tx_head_update_ctrl = tx_head_update_ctrl;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_rx_fifo_ctrl_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.rx_fifo_ctrl;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_rx_fifo_ctrl_setf(uint8_t rx_fifo_ctrl)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.rx_fifo_ctrl = rx_fifo_ctrl;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_qack_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.qack;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_qack_setf(uint8_t qack)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.qack = qack;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_pcf_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.pcf_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_pcf_en_setf(uint8_t pcf_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.pcf_en = pcf_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_hcca_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.hcca_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_hcca_en_setf(uint8_t hcca_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.hcca_en = hcca_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_edca_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.edca_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_edca_en_setf(uint8_t edca_en)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.edca_en = edca_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_ap_sta_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.ap_sta;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_ap_sta_setf(uint8_t ap_sta)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.ap_sta = ap_sta;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_bss_ibss_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.bss_ibss;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_bss_ibss_setf(uint8_t bss_ibss)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.bss_ibss = bss_ibss;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_ctrl_pa_mode_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.pa_mode;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_ctrl_pa_mode_setf(uint8_t pa_mode)
{
    hwp_mac_pcu_reg->mac_pa_ctrl.bit_field.pa_mode = pa_mode;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_pa_status_get(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_pa_status_unpack(uint8_t* self_cts_tx_done_on_tx_abort, uint8_t* pa_tx_abort_mode, uint8_t* beacon_txed_in_pre_beacon_interval, uint8_t* pcf_q_access_lock, uint8_t* hcca_ap_q_access_lock, uint8_t* hcca_sta_priority_7_q_access_lock, uint8_t* hcca_sta_priority_6_q_access_lock, uint8_t* hcca_sta_priority_5_q_access_lock, uint8_t* hcca_sta_priority_4_q_access_lock, uint8_t* hcca_sta_priority_3_q_access_lock, uint8_t* hcca_sta_priority_2_q_access_lock, uint8_t* hcca_sta_priority_1_q_access_lock, uint8_t* hcca_sta_priority_0_q_access_lock, uint8_t* ac_vo_q_access_lock, uint8_t* ac_vi_q_access_lock, uint8_t* ac_be_q_access_lock, uint8_t* ac_bk_q_access_lock, uint8_t* hi_priority_q_access_lock, uint8_t* pa_in_suspend_mode, uint8_t* slot_in_use, uint8_t* beacon_txed, uint8_t* duplicate_detected, uint8_t* cca_secondary, uint8_t* cca_primary, uint8_t* fcs)
{
    T_MAC_PCU_REG_MAC_PA_STATUS localVal = hwp_mac_pcu_reg->mac_pa_status;

    *self_cts_tx_done_on_tx_abort = localVal.bit_field.self_cts_tx_done_on_tx_abort;
    *pa_tx_abort_mode = localVal.bit_field.pa_tx_abort_mode;
    *beacon_txed_in_pre_beacon_interval = localVal.bit_field.beacon_txed_in_pre_beacon_interval;
    *pcf_q_access_lock = localVal.bit_field.pcf_q_access_lock;
    *hcca_ap_q_access_lock = localVal.bit_field.hcca_ap_q_access_lock;
    *hcca_sta_priority_7_q_access_lock = localVal.bit_field.hcca_sta_priority_7_q_access_lock;
    *hcca_sta_priority_6_q_access_lock = localVal.bit_field.hcca_sta_priority_6_q_access_lock;
    *hcca_sta_priority_5_q_access_lock = localVal.bit_field.hcca_sta_priority_5_q_access_lock;
    *hcca_sta_priority_4_q_access_lock = localVal.bit_field.hcca_sta_priority_4_q_access_lock;
    *hcca_sta_priority_3_q_access_lock = localVal.bit_field.hcca_sta_priority_3_q_access_lock;
    *hcca_sta_priority_2_q_access_lock = localVal.bit_field.hcca_sta_priority_2_q_access_lock;
    *hcca_sta_priority_1_q_access_lock = localVal.bit_field.hcca_sta_priority_1_q_access_lock;
    *hcca_sta_priority_0_q_access_lock = localVal.bit_field.hcca_sta_priority_0_q_access_lock;
    *ac_vo_q_access_lock = localVal.bit_field.ac_vo_q_access_lock;
    *ac_vi_q_access_lock = localVal.bit_field.ac_vi_q_access_lock;
    *ac_be_q_access_lock = localVal.bit_field.ac_be_q_access_lock;
    *ac_bk_q_access_lock = localVal.bit_field.ac_bk_q_access_lock;
    *hi_priority_q_access_lock = localVal.bit_field.hi_priority_q_access_lock;
    *pa_in_suspend_mode = localVal.bit_field.pa_in_suspend_mode;
    *slot_in_use = localVal.bit_field.slot_in_use;
    *beacon_txed = localVal.bit_field.beacon_txed;
    *duplicate_detected = localVal.bit_field.duplicate_detected;
    *cca_secondary = localVal.bit_field.cca_secondary;
    *cca_primary = localVal.bit_field.cca_primary;
    *fcs = localVal.bit_field.fcs;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_self_cts_tx_done_on_tx_abort_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.self_cts_tx_done_on_tx_abort;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_pa_tx_abort_mode_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.pa_tx_abort_mode;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_beacon_txed_in_pre_beacon_interval_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.beacon_txed_in_pre_beacon_interval;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_pcf_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.pcf_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_hcca_ap_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.hcca_ap_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_hcca_sta_priority_7_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.hcca_sta_priority_7_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_hcca_sta_priority_6_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.hcca_sta_priority_6_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_hcca_sta_priority_5_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.hcca_sta_priority_5_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_hcca_sta_priority_4_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.hcca_sta_priority_4_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_hcca_sta_priority_3_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.hcca_sta_priority_3_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_hcca_sta_priority_2_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.hcca_sta_priority_2_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_hcca_sta_priority_1_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.hcca_sta_priority_1_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_hcca_sta_priority_0_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.hcca_sta_priority_0_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_ac_vo_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.ac_vo_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_ac_vi_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.ac_vi_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_ac_be_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.ac_be_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_ac_bk_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.ac_bk_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_hi_priority_q_access_lock_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.hi_priority_q_access_lock;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_pa_in_suspend_mode_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.pa_in_suspend_mode;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_slot_in_use_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.slot_in_use;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_beacon_txed_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.beacon_txed;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_duplicate_detected_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.duplicate_detected;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_cca_secondary_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.cca_secondary;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_cca_primary_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.cca_primary;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pa_status_fcs_getf(void)
{
    return hwp_mac_pcu_reg->mac_pa_status.bit_field.fcs;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_address_high_get(void)
{
    return hwp_mac_pcu_reg->mac_address_high.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_address_high_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_address_high.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_address_high_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_address_high.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_address_high_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_ADDRESS_HIGH localVal = hwp_mac_pcu_reg->mac_address_high;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_address_high_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_address_high.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_address_high_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_address_high.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_address_low_get(void)
{
    return hwp_mac_pcu_reg->mac_address_low.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_address_low_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_address_low.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_address_low_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_address_low.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_address_low_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_ADDRESS_LOW localVal = hwp_mac_pcu_reg->mac_address_low;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_address_low_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_address_low.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_address_low_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_address_low.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_bssid_high_get(void)
{
    return hwp_mac_pcu_reg->mac_bssid_high.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_bssid_high_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_bssid_high.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_bssid_high_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_bssid_high.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_bssid_high_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_BSSID_HIGH localVal = hwp_mac_pcu_reg->mac_bssid_high;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_bssid_high_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_bssid_high.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_bssid_high_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_bssid_high.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_bssid_low_get(void)
{
    return hwp_mac_pcu_reg->mac_bssid_low.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_bssid_low_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_bssid_low.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_bssid_low_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_bssid_low.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_bssid_low_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_BSSID_LOW localVal = hwp_mac_pcu_reg->mac_bssid_low;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_bssid_low_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_bssid_low.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_bssid_low_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_bssid_low.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_prbs_seed_value_get(void)
{
    return hwp_mac_pcu_reg->mac_prbs_seed_value.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_prbs_seed_value_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_prbs_seed_value.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_prbs_seed_value_pack(uint8_t ac_bk, uint8_t ac_be, uint8_t ac_vi, uint8_t ac_vo)
{
    hwp_mac_pcu_reg->mac_prbs_seed_value.val = (((uint32_t)ac_bk << 24) | ((uint32_t)ac_be << 16) | ((uint32_t)ac_vi << 8) | ((uint32_t)ac_vo << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_prbs_seed_value_unpack(uint8_t* ac_bk, uint8_t* ac_be, uint8_t* ac_vi, uint8_t* ac_vo)
{
    T_MAC_PCU_REG_MAC_PRBS_SEED_VALUE localVal = hwp_mac_pcu_reg->mac_prbs_seed_value;

    *ac_bk = localVal.bit_field.ac_bk;
    *ac_be = localVal.bit_field.ac_be;
    *ac_vi = localVal.bit_field.ac_vi;
    *ac_vo = localVal.bit_field.ac_vo;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_prbs_seed_value_ac_bk_getf(void)
{
    return hwp_mac_pcu_reg->mac_prbs_seed_value.bit_field.ac_bk;
}

__STATIC_INLINE void mac_pcu_reg_mac_prbs_seed_value_ac_bk_setf(uint8_t ac_bk)
{
    hwp_mac_pcu_reg->mac_prbs_seed_value.bit_field.ac_bk = ac_bk;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_prbs_seed_value_ac_be_getf(void)
{
    return hwp_mac_pcu_reg->mac_prbs_seed_value.bit_field.ac_be;
}

__STATIC_INLINE void mac_pcu_reg_mac_prbs_seed_value_ac_be_setf(uint8_t ac_be)
{
    hwp_mac_pcu_reg->mac_prbs_seed_value.bit_field.ac_be = ac_be;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_prbs_seed_value_ac_vi_getf(void)
{
    return hwp_mac_pcu_reg->mac_prbs_seed_value.bit_field.ac_vi;
}

__STATIC_INLINE void mac_pcu_reg_mac_prbs_seed_value_ac_vi_setf(uint8_t ac_vi)
{
    hwp_mac_pcu_reg->mac_prbs_seed_value.bit_field.ac_vi = ac_vi;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_prbs_seed_value_ac_vo_getf(void)
{
    return hwp_mac_pcu_reg->mac_prbs_seed_value.bit_field.ac_vo;
}

__STATIC_INLINE void mac_pcu_reg_mac_prbs_seed_value_ac_vo_setf(uint8_t ac_vo)
{
    hwp_mac_pcu_reg->mac_prbs_seed_value.bit_field.ac_vo = ac_vo;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_dma_burst_size_get(void)
{
    return hwp_mac_pcu_reg->mac_dma_burst_size.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_dma_burst_size_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_dma_burst_size.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_dma_burst_size_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_dma_burst_size.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_dma_burst_size_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_DMA_BURST_SIZE localVal = hwp_mac_pcu_reg->mac_dma_burst_size;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_dma_burst_size_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_dma_burst_size.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_dma_burst_size_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_dma_burst_size.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_rx_complete_cnt_get(void)
{
    return hwp_mac_pcu_reg->mac_tx_rx_complete_cnt.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_rx_complete_cnt_unpack(uint16_t* rx_cnt, uint16_t* tx_cnt)
{
    T_MAC_PCU_REG_MAC_TX_RX_COMPLETE_CNT localVal = hwp_mac_pcu_reg->mac_tx_rx_complete_cnt;

    *rx_cnt = localVal.bit_field.rx_cnt;
    *tx_cnt = localVal.bit_field.tx_cnt;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_tx_rx_complete_cnt_rx_cnt_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_rx_complete_cnt.bit_field.rx_cnt;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_tx_rx_complete_cnt_tx_cnt_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_rx_complete_cnt.bit_field.tx_cnt;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_prbs_read_value_get(void)
{
    return hwp_mac_pcu_reg->mac_prbs_read_value.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_prbs_read_value_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_prbs_read_value.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_prbs_read_value_pack(uint8_t value, uint8_t read_flag)
{
    hwp_mac_pcu_reg->mac_prbs_read_value.val = (((uint32_t)value << 1) | ((uint32_t)read_flag << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_prbs_read_value_unpack(uint8_t* value, uint8_t* read_flag)
{
    T_MAC_PCU_REG_MAC_PRBS_READ_VALUE localVal = hwp_mac_pcu_reg->mac_prbs_read_value;

    *value = localVal.bit_field.value;
    *read_flag = localVal.bit_field.read_flag;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_prbs_read_value_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_prbs_read_value.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_prbs_read_value_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_prbs_read_value.bit_field.value = value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_prbs_read_value_read_flag_getf(void)
{
    return hwp_mac_pcu_reg->mac_prbs_read_value.bit_field.read_flag;
}

__STATIC_INLINE void mac_pcu_reg_mac_prbs_read_value_read_flag_setf(uint8_t read_flag)
{
    hwp_mac_pcu_reg->mac_prbs_read_value.bit_field.read_flag = read_flag;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_null_frame_rate_get(void)
{
    return hwp_mac_pcu_reg->mac_null_frame_rate.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_null_frame_rate_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_null_frame_rate.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_null_frame_rate_pack(uint8_t null_frame_rate)
{
    hwp_mac_pcu_reg->mac_null_frame_rate.val = (((uint32_t)null_frame_rate << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_null_frame_rate_unpack(uint8_t* null_frame_tx_power_level, uint8_t* null_frame_rate)
{
    T_MAC_PCU_REG_MAC_NULL_FRAME_RATE localVal = hwp_mac_pcu_reg->mac_null_frame_rate;

    *null_frame_tx_power_level = localVal.bit_field.null_frame_tx_power_level;
    *null_frame_rate = localVal.bit_field.null_frame_rate;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_null_frame_rate_null_frame_tx_power_level_getf(void)
{
    return hwp_mac_pcu_reg->mac_null_frame_rate.bit_field.null_frame_tx_power_level;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_null_frame_rate_null_frame_rate_getf(void)
{
    return hwp_mac_pcu_reg->mac_null_frame_rate.bit_field.null_frame_rate;
}

__STATIC_INLINE void mac_pcu_reg_mac_null_frame_rate_null_frame_rate_setf(uint8_t null_frame_rate)
{
    hwp_mac_pcu_reg->mac_null_frame_rate.bit_field.null_frame_rate = null_frame_rate;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_null_frame_phy_tx_mode_get(void)
{
    return hwp_mac_pcu_reg->mac_null_frame_phy_tx_mode.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_null_frame_phy_tx_mode_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_null_frame_phy_tx_mode.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_null_frame_phy_tx_mode_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_null_frame_phy_tx_mode.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_null_frame_phy_tx_mode_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_NULL_FRAME_PHY_TX_MODE localVal = hwp_mac_pcu_reg->mac_null_frame_phy_tx_mode;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_null_frame_phy_tx_mode_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_null_frame_phy_tx_mode.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_null_frame_phy_tx_mode_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_null_frame_phy_tx_mode.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_test_mode_get(void)
{
    return hwp_mac_pcu_reg->mac_test_mode.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_test_mode_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_test_mode.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_test_mode_pack(uint8_t skip_sn, uint8_t corrupt_tx_fcs)
{
    hwp_mac_pcu_reg->mac_test_mode.val = (((uint32_t)skip_sn << 1) | ((uint32_t)corrupt_tx_fcs << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_test_mode_unpack(uint8_t* skip_sn, uint8_t* corrupt_tx_fcs)
{
    T_MAC_PCU_REG_MAC_TEST_MODE localVal = hwp_mac_pcu_reg->mac_test_mode;

    *skip_sn = localVal.bit_field.skip_sn;
    *corrupt_tx_fcs = localVal.bit_field.corrupt_tx_fcs;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_test_mode_skip_sn_getf(void)
{
    return hwp_mac_pcu_reg->mac_test_mode.bit_field.skip_sn;
}

__STATIC_INLINE void mac_pcu_reg_mac_test_mode_skip_sn_setf(uint8_t skip_sn)
{
    hwp_mac_pcu_reg->mac_test_mode.bit_field.skip_sn = skip_sn;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_test_mode_corrupt_tx_fcs_getf(void)
{
    return hwp_mac_pcu_reg->mac_test_mode.bit_field.corrupt_tx_fcs;
}

__STATIC_INLINE void mac_pcu_reg_mac_test_mode_corrupt_tx_fcs_setf(uint8_t corrupt_tx_fcs)
{
    hwp_mac_pcu_reg->mac_test_mode.bit_field.corrupt_tx_fcs = corrupt_tx_fcs;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hw_id_get(void)
{
    return hwp_mac_pcu_reg->mac_hw_id.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hw_id_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_HW_ID localVal = hwp_mac_pcu_reg->mac_hw_id;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hw_id_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_hw_id.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hw_reset_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_hw_reset_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hw_reset_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hw_reset_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hw_reset_ctrl_pack(uint8_t duplicate_detection_reset, uint8_t defrag_lut_reset, uint8_t tsf_reset, uint8_t ce_core_reset, uint8_t pa_core_reset, uint8_t mac_hw_reset)
{
    hwp_mac_pcu_reg->mac_hw_reset_ctrl.val = (((uint32_t)duplicate_detection_reset << 5) | ((uint32_t)defrag_lut_reset << 4) | ((uint32_t)tsf_reset << 3) | ((uint32_t)ce_core_reset << 2) | ((uint32_t)pa_core_reset << 1) | ((uint32_t)mac_hw_reset << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_hw_reset_ctrl_unpack(uint8_t* duplicate_detection_reset, uint8_t* defrag_lut_reset, uint8_t* tsf_reset, uint8_t* ce_core_reset, uint8_t* pa_core_reset, uint8_t* mac_hw_reset)
{
    T_MAC_PCU_REG_MAC_HW_RESET_CTRL localVal = hwp_mac_pcu_reg->mac_hw_reset_ctrl;

    *duplicate_detection_reset = localVal.bit_field.duplicate_detection_reset;
    *defrag_lut_reset = localVal.bit_field.defrag_lut_reset;
    *tsf_reset = localVal.bit_field.tsf_reset;
    *ce_core_reset = localVal.bit_field.ce_core_reset;
    *pa_core_reset = localVal.bit_field.pa_core_reset;
    *mac_hw_reset = localVal.bit_field.mac_hw_reset;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_hw_reset_ctrl_duplicate_detection_reset_getf(void)
{
    return hwp_mac_pcu_reg->mac_hw_reset_ctrl.bit_field.duplicate_detection_reset;
}

__STATIC_INLINE void mac_pcu_reg_mac_hw_reset_ctrl_duplicate_detection_reset_setf(uint8_t duplicate_detection_reset)
{
    hwp_mac_pcu_reg->mac_hw_reset_ctrl.bit_field.duplicate_detection_reset = duplicate_detection_reset;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_hw_reset_ctrl_defrag_lut_reset_getf(void)
{
    return hwp_mac_pcu_reg->mac_hw_reset_ctrl.bit_field.defrag_lut_reset;
}

__STATIC_INLINE void mac_pcu_reg_mac_hw_reset_ctrl_defrag_lut_reset_setf(uint8_t defrag_lut_reset)
{
    hwp_mac_pcu_reg->mac_hw_reset_ctrl.bit_field.defrag_lut_reset = defrag_lut_reset;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_hw_reset_ctrl_tsf_reset_getf(void)
{
    return hwp_mac_pcu_reg->mac_hw_reset_ctrl.bit_field.tsf_reset;
}

__STATIC_INLINE void mac_pcu_reg_mac_hw_reset_ctrl_tsf_reset_setf(uint8_t tsf_reset)
{
    hwp_mac_pcu_reg->mac_hw_reset_ctrl.bit_field.tsf_reset = tsf_reset;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_hw_reset_ctrl_ce_core_reset_getf(void)
{
    return hwp_mac_pcu_reg->mac_hw_reset_ctrl.bit_field.ce_core_reset;
}

__STATIC_INLINE void mac_pcu_reg_mac_hw_reset_ctrl_ce_core_reset_setf(uint8_t ce_core_reset)
{
    hwp_mac_pcu_reg->mac_hw_reset_ctrl.bit_field.ce_core_reset = ce_core_reset;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_hw_reset_ctrl_pa_core_reset_getf(void)
{
    return hwp_mac_pcu_reg->mac_hw_reset_ctrl.bit_field.pa_core_reset;
}

__STATIC_INLINE void mac_pcu_reg_mac_hw_reset_ctrl_pa_core_reset_setf(uint8_t pa_core_reset)
{
    hwp_mac_pcu_reg->mac_hw_reset_ctrl.bit_field.pa_core_reset = pa_core_reset;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_hw_reset_ctrl_mac_hw_reset_getf(void)
{
    return hwp_mac_pcu_reg->mac_hw_reset_ctrl.bit_field.mac_hw_reset;
}

__STATIC_INLINE void mac_pcu_reg_mac_hw_reset_ctrl_mac_hw_reset_setf(uint8_t mac_hw_reset)
{
    hwp_mac_pcu_reg->mac_hw_reset_ctrl.bit_field.mac_hw_reset = mac_hw_reset;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_abort_duration_timeout_get(void)
{
    return hwp_mac_pcu_reg->mac_tx_abort_duration_timeout.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_duration_timeout_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tx_abort_duration_timeout.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_duration_timeout_pack(uint16_t timeout, uint16_t duration_id)
{
    hwp_mac_pcu_reg->mac_tx_abort_duration_timeout.val = (((uint32_t)timeout << 16) | ((uint32_t)duration_id << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_duration_timeout_unpack(uint16_t* timeout, uint16_t* duration_id)
{
    T_MAC_PCU_REG_MAC_TX_ABORT_DURATION_TIMEOUT localVal = hwp_mac_pcu_reg->mac_tx_abort_duration_timeout;

    *timeout = localVal.bit_field.timeout;
    *duration_id = localVal.bit_field.duration_id;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_tx_abort_duration_timeout_timeout_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_abort_duration_timeout.bit_field.timeout;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_duration_timeout_timeout_setf(uint16_t timeout)
{
    hwp_mac_pcu_reg->mac_tx_abort_duration_timeout.bit_field.timeout = timeout;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_tx_abort_duration_timeout_duration_id_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_abort_duration_timeout.bit_field.duration_id;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_duration_timeout_duration_id_setf(uint16_t duration_id)
{
    hwp_mac_pcu_reg->mac_tx_abort_duration_timeout.bit_field.duration_id = duration_id;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_abort_frame_rate_get(void)
{
    return hwp_mac_pcu_reg->mac_tx_abort_frame_rate.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_frame_rate_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tx_abort_frame_rate.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_frame_rate_pack(uint8_t tx_power_level, uint8_t rate)
{
    hwp_mac_pcu_reg->mac_tx_abort_frame_rate.val = (((uint32_t)tx_power_level << 8) | ((uint32_t)rate << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_frame_rate_unpack(uint8_t* tx_power_level, uint8_t* rate)
{
    T_MAC_PCU_REG_MAC_TX_ABORT_FRAME_RATE localVal = hwp_mac_pcu_reg->mac_tx_abort_frame_rate;

    *tx_power_level = localVal.bit_field.tx_power_level;
    *rate = localVal.bit_field.rate;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tx_abort_frame_rate_tx_power_level_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_abort_frame_rate.bit_field.tx_power_level;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_frame_rate_tx_power_level_setf(uint8_t tx_power_level)
{
    hwp_mac_pcu_reg->mac_tx_abort_frame_rate.bit_field.tx_power_level = tx_power_level;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tx_abort_frame_rate_rate_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_abort_frame_rate.bit_field.rate;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_frame_rate_rate_setf(uint8_t rate)
{
    hwp_mac_pcu_reg->mac_tx_abort_frame_rate.bit_field.rate = rate;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_abort_frame_phy_tx_mode_get(void)
{
    return hwp_mac_pcu_reg->mac_tx_abort_frame_phy_tx_mode.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_frame_phy_tx_mode_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tx_abort_frame_phy_tx_mode.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_frame_phy_tx_mode_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tx_abort_frame_phy_tx_mode.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_frame_phy_tx_mode_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_TX_ABORT_FRAME_PHY_TX_MODE localVal = hwp_mac_pcu_reg->mac_tx_abort_frame_phy_tx_mode;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_abort_frame_phy_tx_mode_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_abort_frame_phy_tx_mode.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_abort_frame_phy_tx_mode_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tx_abort_frame_phy_tx_mode.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_extended_pa_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_extended_pa_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_extended_pa_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_extended_pa_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_extended_pa_ctrl_pack(uint8_t disable_defrag_timer, uint8_t sifs_responses_at_non_ht_rate, uint8_t sync_bus_pa_clk, uint8_t rx_buffer_based_ack, uint8_t touch_mpdu)
{
    hwp_mac_pcu_reg->mac_extended_pa_ctrl.val = (((uint32_t)disable_defrag_timer << 4) | ((uint32_t)sifs_responses_at_non_ht_rate << 3) | ((uint32_t)sync_bus_pa_clk << 2) | ((uint32_t)rx_buffer_based_ack << 1) | ((uint32_t)touch_mpdu << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_extended_pa_ctrl_unpack(uint8_t* disable_defrag_timer, uint8_t* sifs_responses_at_non_ht_rate, uint8_t* sync_bus_pa_clk, uint8_t* rx_buffer_based_ack, uint8_t* touch_mpdu)
{
    T_MAC_PCU_REG_MAC_EXTENDED_PA_CTRL localVal = hwp_mac_pcu_reg->mac_extended_pa_ctrl;

    *disable_defrag_timer = localVal.bit_field.disable_defrag_timer;
    *sifs_responses_at_non_ht_rate = localVal.bit_field.sifs_responses_at_non_ht_rate;
    *sync_bus_pa_clk = localVal.bit_field.sync_bus_pa_clk;
    *rx_buffer_based_ack = localVal.bit_field.rx_buffer_based_ack;
    *touch_mpdu = localVal.bit_field.touch_mpdu;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_extended_pa_ctrl_disable_defrag_timer_getf(void)
{
    return hwp_mac_pcu_reg->mac_extended_pa_ctrl.bit_field.disable_defrag_timer;
}

__STATIC_INLINE void mac_pcu_reg_mac_extended_pa_ctrl_disable_defrag_timer_setf(uint8_t disable_defrag_timer)
{
    hwp_mac_pcu_reg->mac_extended_pa_ctrl.bit_field.disable_defrag_timer = disable_defrag_timer;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_extended_pa_ctrl_sifs_responses_at_non_ht_rate_getf(void)
{
    return hwp_mac_pcu_reg->mac_extended_pa_ctrl.bit_field.sifs_responses_at_non_ht_rate;
}

__STATIC_INLINE void mac_pcu_reg_mac_extended_pa_ctrl_sifs_responses_at_non_ht_rate_setf(uint8_t sifs_responses_at_non_ht_rate)
{
    hwp_mac_pcu_reg->mac_extended_pa_ctrl.bit_field.sifs_responses_at_non_ht_rate = sifs_responses_at_non_ht_rate;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_extended_pa_ctrl_sync_bus_pa_clk_getf(void)
{
    return hwp_mac_pcu_reg->mac_extended_pa_ctrl.bit_field.sync_bus_pa_clk;
}

__STATIC_INLINE void mac_pcu_reg_mac_extended_pa_ctrl_sync_bus_pa_clk_setf(uint8_t sync_bus_pa_clk)
{
    hwp_mac_pcu_reg->mac_extended_pa_ctrl.bit_field.sync_bus_pa_clk = sync_bus_pa_clk;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_extended_pa_ctrl_rx_buffer_based_ack_getf(void)
{
    return hwp_mac_pcu_reg->mac_extended_pa_ctrl.bit_field.rx_buffer_based_ack;
}

__STATIC_INLINE void mac_pcu_reg_mac_extended_pa_ctrl_rx_buffer_based_ack_setf(uint8_t rx_buffer_based_ack)
{
    hwp_mac_pcu_reg->mac_extended_pa_ctrl.bit_field.rx_buffer_based_ack = rx_buffer_based_ack;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_extended_pa_ctrl_touch_mpdu_getf(void)
{
    return hwp_mac_pcu_reg->mac_extended_pa_ctrl.bit_field.touch_mpdu;
}

__STATIC_INLINE void mac_pcu_reg_mac_extended_pa_ctrl_touch_mpdu_setf(uint8_t touch_mpdu)
{
    hwp_mac_pcu_reg->mac_extended_pa_ctrl.bit_field.touch_mpdu = touch_mpdu;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_frame_filter_get(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_pack(uint8_t sifs_respone_ba_frame, uint8_t non_sifs_response_ba_frame, uint8_t bc_mc_mgmt_other_bss, uint8_t sec_channel_frames, uint8_t non_directed_ctrl, uint8_t non_directed_mgmt, uint8_t discarded_ibss_beacon, uint8_t de_authentication, uint8_t fcs_failed, uint8_t duplicate_frames, uint8_t bc_mc_data_other_bss, uint8_t non_directed_data, uint8_t qcf_poll, uint8_t cf_end, uint8_t atim, uint8_t beacon, uint8_t rts, uint8_t cts, uint8_t ack)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.val = (((uint32_t)sifs_respone_ba_frame << 18) | ((uint32_t)non_sifs_response_ba_frame << 17) | ((uint32_t)bc_mc_mgmt_other_bss << 16) | ((uint32_t)sec_channel_frames << 15) | ((uint32_t)non_directed_ctrl << 14) | ((uint32_t)non_directed_mgmt << 13) | ((uint32_t)discarded_ibss_beacon << 12) | ((uint32_t)de_authentication << 11) | ((uint32_t)fcs_failed << 10) | ((uint32_t)duplicate_frames << 9) | ((uint32_t)bc_mc_data_other_bss << 8) | ((uint32_t)non_directed_data << 7) | ((uint32_t)qcf_poll << 6) | ((uint32_t)cf_end << 5) | ((uint32_t)atim << 4) | ((uint32_t)beacon << 3) | ((uint32_t)rts << 2) | ((uint32_t)cts << 1) | ((uint32_t)ack << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_unpack(uint8_t* sifs_respone_ba_frame, uint8_t* non_sifs_response_ba_frame, uint8_t* bc_mc_mgmt_other_bss, uint8_t* sec_channel_frames, uint8_t* non_directed_ctrl, uint8_t* non_directed_mgmt, uint8_t* discarded_ibss_beacon, uint8_t* de_authentication, uint8_t* fcs_failed, uint8_t* duplicate_frames, uint8_t* bc_mc_data_other_bss, uint8_t* non_directed_data, uint8_t* qcf_poll, uint8_t* cf_end, uint8_t* atim, uint8_t* beacon, uint8_t* rts, uint8_t* cts, uint8_t* ack)
{
    T_MAC_PCU_REG_MAC_RX_FRAME_FILTER localVal = hwp_mac_pcu_reg->mac_rx_frame_filter;

    *sifs_respone_ba_frame = localVal.bit_field.sifs_respone_ba_frame;
    *non_sifs_response_ba_frame = localVal.bit_field.non_sifs_response_ba_frame;
    *bc_mc_mgmt_other_bss = localVal.bit_field.bc_mc_mgmt_other_bss;
    *sec_channel_frames = localVal.bit_field.sec_channel_frames;
    *non_directed_ctrl = localVal.bit_field.non_directed_ctrl;
    *non_directed_mgmt = localVal.bit_field.non_directed_mgmt;
    *discarded_ibss_beacon = localVal.bit_field.discarded_ibss_beacon;
    *de_authentication = localVal.bit_field.de_authentication;
    *fcs_failed = localVal.bit_field.fcs_failed;
    *duplicate_frames = localVal.bit_field.duplicate_frames;
    *bc_mc_data_other_bss = localVal.bit_field.bc_mc_data_other_bss;
    *non_directed_data = localVal.bit_field.non_directed_data;
    *qcf_poll = localVal.bit_field.qcf_poll;
    *cf_end = localVal.bit_field.cf_end;
    *atim = localVal.bit_field.atim;
    *beacon = localVal.bit_field.beacon;
    *rts = localVal.bit_field.rts;
    *cts = localVal.bit_field.cts;
    *ack = localVal.bit_field.ack;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_sifs_respone_ba_frame_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.sifs_respone_ba_frame;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_sifs_respone_ba_frame_setf(uint8_t sifs_respone_ba_frame)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.sifs_respone_ba_frame = sifs_respone_ba_frame;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_non_sifs_response_ba_frame_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.non_sifs_response_ba_frame;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_non_sifs_response_ba_frame_setf(uint8_t non_sifs_response_ba_frame)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.non_sifs_response_ba_frame = non_sifs_response_ba_frame;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_bc_mc_mgmt_other_bss_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.bc_mc_mgmt_other_bss;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_bc_mc_mgmt_other_bss_setf(uint8_t bc_mc_mgmt_other_bss)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.bc_mc_mgmt_other_bss = bc_mc_mgmt_other_bss;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_sec_channel_frames_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.sec_channel_frames;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_sec_channel_frames_setf(uint8_t sec_channel_frames)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.sec_channel_frames = sec_channel_frames;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_non_directed_ctrl_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.non_directed_ctrl;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_non_directed_ctrl_setf(uint8_t non_directed_ctrl)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.non_directed_ctrl = non_directed_ctrl;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_non_directed_mgmt_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.non_directed_mgmt;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_non_directed_mgmt_setf(uint8_t non_directed_mgmt)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.non_directed_mgmt = non_directed_mgmt;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_discarded_ibss_beacon_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.discarded_ibss_beacon;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_discarded_ibss_beacon_setf(uint8_t discarded_ibss_beacon)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.discarded_ibss_beacon = discarded_ibss_beacon;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_de_authentication_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.de_authentication;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_de_authentication_setf(uint8_t de_authentication)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.de_authentication = de_authentication;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_fcs_failed_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.fcs_failed;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_fcs_failed_setf(uint8_t fcs_failed)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.fcs_failed = fcs_failed;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_duplicate_frames_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.duplicate_frames;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_duplicate_frames_setf(uint8_t duplicate_frames)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.duplicate_frames = duplicate_frames;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_bc_mc_data_other_bss_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.bc_mc_data_other_bss;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_bc_mc_data_other_bss_setf(uint8_t bc_mc_data_other_bss)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.bc_mc_data_other_bss = bc_mc_data_other_bss;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_non_directed_data_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.non_directed_data;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_non_directed_data_setf(uint8_t non_directed_data)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.non_directed_data = non_directed_data;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_qcf_poll_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.qcf_poll;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_qcf_poll_setf(uint8_t qcf_poll)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.qcf_poll = qcf_poll;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_cf_end_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.cf_end;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_cf_end_setf(uint8_t cf_end)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.cf_end = cf_end;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_atim_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.atim;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_atim_setf(uint8_t atim)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.atim = atim;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_beacon_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.beacon;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_beacon_setf(uint8_t beacon)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.beacon = beacon;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_rts_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.rts;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_rts_setf(uint8_t rts)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.rts = rts;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_cts_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.cts;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_cts_setf(uint8_t cts)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.cts = cts;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_frame_filter_ack_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.ack;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_filter_ack_setf(uint8_t ack)
{
    hwp_mac_pcu_reg->mac_rx_frame_filter.bit_field.ack = ack;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_frame_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_frame_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_frame_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_frame_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_frame_ctrl_pack(uint8_t protocol_version)
{
    hwp_mac_pcu_reg->mac_frame_ctrl.val = (((uint32_t)protocol_version << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_frame_ctrl_unpack(uint8_t* protocol_version)
{
    T_MAC_PCU_REG_MAC_FRAME_CTRL localVal = hwp_mac_pcu_reg->mac_frame_ctrl;

    *protocol_version = localVal.bit_field.protocol_version;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_frame_ctrl_protocol_version_getf(void)
{
    return hwp_mac_pcu_reg->mac_frame_ctrl.bit_field.protocol_version;
}

__STATIC_INLINE void mac_pcu_reg_mac_frame_ctrl_protocol_version_setf(uint8_t protocol_version)
{
    hwp_mac_pcu_reg->mac_frame_ctrl.bit_field.protocol_version = protocol_version;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_buffer_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_rx_buffer_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_buffer_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_buffer_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_buffer_addr_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_buffer_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_buffer_addr_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_RX_BUFFER_ADDR localVal = hwp_mac_pcu_reg->mac_rx_buffer_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_buffer_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_buffer_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_buffer_addr_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_buffer_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_fcs_fail_cnt_get(void)
{
    return hwp_mac_pcu_reg->mac_fcs_fail_cnt.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_fcs_fail_cnt_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_fcs_fail_cnt.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_fcs_fail_cnt_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_fcs_fail_cnt.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_fcs_fail_cnt_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_FCS_FAIL_CNT localVal = hwp_mac_pcu_reg->mac_fcs_fail_cnt;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_fcs_fail_cnt_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_fcs_fail_cnt.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_fcs_fail_cnt_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_fcs_fail_cnt.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_max_frame_length_filter_get(void)
{
    return hwp_mac_pcu_reg->mac_rx_max_frame_length_filter.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_max_frame_length_filter_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_max_frame_length_filter.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_max_frame_length_filter_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_rx_max_frame_length_filter.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_max_frame_length_filter_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_RX_MAX_FRAME_LENGTH_FILTER localVal = hwp_mac_pcu_reg->mac_rx_max_frame_length_filter;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_rx_max_frame_length_filter_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_max_frame_length_filter.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_max_frame_length_filter_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_rx_max_frame_length_filter.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_duplicate_frame_cnt_get(void)
{
    return hwp_mac_pcu_reg->mac_duplicate_frame_cnt.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_duplicate_frame_cnt_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_duplicate_frame_cnt.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_duplicate_frame_cnt_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_duplicate_frame_cnt.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_duplicate_frame_cnt_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_DUPLICATE_FRAME_CNT localVal = hwp_mac_pcu_reg->mac_duplicate_frame_cnt;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_duplicate_frame_cnt_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_duplicate_frame_cnt.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_duplicate_frame_cnt_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_duplicate_frame_cnt.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_end_cnt_get(void)
{
    return hwp_mac_pcu_reg->mac_rx_end_cnt.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_end_cnt_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_end_cnt.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_end_cnt_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_end_cnt.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_end_cnt_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_RX_END_CNT localVal = hwp_mac_pcu_reg->mac_rx_end_cnt;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_end_cnt_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_end_cnt.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_end_cnt_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_end_cnt.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_end_error_cnt_get(void)
{
    return hwp_mac_pcu_reg->mac_rx_end_error_cnt.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_end_error_cnt_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_end_error_cnt.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_end_error_cnt_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_end_error_cnt.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_end_error_cnt_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_RX_END_ERROR_CNT localVal = hwp_mac_pcu_reg->mac_rx_end_error_cnt;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_end_error_cnt_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_end_error_cnt.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_end_error_cnt_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_end_error_cnt.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ampdu_rxed_cnt_get(void)
{
    return hwp_mac_pcu_reg->mac_ampdu_rxed_cnt.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ampdu_rxed_cnt_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_AMPDU_RXED_CNT localVal = hwp_mac_pcu_reg->mac_ampdu_rxed_cnt;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ampdu_rxed_cnt_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_ampdu_rxed_cnt.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_mpdu_in_ampdu_cnt_get(void)
{
    return hwp_mac_pcu_reg->mac_mpdu_in_ampdu_cnt.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_mpdu_in_ampdu_cnt_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_MPDU_IN_AMPDU_CNT localVal = hwp_mac_pcu_reg->mac_mpdu_in_ampdu_cnt;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_mpdu_in_ampdu_cnt_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_mpdu_in_ampdu_cnt.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rxed_octets_in_ampdu_get(void)
{
    return hwp_mac_pcu_reg->mac_rxed_octets_in_ampdu.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rxed_octets_in_ampdu_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_RXED_OCTETS_IN_AMPDU localVal = hwp_mac_pcu_reg->mac_rxed_octets_in_ampdu;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rxed_octets_in_ampdu_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_rxed_octets_in_ampdu.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ampdu_del_crc_error_cnt_get(void)
{
    return hwp_mac_pcu_reg->mac_ampdu_del_crc_error_cnt.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ampdu_del_crc_error_cnt_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_AMPDU_DEL_CRC_ERROR_CNT localVal = hwp_mac_pcu_reg->mac_ampdu_del_crc_error_cnt;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ampdu_del_crc_error_cnt_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_ampdu_del_crc_error_cnt.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_defrag_lifetime_limit_get(void)
{
    return hwp_mac_pcu_reg->mac_defrag_lifetime_limit.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_defrag_lifetime_limit_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_defrag_lifetime_limit.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_defrag_lifetime_limit_pack(uint16_t rx_lifetime)
{
    hwp_mac_pcu_reg->mac_defrag_lifetime_limit.val = (((uint32_t)rx_lifetime << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_defrag_lifetime_limit_unpack(uint16_t* rx_lifetime)
{
    T_MAC_PCU_REG_MAC_DEFRAG_LIFETIME_LIMIT localVal = hwp_mac_pcu_reg->mac_defrag_lifetime_limit;

    *rx_lifetime = localVal.bit_field.rx_lifetime;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_defrag_lifetime_limit_rx_lifetime_getf(void)
{
    return hwp_mac_pcu_reg->mac_defrag_lifetime_limit.bit_field.rx_lifetime;
}

__STATIC_INLINE void mac_pcu_reg_mac_defrag_lifetime_limit_rx_lifetime_setf(uint16_t rx_lifetime)
{
    hwp_mac_pcu_reg->mac_defrag_lifetime_limit.bit_field.rx_lifetime = rx_lifetime;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hi_priority_rx_q_buffer_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_hi_priority_rx_q_buffer_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_priority_rx_q_buffer_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hi_priority_rx_q_buffer_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_priority_rx_q_buffer_addr_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hi_priority_rx_q_buffer_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_priority_rx_q_buffer_addr_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_HI_PRIORITY_RX_Q_BUFFER_ADDR localVal = hwp_mac_pcu_reg->mac_hi_priority_rx_q_buffer_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hi_priority_rx_q_buffer_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_hi_priority_rx_q_buffer_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_priority_rx_q_buffer_addr_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hi_priority_rx_q_buffer_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hi_priority_rx_q_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_priority_rx_q_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_priority_rx_q_ctrl_pack(uint8_t atim, uint8_t probe_response, uint8_t probe_request, uint8_t beacon, uint8_t hi_priority_q_en)
{
    hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl.val = (((uint32_t)atim << 4) | ((uint32_t)probe_response << 3) | ((uint32_t)probe_request << 2) | ((uint32_t)beacon << 1) | ((uint32_t)hi_priority_q_en << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_priority_rx_q_ctrl_unpack(uint8_t* atim, uint8_t* probe_response, uint8_t* probe_request, uint8_t* beacon, uint8_t* hi_priority_q_en)
{
    T_MAC_PCU_REG_MAC_HI_PRIORITY_RX_Q_CTRL localVal = hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl;

    *atim = localVal.bit_field.atim;
    *probe_response = localVal.bit_field.probe_response;
    *probe_request = localVal.bit_field.probe_request;
    *beacon = localVal.bit_field.beacon;
    *hi_priority_q_en = localVal.bit_field.hi_priority_q_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_hi_priority_rx_q_ctrl_atim_getf(void)
{
    return hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl.bit_field.atim;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_priority_rx_q_ctrl_atim_setf(uint8_t atim)
{
    hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl.bit_field.atim = atim;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_hi_priority_rx_q_ctrl_probe_response_getf(void)
{
    return hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl.bit_field.probe_response;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_priority_rx_q_ctrl_probe_response_setf(uint8_t probe_response)
{
    hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl.bit_field.probe_response = probe_response;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_hi_priority_rx_q_ctrl_probe_request_getf(void)
{
    return hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl.bit_field.probe_request;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_priority_rx_q_ctrl_probe_request_setf(uint8_t probe_request)
{
    hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl.bit_field.probe_request = probe_request;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_hi_priority_rx_q_ctrl_beacon_getf(void)
{
    return hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl.bit_field.beacon;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_priority_rx_q_ctrl_beacon_setf(uint8_t beacon)
{
    hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl.bit_field.beacon = beacon;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_hi_priority_rx_q_ctrl_hi_priority_q_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl.bit_field.hi_priority_q_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_priority_rx_q_ctrl_hi_priority_q_en_setf(uint8_t hi_priority_q_en)
{
    hwp_mac_pcu_reg->mac_hi_priority_rx_q_ctrl.bit_field.hi_priority_q_en = hi_priority_q_en;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_sub_msdu_gap_get(void)
{
    return hwp_mac_pcu_reg->mac_sub_msdu_gap.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_sub_msdu_gap_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_sub_msdu_gap.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sub_msdu_gap_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_sub_msdu_gap.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_sub_msdu_gap_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_SUB_MSDU_GAP localVal = hwp_mac_pcu_reg->mac_sub_msdu_gap;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_sub_msdu_gap_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_sub_msdu_gap.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sub_msdu_gap_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_sub_msdu_gap.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_mac_rx_buffer_length_get(void)
{
    return hwp_mac_pcu_reg->mac_mac_rx_buffer_length.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_mac_rx_buffer_length_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_mac_rx_buffer_length.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_mac_rx_buffer_length_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_mac_rx_buffer_length.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_mac_rx_buffer_length_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_MAC_RX_BUFFER_LENGTH localVal = hwp_mac_pcu_reg->mac_mac_rx_buffer_length;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_mac_rx_buffer_length_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_mac_rx_buffer_length.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_mac_rx_buffer_length_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_mac_rx_buffer_length.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_aifsn_get(void)
{
    return hwp_mac_pcu_reg->mac_aifsn.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_aifsn_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_aifsn.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_aifsn_pack(uint8_t ac_bk, uint8_t ac_be, uint8_t ac_vi, uint8_t ac_vo)
{
    hwp_mac_pcu_reg->mac_aifsn.val = (((uint32_t)ac_bk << 12) | ((uint32_t)ac_be << 8) | ((uint32_t)ac_vi << 4) | ((uint32_t)ac_vo << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_aifsn_unpack(uint8_t* ac_bk, uint8_t* ac_be, uint8_t* ac_vi, uint8_t* ac_vo)
{
    T_MAC_PCU_REG_MAC_AIFSN localVal = hwp_mac_pcu_reg->mac_aifsn;

    *ac_bk = localVal.bit_field.ac_bk;
    *ac_be = localVal.bit_field.ac_be;
    *ac_vi = localVal.bit_field.ac_vi;
    *ac_vo = localVal.bit_field.ac_vo;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_aifsn_ac_bk_getf(void)
{
    return hwp_mac_pcu_reg->mac_aifsn.bit_field.ac_bk;
}

__STATIC_INLINE void mac_pcu_reg_mac_aifsn_ac_bk_setf(uint8_t ac_bk)
{
    hwp_mac_pcu_reg->mac_aifsn.bit_field.ac_bk = ac_bk;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_aifsn_ac_be_getf(void)
{
    return hwp_mac_pcu_reg->mac_aifsn.bit_field.ac_be;
}

__STATIC_INLINE void mac_pcu_reg_mac_aifsn_ac_be_setf(uint8_t ac_be)
{
    hwp_mac_pcu_reg->mac_aifsn.bit_field.ac_be = ac_be;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_aifsn_ac_vi_getf(void)
{
    return hwp_mac_pcu_reg->mac_aifsn.bit_field.ac_vi;
}

__STATIC_INLINE void mac_pcu_reg_mac_aifsn_ac_vi_setf(uint8_t ac_vi)
{
    hwp_mac_pcu_reg->mac_aifsn.bit_field.ac_vi = ac_vi;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_aifsn_ac_vo_getf(void)
{
    return hwp_mac_pcu_reg->mac_aifsn.bit_field.ac_vo;
}

__STATIC_INLINE void mac_pcu_reg_mac_aifsn_ac_vo_setf(uint8_t ac_vo)
{
    hwp_mac_pcu_reg->mac_aifsn.bit_field.ac_vo = ac_vo;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_cwminmax_ac_bk_get(void)
{
    return hwp_mac_pcu_reg->mac_cwminmax_ac_bk.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_bk_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_bk.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_bk_pack(uint8_t max, uint8_t min)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_bk.val = (((uint32_t)max << 4) | ((uint32_t)min << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_bk_unpack(uint8_t* max, uint8_t* min)
{
    T_MAC_PCU_REG_MAC_CWMINMAX_AC_BK localVal = hwp_mac_pcu_reg->mac_cwminmax_ac_bk;

    *max = localVal.bit_field.max;
    *min = localVal.bit_field.min;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_cwminmax_ac_bk_max_getf(void)
{
    return hwp_mac_pcu_reg->mac_cwminmax_ac_bk.bit_field.max;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_bk_max_setf(uint8_t max)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_bk.bit_field.max = max;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_cwminmax_ac_bk_min_getf(void)
{
    return hwp_mac_pcu_reg->mac_cwminmax_ac_bk.bit_field.min;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_bk_min_setf(uint8_t min)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_bk.bit_field.min = min;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_cwminmax_ac_be_get(void)
{
    return hwp_mac_pcu_reg->mac_cwminmax_ac_be.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_be_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_be.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_be_pack(uint8_t max, uint8_t min)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_be.val = (((uint32_t)max << 4) | ((uint32_t)min << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_be_unpack(uint8_t* max, uint8_t* min)
{
    T_MAC_PCU_REG_MAC_CWMINMAX_AC_BE localVal = hwp_mac_pcu_reg->mac_cwminmax_ac_be;

    *max = localVal.bit_field.max;
    *min = localVal.bit_field.min;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_cwminmax_ac_be_max_getf(void)
{
    return hwp_mac_pcu_reg->mac_cwminmax_ac_be.bit_field.max;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_be_max_setf(uint8_t max)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_be.bit_field.max = max;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_cwminmax_ac_be_min_getf(void)
{
    return hwp_mac_pcu_reg->mac_cwminmax_ac_be.bit_field.min;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_be_min_setf(uint8_t min)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_be.bit_field.min = min;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_cwminmax_ac_vi_get(void)
{
    return hwp_mac_pcu_reg->mac_cwminmax_ac_vi.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_vi_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_vi.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_vi_pack(uint8_t max, uint8_t min)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_vi.val = (((uint32_t)max << 4) | ((uint32_t)min << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_vi_unpack(uint8_t* max, uint8_t* min)
{
    T_MAC_PCU_REG_MAC_CWMINMAX_AC_VI localVal = hwp_mac_pcu_reg->mac_cwminmax_ac_vi;

    *max = localVal.bit_field.max;
    *min = localVal.bit_field.min;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_cwminmax_ac_vi_max_getf(void)
{
    return hwp_mac_pcu_reg->mac_cwminmax_ac_vi.bit_field.max;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_vi_max_setf(uint8_t max)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_vi.bit_field.max = max;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_cwminmax_ac_vi_min_getf(void)
{
    return hwp_mac_pcu_reg->mac_cwminmax_ac_vi.bit_field.min;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_vi_min_setf(uint8_t min)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_vi.bit_field.min = min;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_cwminmax_ac_vo_get(void)
{
    return hwp_mac_pcu_reg->mac_cwminmax_ac_vo.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_vo_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_vo.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_vo_pack(uint8_t max, uint8_t min)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_vo.val = (((uint32_t)max << 4) | ((uint32_t)min << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_vo_unpack(uint8_t* max, uint8_t* min)
{
    T_MAC_PCU_REG_MAC_CWMINMAX_AC_VO localVal = hwp_mac_pcu_reg->mac_cwminmax_ac_vo;

    *max = localVal.bit_field.max;
    *min = localVal.bit_field.min;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_cwminmax_ac_vo_max_getf(void)
{
    return hwp_mac_pcu_reg->mac_cwminmax_ac_vo.bit_field.max;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_vo_max_setf(uint8_t max)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_vo.bit_field.max = max;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_cwminmax_ac_vo_min_getf(void)
{
    return hwp_mac_pcu_reg->mac_cwminmax_ac_vo.bit_field.min;
}

__STATIC_INLINE void mac_pcu_reg_mac_cwminmax_ac_vo_min_setf(uint8_t min)
{
    hwp_mac_pcu_reg->mac_cwminmax_ac_vo.bit_field.min = min;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_edca_txop_limit_ac_bk_be_get(void)
{
    return hwp_mac_pcu_reg->mac_edca_txop_limit_ac_bk_be.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_edca_txop_limit_ac_bk_be_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_edca_txop_limit_ac_bk_be.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_edca_txop_limit_ac_bk_be_pack(uint16_t ac_be, uint16_t ac_bk)
{
    hwp_mac_pcu_reg->mac_edca_txop_limit_ac_bk_be.val = (((uint32_t)ac_be << 16) | ((uint32_t)ac_bk << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_edca_txop_limit_ac_bk_be_unpack(uint16_t* ac_be, uint16_t* ac_bk)
{
    T_MAC_PCU_REG_MAC_EDCA_TXOP_LIMIT_AC_BK_BE localVal = hwp_mac_pcu_reg->mac_edca_txop_limit_ac_bk_be;

    *ac_be = localVal.bit_field.ac_be;
    *ac_bk = localVal.bit_field.ac_bk;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_edca_txop_limit_ac_bk_be_ac_be_getf(void)
{
    return hwp_mac_pcu_reg->mac_edca_txop_limit_ac_bk_be.bit_field.ac_be;
}

__STATIC_INLINE void mac_pcu_reg_mac_edca_txop_limit_ac_bk_be_ac_be_setf(uint16_t ac_be)
{
    hwp_mac_pcu_reg->mac_edca_txop_limit_ac_bk_be.bit_field.ac_be = ac_be;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_edca_txop_limit_ac_bk_be_ac_bk_getf(void)
{
    return hwp_mac_pcu_reg->mac_edca_txop_limit_ac_bk_be.bit_field.ac_bk;
}

__STATIC_INLINE void mac_pcu_reg_mac_edca_txop_limit_ac_bk_be_ac_bk_setf(uint16_t ac_bk)
{
    hwp_mac_pcu_reg->mac_edca_txop_limit_ac_bk_be.bit_field.ac_bk = ac_bk;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_edca_txop_limit_ac_vi_vo_get(void)
{
    return hwp_mac_pcu_reg->mac_edca_txop_limit_ac_vi_vo.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_edca_txop_limit_ac_vi_vo_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_edca_txop_limit_ac_vi_vo.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_edca_txop_limit_ac_vi_vo_pack(uint16_t ac_vo, uint16_t ac_vi)
{
    hwp_mac_pcu_reg->mac_edca_txop_limit_ac_vi_vo.val = (((uint32_t)ac_vo << 16) | ((uint32_t)ac_vi << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_edca_txop_limit_ac_vi_vo_unpack(uint16_t* ac_vo, uint16_t* ac_vi)
{
    T_MAC_PCU_REG_MAC_EDCA_TXOP_LIMIT_AC_VI_VO localVal = hwp_mac_pcu_reg->mac_edca_txop_limit_ac_vi_vo;

    *ac_vo = localVal.bit_field.ac_vo;
    *ac_vi = localVal.bit_field.ac_vi;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_edca_txop_limit_ac_vi_vo_ac_vo_getf(void)
{
    return hwp_mac_pcu_reg->mac_edca_txop_limit_ac_vi_vo.bit_field.ac_vo;
}

__STATIC_INLINE void mac_pcu_reg_mac_edca_txop_limit_ac_vi_vo_ac_vo_setf(uint16_t ac_vo)
{
    hwp_mac_pcu_reg->mac_edca_txop_limit_ac_vi_vo.bit_field.ac_vo = ac_vo;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_edca_txop_limit_ac_vi_vo_ac_vi_getf(void)
{
    return hwp_mac_pcu_reg->mac_edca_txop_limit_ac_vi_vo.bit_field.ac_vi;
}

__STATIC_INLINE void mac_pcu_reg_mac_edca_txop_limit_ac_vi_vo_ac_vi_setf(uint16_t ac_vi)
{
    hwp_mac_pcu_reg->mac_edca_txop_limit_ac_vi_vo.bit_field.ac_vi = ac_vi;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_bk_first_frame_pointer_get(void)
{
    return hwp_mac_pcu_reg->mac_ac_bk_first_frame_pointer.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_bk_first_frame_pointer_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_bk_first_frame_pointer.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_bk_first_frame_pointer_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_bk_first_frame_pointer.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_bk_first_frame_pointer_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_AC_BK_FIRST_FRAME_POINTER localVal = hwp_mac_pcu_reg->mac_ac_bk_first_frame_pointer;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_bk_first_frame_pointer_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_bk_first_frame_pointer.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_bk_first_frame_pointer_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_bk_first_frame_pointer.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_bk_q_retry_cnts_get(void)
{
    return hwp_mac_pcu_reg->mac_ac_bk_q_retry_cnts.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_bk_q_retry_cnts_unpack(uint16_t* long_retry_cnt, uint16_t* short_retry_cnt)
{
    T_MAC_PCU_REG_MAC_AC_BK_Q_RETRY_CNTS localVal = hwp_mac_pcu_reg->mac_ac_bk_q_retry_cnts;

    *long_retry_cnt = localVal.bit_field.long_retry_cnt;
    *short_retry_cnt = localVal.bit_field.short_retry_cnt;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ac_bk_q_retry_cnts_long_retry_cnt_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_bk_q_retry_cnts.bit_field.long_retry_cnt;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ac_bk_q_retry_cnts_short_retry_cnt_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_bk_q_retry_cnts.bit_field.short_retry_cnt;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_be_first_frame_pointer_get(void)
{
    return hwp_mac_pcu_reg->mac_ac_be_first_frame_pointer.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_be_first_frame_pointer_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_be_first_frame_pointer.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_be_first_frame_pointer_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_be_first_frame_pointer.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_be_first_frame_pointer_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_AC_BE_FIRST_FRAME_POINTER localVal = hwp_mac_pcu_reg->mac_ac_be_first_frame_pointer;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_be_first_frame_pointer_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_be_first_frame_pointer.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_be_first_frame_pointer_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_be_first_frame_pointer.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_be_q_retry_cnts_get(void)
{
    return hwp_mac_pcu_reg->mac_ac_be_q_retry_cnts.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_be_q_retry_cnts_unpack(uint16_t* long_retry_cnt, uint16_t* short_retry_cnt)
{
    T_MAC_PCU_REG_MAC_AC_BE_Q_RETRY_CNTS localVal = hwp_mac_pcu_reg->mac_ac_be_q_retry_cnts;

    *long_retry_cnt = localVal.bit_field.long_retry_cnt;
    *short_retry_cnt = localVal.bit_field.short_retry_cnt;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ac_be_q_retry_cnts_long_retry_cnt_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_be_q_retry_cnts.bit_field.long_retry_cnt;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ac_be_q_retry_cnts_short_retry_cnt_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_be_q_retry_cnts.bit_field.short_retry_cnt;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_vi_first_frame_pointer_get(void)
{
    return hwp_mac_pcu_reg->mac_ac_vi_first_frame_pointer.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vi_first_frame_pointer_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_vi_first_frame_pointer.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vi_first_frame_pointer_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_vi_first_frame_pointer.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vi_first_frame_pointer_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_AC_VI_FIRST_FRAME_POINTER localVal = hwp_mac_pcu_reg->mac_ac_vi_first_frame_pointer;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_vi_first_frame_pointer_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_vi_first_frame_pointer.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vi_first_frame_pointer_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_vi_first_frame_pointer.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_vi_q_retry_cnts_get(void)
{
    return hwp_mac_pcu_reg->mac_ac_vi_q_retry_cnts.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vi_q_retry_cnts_unpack(uint16_t* long_retry_cnt, uint16_t* short_retry_cnt)
{
    T_MAC_PCU_REG_MAC_AC_VI_Q_RETRY_CNTS localVal = hwp_mac_pcu_reg->mac_ac_vi_q_retry_cnts;

    *long_retry_cnt = localVal.bit_field.long_retry_cnt;
    *short_retry_cnt = localVal.bit_field.short_retry_cnt;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ac_vi_q_retry_cnts_long_retry_cnt_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_vi_q_retry_cnts.bit_field.long_retry_cnt;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ac_vi_q_retry_cnts_short_retry_cnt_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_vi_q_retry_cnts.bit_field.short_retry_cnt;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_vo_first_frame_pointer_get(void)
{
    return hwp_mac_pcu_reg->mac_ac_vo_first_frame_pointer.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vo_first_frame_pointer_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_vo_first_frame_pointer.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vo_first_frame_pointer_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_vo_first_frame_pointer.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vo_first_frame_pointer_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_AC_VO_FIRST_FRAME_POINTER localVal = hwp_mac_pcu_reg->mac_ac_vo_first_frame_pointer;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_vo_first_frame_pointer_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_vo_first_frame_pointer.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vo_first_frame_pointer_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_vo_first_frame_pointer.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_vo_q_retry_cnts_get(void)
{
    return hwp_mac_pcu_reg->mac_ac_vo_q_retry_cnts.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vo_q_retry_cnts_unpack(uint16_t* long_retry_cnt, uint16_t* short_retry_cnt)
{
    T_MAC_PCU_REG_MAC_AC_VO_Q_RETRY_CNTS localVal = hwp_mac_pcu_reg->mac_ac_vo_q_retry_cnts;

    *long_retry_cnt = localVal.bit_field.long_retry_cnt;
    *short_retry_cnt = localVal.bit_field.short_retry_cnt;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ac_vo_q_retry_cnts_long_retry_cnt_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_vo_q_retry_cnts.bit_field.long_retry_cnt;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ac_vo_q_retry_cnts_short_retry_cnt_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_vo_q_retry_cnts.bit_field.short_retry_cnt;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hi_prioriy_first_frame_pointer_get(void)
{
    return hwp_mac_pcu_reg->mac_hi_prioriy_first_frame_pointer.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_prioriy_first_frame_pointer_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hi_prioriy_first_frame_pointer.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_prioriy_first_frame_pointer_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hi_prioriy_first_frame_pointer.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_prioriy_first_frame_pointer_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_HI_PRIORIY_FIRST_FRAME_POINTER localVal = hwp_mac_pcu_reg->mac_hi_prioriy_first_frame_pointer;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hi_prioriy_first_frame_pointer_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_hi_prioriy_first_frame_pointer.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_prioriy_first_frame_pointer_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hi_prioriy_first_frame_pointer.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_msdu_max_tx_lifetime_limit_get(void)
{
    return hwp_mac_pcu_reg->mac_msdu_max_tx_lifetime_limit.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_msdu_max_tx_lifetime_limit_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_msdu_max_tx_lifetime_limit.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_msdu_max_tx_lifetime_limit_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_msdu_max_tx_lifetime_limit.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_msdu_max_tx_lifetime_limit_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_MSDU_MAX_TX_LIFETIME_LIMIT localVal = hwp_mac_pcu_reg->mac_msdu_max_tx_lifetime_limit;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_msdu_max_tx_lifetime_limit_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_msdu_max_tx_lifetime_limit.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_msdu_max_tx_lifetime_limit_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_msdu_max_tx_lifetime_limit.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_be_bk_edca_lifetime_limit_get(void)
{
    return hwp_mac_pcu_reg->mac_ac_be_bk_edca_lifetime_limit.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_be_bk_edca_lifetime_limit_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_be_bk_edca_lifetime_limit.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_be_bk_edca_lifetime_limit_pack(uint16_t ac_be_lifetime, uint16_t ac_bk_lifetime)
{
    hwp_mac_pcu_reg->mac_ac_be_bk_edca_lifetime_limit.val = (((uint32_t)ac_be_lifetime << 16) | ((uint32_t)ac_bk_lifetime << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_be_bk_edca_lifetime_limit_unpack(uint16_t* ac_be_lifetime, uint16_t* ac_bk_lifetime)
{
    T_MAC_PCU_REG_MAC_AC_BE_BK_EDCA_LIFETIME_LIMIT localVal = hwp_mac_pcu_reg->mac_ac_be_bk_edca_lifetime_limit;

    *ac_be_lifetime = localVal.bit_field.ac_be_lifetime;
    *ac_bk_lifetime = localVal.bit_field.ac_bk_lifetime;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ac_be_bk_edca_lifetime_limit_ac_be_lifetime_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_be_bk_edca_lifetime_limit.bit_field.ac_be_lifetime;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_be_bk_edca_lifetime_limit_ac_be_lifetime_setf(uint16_t ac_be_lifetime)
{
    hwp_mac_pcu_reg->mac_ac_be_bk_edca_lifetime_limit.bit_field.ac_be_lifetime = ac_be_lifetime;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ac_be_bk_edca_lifetime_limit_ac_bk_lifetime_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_be_bk_edca_lifetime_limit.bit_field.ac_bk_lifetime;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_be_bk_edca_lifetime_limit_ac_bk_lifetime_setf(uint16_t ac_bk_lifetime)
{
    hwp_mac_pcu_reg->mac_ac_be_bk_edca_lifetime_limit.bit_field.ac_bk_lifetime = ac_bk_lifetime;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ac_vo_vi_edca_lifetime_limit_get(void)
{
    return hwp_mac_pcu_reg->mac_ac_vo_vi_edca_lifetime_limit.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vo_vi_edca_lifetime_limit_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ac_vo_vi_edca_lifetime_limit.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vo_vi_edca_lifetime_limit_pack(uint16_t ac_vo_lifetime, uint16_t ac_vi_lifetime)
{
    hwp_mac_pcu_reg->mac_ac_vo_vi_edca_lifetime_limit.val = (((uint32_t)ac_vo_lifetime << 16) | ((uint32_t)ac_vi_lifetime << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vo_vi_edca_lifetime_limit_unpack(uint16_t* ac_vo_lifetime, uint16_t* ac_vi_lifetime)
{
    T_MAC_PCU_REG_MAC_AC_VO_VI_EDCA_LIFETIME_LIMIT localVal = hwp_mac_pcu_reg->mac_ac_vo_vi_edca_lifetime_limit;

    *ac_vo_lifetime = localVal.bit_field.ac_vo_lifetime;
    *ac_vi_lifetime = localVal.bit_field.ac_vi_lifetime;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ac_vo_vi_edca_lifetime_limit_ac_vo_lifetime_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_vo_vi_edca_lifetime_limit.bit_field.ac_vo_lifetime;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vo_vi_edca_lifetime_limit_ac_vo_lifetime_setf(uint16_t ac_vo_lifetime)
{
    hwp_mac_pcu_reg->mac_ac_vo_vi_edca_lifetime_limit.bit_field.ac_vo_lifetime = ac_vo_lifetime;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ac_vo_vi_edca_lifetime_limit_ac_vi_lifetime_getf(void)
{
    return hwp_mac_pcu_reg->mac_ac_vo_vi_edca_lifetime_limit.bit_field.ac_vi_lifetime;
}

__STATIC_INLINE void mac_pcu_reg_mac_ac_vo_vi_edca_lifetime_limit_ac_vi_lifetime_setf(uint16_t ac_vi_lifetime)
{
    hwp_mac_pcu_reg->mac_ac_vo_vi_edca_lifetime_limit.bit_field.ac_vi_lifetime = ac_vi_lifetime;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority0_first_frame_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority0_first_frame_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority0_first_frame_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority0_first_frame_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority0_first_frame_addr_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority0_first_frame_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority0_first_frame_addr_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY0_FIRST_FRAME_ADDR localVal = hwp_mac_pcu_reg->mac_hcca_sta_priority0_first_frame_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority0_first_frame_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority0_first_frame_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority0_first_frame_addr_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority0_first_frame_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority1_first_frame_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority1_first_frame_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority1_first_frame_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority1_first_frame_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority1_first_frame_addr_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority1_first_frame_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority1_first_frame_addr_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY1_FIRST_FRAME_ADDR localVal = hwp_mac_pcu_reg->mac_hcca_sta_priority1_first_frame_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority1_first_frame_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority1_first_frame_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority1_first_frame_addr_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority1_first_frame_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority2_first_frame_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority2_first_frame_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority2_first_frame_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority2_first_frame_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority2_first_frame_addr_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority2_first_frame_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority2_first_frame_addr_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY2_FIRST_FRAME_ADDR localVal = hwp_mac_pcu_reg->mac_hcca_sta_priority2_first_frame_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority2_first_frame_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority2_first_frame_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority2_first_frame_addr_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority2_first_frame_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority3_first_frame_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority3_first_frame_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority3_first_frame_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority3_first_frame_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority3_first_frame_addr_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority3_first_frame_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority3_first_frame_addr_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY3_FIRST_FRAME_ADDR localVal = hwp_mac_pcu_reg->mac_hcca_sta_priority3_first_frame_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority3_first_frame_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority3_first_frame_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority3_first_frame_addr_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority3_first_frame_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority4_first_frame_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority4_first_frame_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority4_first_frame_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority4_first_frame_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority4_first_frame_addr_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority4_first_frame_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority4_first_frame_addr_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY4_FIRST_FRAME_ADDR localVal = hwp_mac_pcu_reg->mac_hcca_sta_priority4_first_frame_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority4_first_frame_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority4_first_frame_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority4_first_frame_addr_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority4_first_frame_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority5_first_frame_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority5_first_frame_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority5_first_frame_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority5_first_frame_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority5_first_frame_addr_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority5_first_frame_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority5_first_frame_addr_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY5_FIRST_FRAME_ADDR localVal = hwp_mac_pcu_reg->mac_hcca_sta_priority5_first_frame_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority5_first_frame_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority5_first_frame_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority5_first_frame_addr_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority5_first_frame_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority6_first_frame_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority6_first_frame_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority6_first_frame_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority6_first_frame_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority6_first_frame_addr_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority6_first_frame_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority6_first_frame_addr_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY6_FIRST_FRAME_ADDR localVal = hwp_mac_pcu_reg->mac_hcca_sta_priority6_first_frame_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority6_first_frame_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority6_first_frame_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority6_first_frame_addr_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority6_first_frame_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority7_first_frame_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority7_first_frame_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority7_first_frame_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority7_first_frame_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority7_first_frame_addr_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority7_first_frame_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority7_first_frame_addr_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_HCCA_STA_PRIORITY7_FIRST_FRAME_ADDR localVal = hwp_mac_pcu_reg->mac_hcca_sta_priority7_first_frame_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hcca_sta_priority7_first_frame_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_hcca_sta_priority7_first_frame_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_hcca_sta_priority7_first_frame_addr_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_hcca_sta_priority7_first_frame_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tsf_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_tsf_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tsf_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_ctrl_pack(uint8_t beacon_tx_suspend, uint8_t start_join, uint8_t tsf_en)
{
    hwp_mac_pcu_reg->mac_tsf_ctrl.val = (((uint32_t)beacon_tx_suspend << 2) | ((uint32_t)start_join << 1) | ((uint32_t)tsf_en << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_ctrl_unpack(uint8_t* beacon_tx_suspend, uint8_t* start_join, uint8_t* tsf_en)
{
    T_MAC_PCU_REG_MAC_TSF_CTRL localVal = hwp_mac_pcu_reg->mac_tsf_ctrl;

    *beacon_tx_suspend = localVal.bit_field.beacon_tx_suspend;
    *start_join = localVal.bit_field.start_join;
    *tsf_en = localVal.bit_field.tsf_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tsf_ctrl_beacon_tx_suspend_getf(void)
{
    return hwp_mac_pcu_reg->mac_tsf_ctrl.bit_field.beacon_tx_suspend;
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_ctrl_beacon_tx_suspend_setf(uint8_t beacon_tx_suspend)
{
    hwp_mac_pcu_reg->mac_tsf_ctrl.bit_field.beacon_tx_suspend = beacon_tx_suspend;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tsf_ctrl_start_join_getf(void)
{
    return hwp_mac_pcu_reg->mac_tsf_ctrl.bit_field.start_join;
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_ctrl_start_join_setf(uint8_t start_join)
{
    hwp_mac_pcu_reg->mac_tsf_ctrl.bit_field.start_join = start_join;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tsf_ctrl_tsf_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_tsf_ctrl.bit_field.tsf_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_ctrl_tsf_en_setf(uint8_t tsf_en)
{
    hwp_mac_pcu_reg->mac_tsf_ctrl.bit_field.tsf_en = tsf_en;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tsf_timer_read_value_high_get(void)
{
    return hwp_mac_pcu_reg->mac_tsf_timer_read_value_high.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_timer_read_value_high_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tsf_timer_read_value_high.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_timer_read_value_high_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tsf_timer_read_value_high.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_timer_read_value_high_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_TSF_TIMER_READ_VALUE_HIGH localVal = hwp_mac_pcu_reg->mac_tsf_timer_read_value_high;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tsf_timer_read_value_high_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_tsf_timer_read_value_high.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_timer_read_value_high_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tsf_timer_read_value_high.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tsf_timer_read_value_low_get(void)
{
    return hwp_mac_pcu_reg->mac_tsf_timer_read_value_low.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_timer_read_value_low_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tsf_timer_read_value_low.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_timer_read_value_low_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tsf_timer_read_value_low.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_timer_read_value_low_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_TSF_TIMER_READ_VALUE_LOW localVal = hwp_mac_pcu_reg->mac_tsf_timer_read_value_low;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tsf_timer_read_value_low_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_tsf_timer_read_value_low.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tsf_timer_read_value_low_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tsf_timer_read_value_low.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_beacon_period_get(void)
{
    return hwp_mac_pcu_reg->mac_beacon_period.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_period_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_beacon_period.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_period_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_beacon_period.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_period_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_BEACON_PERIOD localVal = hwp_mac_pcu_reg->mac_beacon_period;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_beacon_period_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_beacon_period.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_period_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_beacon_period.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_dtim_period_get(void)
{
    return hwp_mac_pcu_reg->mac_dtim_period.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_dtim_period_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_dtim_period.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_dtim_period_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_dtim_period.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_dtim_period_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_DTIM_PERIOD localVal = hwp_mac_pcu_reg->mac_dtim_period;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_dtim_period_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_dtim_period.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_dtim_period_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_dtim_period.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_beacon_packet_pointer_get(void)
{
    return hwp_mac_pcu_reg->mac_beacon_packet_pointer.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_packet_pointer_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_beacon_packet_pointer.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_packet_pointer_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_beacon_packet_pointer.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_packet_pointer_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_BEACON_PACKET_POINTER localVal = hwp_mac_pcu_reg->mac_beacon_packet_pointer;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_beacon_packet_pointer_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_beacon_packet_pointer.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_packet_pointer_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_beacon_packet_pointer.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_beacon_tx_param_get(void)
{
    return hwp_mac_pcu_reg->mac_beacon_tx_param.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_tx_param_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_beacon_tx_param.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_tx_param_pack(uint8_t power_level, uint8_t data_rate, uint16_t length)
{
    hwp_mac_pcu_reg->mac_beacon_tx_param.val = (((uint32_t)power_level << 24) | ((uint32_t)data_rate << 16) | ((uint32_t)length << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_tx_param_unpack(uint8_t* power_level, uint8_t* data_rate, uint16_t* length)
{
    T_MAC_PCU_REG_MAC_BEACON_TX_PARAM localVal = hwp_mac_pcu_reg->mac_beacon_tx_param;

    *power_level = localVal.bit_field.power_level;
    *data_rate = localVal.bit_field.data_rate;
    *length = localVal.bit_field.length;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_beacon_tx_param_power_level_getf(void)
{
    return hwp_mac_pcu_reg->mac_beacon_tx_param.bit_field.power_level;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_tx_param_power_level_setf(uint8_t power_level)
{
    hwp_mac_pcu_reg->mac_beacon_tx_param.bit_field.power_level = power_level;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_beacon_tx_param_data_rate_getf(void)
{
    return hwp_mac_pcu_reg->mac_beacon_tx_param.bit_field.data_rate;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_tx_param_data_rate_setf(uint8_t data_rate)
{
    hwp_mac_pcu_reg->mac_beacon_tx_param.bit_field.data_rate = data_rate;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_beacon_tx_param_length_getf(void)
{
    return hwp_mac_pcu_reg->mac_beacon_tx_param.bit_field.length;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_tx_param_length_setf(uint16_t length)
{
    hwp_mac_pcu_reg->mac_beacon_tx_param.bit_field.length = length;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_dtim_count_status_get(void)
{
    return hwp_mac_pcu_reg->mac_dtim_count_status.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_dtim_count_status_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_DTIM_COUNT_STATUS localVal = hwp_mac_pcu_reg->mac_dtim_count_status;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_dtim_count_status_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_dtim_count_status.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_dtim_count_update_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_dtim_count_update_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_dtim_count_update_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_dtim_count_update_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_dtim_count_update_ctrl_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_dtim_count_update_ctrl.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_dtim_count_update_ctrl_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_DTIM_COUNT_UPDATE_CTRL localVal = hwp_mac_pcu_reg->mac_dtim_count_update_ctrl;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_dtim_count_update_ctrl_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_dtim_count_update_ctrl.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_dtim_count_update_ctrl_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_dtim_count_update_ctrl.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_beacon_phy_tx_mode_get(void)
{
    return hwp_mac_pcu_reg->mac_beacon_phy_tx_mode.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_phy_tx_mode_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_beacon_phy_tx_mode.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_phy_tx_mode_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_beacon_phy_tx_mode.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_phy_tx_mode_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_BEACON_PHY_TX_MODE localVal = hwp_mac_pcu_reg->mac_beacon_phy_tx_mode;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_beacon_phy_tx_mode_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_beacon_phy_tx_mode.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_beacon_phy_tx_mode_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_beacon_phy_tx_mode.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tbtt_timer_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_tbtt_timer_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tbtt_timer_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tbtt_timer_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tbtt_timer_ctrl_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tbtt_timer_ctrl.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_tbtt_timer_ctrl_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_TBTT_TIMER_CTRL localVal = hwp_mac_pcu_reg->mac_tbtt_timer_ctrl;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tbtt_timer_ctrl_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_tbtt_timer_ctrl.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tbtt_timer_ctrl_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tbtt_timer_ctrl.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_protection_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_protection_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_protection_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_ctrl_pack(uint8_t erp_prot_type, uint8_t erp_prot, uint8_t nav_port_type, uint8_t enhanced_nav_prot, uint8_t mode)
{
    hwp_mac_pcu_reg->mac_protection_ctrl.val = (((uint32_t)erp_prot_type << 5) | ((uint32_t)erp_prot << 4) | ((uint32_t)nav_port_type << 3) | ((uint32_t)enhanced_nav_prot << 2) | ((uint32_t)mode << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_ctrl_unpack(uint8_t* erp_prot_type, uint8_t* erp_prot, uint8_t* nav_port_type, uint8_t* enhanced_nav_prot, uint8_t* mode)
{
    T_MAC_PCU_REG_MAC_PROTECTION_CTRL localVal = hwp_mac_pcu_reg->mac_protection_ctrl;

    *erp_prot_type = localVal.bit_field.erp_prot_type;
    *erp_prot = localVal.bit_field.erp_prot;
    *nav_port_type = localVal.bit_field.nav_port_type;
    *enhanced_nav_prot = localVal.bit_field.enhanced_nav_prot;
    *mode = localVal.bit_field.mode;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_protection_ctrl_erp_prot_type_getf(void)
{
    return hwp_mac_pcu_reg->mac_protection_ctrl.bit_field.erp_prot_type;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_ctrl_erp_prot_type_setf(uint8_t erp_prot_type)
{
    hwp_mac_pcu_reg->mac_protection_ctrl.bit_field.erp_prot_type = erp_prot_type;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_protection_ctrl_erp_prot_getf(void)
{
    return hwp_mac_pcu_reg->mac_protection_ctrl.bit_field.erp_prot;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_ctrl_erp_prot_setf(uint8_t erp_prot)
{
    hwp_mac_pcu_reg->mac_protection_ctrl.bit_field.erp_prot = erp_prot;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_protection_ctrl_nav_port_type_getf(void)
{
    return hwp_mac_pcu_reg->mac_protection_ctrl.bit_field.nav_port_type;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_ctrl_nav_port_type_setf(uint8_t nav_port_type)
{
    hwp_mac_pcu_reg->mac_protection_ctrl.bit_field.nav_port_type = nav_port_type;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_protection_ctrl_enhanced_nav_prot_getf(void)
{
    return hwp_mac_pcu_reg->mac_protection_ctrl.bit_field.enhanced_nav_prot;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_ctrl_enhanced_nav_prot_setf(uint8_t enhanced_nav_prot)
{
    hwp_mac_pcu_reg->mac_protection_ctrl.bit_field.enhanced_nav_prot = enhanced_nav_prot;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_protection_ctrl_mode_getf(void)
{
    return hwp_mac_pcu_reg->mac_protection_ctrl.bit_field.mode;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_ctrl_mode_setf(uint8_t mode)
{
    hwp_mac_pcu_reg->mac_protection_ctrl.bit_field.mode = mode;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rts_thresh_get(void)
{
    return hwp_mac_pcu_reg->mac_rts_thresh.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rts_thresh_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rts_thresh.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rts_thresh_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_rts_thresh.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_rts_thresh_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_RTS_THRESH localVal = hwp_mac_pcu_reg->mac_rts_thresh;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_rts_thresh_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_rts_thresh.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rts_thresh_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_rts_thresh.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_protection_data_rate_get(void)
{
    return hwp_mac_pcu_reg->mac_protection_data_rate.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_data_rate_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_protection_data_rate.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_data_rate_pack(uint8_t prot_tx_power_level, uint8_t prot_data_rate)
{
    hwp_mac_pcu_reg->mac_protection_data_rate.val = (((uint32_t)prot_tx_power_level << 8) | ((uint32_t)prot_data_rate << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_data_rate_unpack(uint8_t* prot_tx_power_level, uint8_t* prot_data_rate)
{
    T_MAC_PCU_REG_MAC_PROTECTION_DATA_RATE localVal = hwp_mac_pcu_reg->mac_protection_data_rate;

    *prot_tx_power_level = localVal.bit_field.prot_tx_power_level;
    *prot_data_rate = localVal.bit_field.prot_data_rate;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_protection_data_rate_prot_tx_power_level_getf(void)
{
    return hwp_mac_pcu_reg->mac_protection_data_rate.bit_field.prot_tx_power_level;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_data_rate_prot_tx_power_level_setf(uint8_t prot_tx_power_level)
{
    hwp_mac_pcu_reg->mac_protection_data_rate.bit_field.prot_tx_power_level = prot_tx_power_level;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_protection_data_rate_prot_data_rate_getf(void)
{
    return hwp_mac_pcu_reg->mac_protection_data_rate.bit_field.prot_data_rate;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_data_rate_prot_data_rate_setf(uint8_t prot_data_rate)
{
    hwp_mac_pcu_reg->mac_protection_data_rate.bit_field.prot_data_rate = prot_data_rate;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_txop_holder_addr_low_get(void)
{
    return hwp_mac_pcu_reg->mac_txop_holder_addr_low.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_txop_holder_addr_low_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_TXOP_HOLDER_ADDR_LOW localVal = hwp_mac_pcu_reg->mac_txop_holder_addr_low;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_txop_holder_addr_low_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_txop_holder_addr_low.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_txop_holder_addr_high_get(void)
{
    return hwp_mac_pcu_reg->mac_txop_holder_addr_high.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_txop_holder_addr_high_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_TXOP_HOLDER_ADDR_HIGH localVal = hwp_mac_pcu_reg->mac_txop_holder_addr_high;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_txop_holder_addr_high_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_txop_holder_addr_high.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_resp_rate_lut0_get(void)
{
    return hwp_mac_pcu_reg->mac_resp_rate_lut0.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_resp_rate_lut0_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_resp_rate_lut0.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_resp_rate_lut0_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_resp_rate_lut0.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_resp_rate_lut0_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_RESP_RATE_LUT0 localVal = hwp_mac_pcu_reg->mac_resp_rate_lut0;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_resp_rate_lut0_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_resp_rate_lut0.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_resp_rate_lut0_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_resp_rate_lut0.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_resp_rate_lut1_get(void)
{
    return hwp_mac_pcu_reg->mac_resp_rate_lut1.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_resp_rate_lut1_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_resp_rate_lut1.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_resp_rate_lut1_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_resp_rate_lut1.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_resp_rate_lut1_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_RESP_RATE_LUT1 localVal = hwp_mac_pcu_reg->mac_resp_rate_lut1;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_resp_rate_lut1_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_resp_rate_lut1.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_resp_rate_lut1_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_resp_rate_lut1.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_fragmentation_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_fragmentation_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_fragmentation_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_fragmentation_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_fragmentation_ctrl_pack(uint16_t mini_txop_frag_length, uint16_t frag_thresh)
{
    hwp_mac_pcu_reg->mac_fragmentation_ctrl.val = (((uint32_t)mini_txop_frag_length << 16) | ((uint32_t)frag_thresh << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_fragmentation_ctrl_unpack(uint16_t* mini_txop_frag_length, uint16_t* frag_thresh)
{
    T_MAC_PCU_REG_MAC_FRAGMENTATION_CTRL localVal = hwp_mac_pcu_reg->mac_fragmentation_ctrl;

    *mini_txop_frag_length = localVal.bit_field.mini_txop_frag_length;
    *frag_thresh = localVal.bit_field.frag_thresh;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_fragmentation_ctrl_mini_txop_frag_length_getf(void)
{
    return hwp_mac_pcu_reg->mac_fragmentation_ctrl.bit_field.mini_txop_frag_length;
}

__STATIC_INLINE void mac_pcu_reg_mac_fragmentation_ctrl_mini_txop_frag_length_setf(uint16_t mini_txop_frag_length)
{
    hwp_mac_pcu_reg->mac_fragmentation_ctrl.bit_field.mini_txop_frag_length = mini_txop_frag_length;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_fragmentation_ctrl_frag_thresh_getf(void)
{
    return hwp_mac_pcu_reg->mac_fragmentation_ctrl.bit_field.frag_thresh;
}

__STATIC_INLINE void mac_pcu_reg_mac_fragmentation_ctrl_frag_thresh_setf(uint16_t frag_thresh)
{
    hwp_mac_pcu_reg->mac_fragmentation_ctrl.bit_field.frag_thresh = frag_thresh;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_protection_tx_mode_get(void)
{
    return hwp_mac_pcu_reg->mac_protection_tx_mode.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_tx_mode_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_protection_tx_mode.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_tx_mode_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_protection_tx_mode.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_tx_mode_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_PROTECTION_TX_MODE localVal = hwp_mac_pcu_reg->mac_protection_tx_mode;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_protection_tx_mode_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_protection_tx_mode.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_protection_tx_mode_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_protection_tx_mode.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ht_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_pack(uint8_t lsig_nav_update_en, uint8_t first_frame_txop_limit_check, uint8_t sec_channel_offset, uint8_t fc_prot_type, uint8_t ppdu_demotion_in_mid_txop_40mhz, uint8_t ppdu_demotion_in_start_txop_40mhz, uint8_t txop_promotion_40mhz, uint8_t fc_sta_en, uint8_t disable_rts_cts, uint8_t rifs_burst_en, uint8_t non_gf_sta_present, uint8_t ht_prot_type, uint8_t long_nav_support, uint8_t ht_operation_mode)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.val = (((uint32_t)lsig_nav_update_en << 15) | ((uint32_t)first_frame_txop_limit_check << 14) | ((uint32_t)sec_channel_offset << 13) | ((uint32_t)fc_prot_type << 12) | ((uint32_t)ppdu_demotion_in_mid_txop_40mhz << 11) | ((uint32_t)ppdu_demotion_in_start_txop_40mhz << 10) | ((uint32_t)txop_promotion_40mhz << 9) | ((uint32_t)fc_sta_en << 8) | ((uint32_t)disable_rts_cts << 7) | ((uint32_t)rifs_burst_en << 6) | ((uint32_t)non_gf_sta_present << 5) | ((uint32_t)ht_prot_type << 3) | ((uint32_t)long_nav_support << 2) | ((uint32_t)ht_operation_mode << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_unpack(uint8_t* lsig_nav_update_en, uint8_t* first_frame_txop_limit_check, uint8_t* sec_channel_offset, uint8_t* fc_prot_type, uint8_t* ppdu_demotion_in_mid_txop_40mhz, uint8_t* ppdu_demotion_in_start_txop_40mhz, uint8_t* txop_promotion_40mhz, uint8_t* fc_sta_en, uint8_t* disable_rts_cts, uint8_t* rifs_burst_en, uint8_t* non_gf_sta_present, uint8_t* ht_prot_type, uint8_t* long_nav_support, uint8_t* ht_operation_mode)
{
    T_MAC_PCU_REG_MAC_HT_CTRL localVal = hwp_mac_pcu_reg->mac_ht_ctrl;

    *lsig_nav_update_en = localVal.bit_field.lsig_nav_update_en;
    *first_frame_txop_limit_check = localVal.bit_field.first_frame_txop_limit_check;
    *sec_channel_offset = localVal.bit_field.sec_channel_offset;
    *fc_prot_type = localVal.bit_field.fc_prot_type;
    *ppdu_demotion_in_mid_txop_40mhz = localVal.bit_field.ppdu_demotion_in_mid_txop_40mhz;
    *ppdu_demotion_in_start_txop_40mhz = localVal.bit_field.ppdu_demotion_in_start_txop_40mhz;
    *txop_promotion_40mhz = localVal.bit_field.txop_promotion_40mhz;
    *fc_sta_en = localVal.bit_field.fc_sta_en;
    *disable_rts_cts = localVal.bit_field.disable_rts_cts;
    *rifs_burst_en = localVal.bit_field.rifs_burst_en;
    *non_gf_sta_present = localVal.bit_field.non_gf_sta_present;
    *ht_prot_type = localVal.bit_field.ht_prot_type;
    *long_nav_support = localVal.bit_field.long_nav_support;
    *ht_operation_mode = localVal.bit_field.ht_operation_mode;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_lsig_nav_update_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.lsig_nav_update_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_lsig_nav_update_en_setf(uint8_t lsig_nav_update_en)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.lsig_nav_update_en = lsig_nav_update_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_first_frame_txop_limit_check_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.first_frame_txop_limit_check;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_first_frame_txop_limit_check_setf(uint8_t first_frame_txop_limit_check)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.first_frame_txop_limit_check = first_frame_txop_limit_check;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_sec_channel_offset_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.sec_channel_offset;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_sec_channel_offset_setf(uint8_t sec_channel_offset)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.sec_channel_offset = sec_channel_offset;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_fc_prot_type_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.fc_prot_type;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_fc_prot_type_setf(uint8_t fc_prot_type)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.fc_prot_type = fc_prot_type;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_ppdu_demotion_in_mid_txop_40mhz_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.ppdu_demotion_in_mid_txop_40mhz;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_ppdu_demotion_in_mid_txop_40mhz_setf(uint8_t ppdu_demotion_in_mid_txop_40mhz)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.ppdu_demotion_in_mid_txop_40mhz = ppdu_demotion_in_mid_txop_40mhz;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_ppdu_demotion_in_start_txop_40mhz_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.ppdu_demotion_in_start_txop_40mhz;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_ppdu_demotion_in_start_txop_40mhz_setf(uint8_t ppdu_demotion_in_start_txop_40mhz)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.ppdu_demotion_in_start_txop_40mhz = ppdu_demotion_in_start_txop_40mhz;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_txop_promotion_40mhz_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.txop_promotion_40mhz;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_txop_promotion_40mhz_setf(uint8_t txop_promotion_40mhz)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.txop_promotion_40mhz = txop_promotion_40mhz;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_fc_sta_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.fc_sta_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_fc_sta_en_setf(uint8_t fc_sta_en)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.fc_sta_en = fc_sta_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_disable_rts_cts_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.disable_rts_cts;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_disable_rts_cts_setf(uint8_t disable_rts_cts)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.disable_rts_cts = disable_rts_cts;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_rifs_burst_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.rifs_burst_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_rifs_burst_en_setf(uint8_t rifs_burst_en)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.rifs_burst_en = rifs_burst_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_non_gf_sta_present_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.non_gf_sta_present;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_non_gf_sta_present_setf(uint8_t non_gf_sta_present)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.non_gf_sta_present = non_gf_sta_present;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_ht_prot_type_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.ht_prot_type;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_ht_prot_type_setf(uint8_t ht_prot_type)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.ht_prot_type = ht_prot_type;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_long_nav_support_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.long_nav_support;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_long_nav_support_setf(uint8_t long_nav_support)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.long_nav_support = long_nav_support;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ht_ctrl_ht_operation_mode_getf(void)
{
    return hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.ht_operation_mode;
}

__STATIC_INLINE void mac_pcu_reg_mac_ht_ctrl_ht_operation_mode_setf(uint8_t ht_operation_mode)
{
    hwp_mac_pcu_reg->mac_ht_ctrl.bit_field.ht_operation_mode = ht_operation_mode;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_pack(uint8_t mpdu_aggr_en, uint8_t max_num_aggr, uint8_t max_rx_amdpu_factor, uint8_t mini_mpdu_start_spacing, uint8_t lut_index, uint8_t read_write, uint8_t lut_operation_en)
{
    hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.val = (((uint32_t)mpdu_aggr_en << 23) | ((uint32_t)max_num_aggr << 15) | ((uint32_t)max_rx_amdpu_factor << 13) | ((uint32_t)mini_mpdu_start_spacing << 10) | ((uint32_t)lut_index << 2) | ((uint32_t)read_write << 1) | ((uint32_t)lut_operation_en << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_unpack(uint8_t* mpdu_aggr_en, uint8_t* max_num_aggr, uint8_t* max_rx_amdpu_factor, uint8_t* mini_mpdu_start_spacing, uint8_t* lut_index, uint8_t* read_write, uint8_t* lut_operation_en)
{
    T_MAC_PCU_REG_MAC_RA_AMPDU_SPECIFIC_LUT_CTRL localVal = hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl;

    *mpdu_aggr_en = localVal.bit_field.mpdu_aggr_en;
    *max_num_aggr = localVal.bit_field.max_num_aggr;
    *max_rx_amdpu_factor = localVal.bit_field.max_rx_amdpu_factor;
    *mini_mpdu_start_spacing = localVal.bit_field.mini_mpdu_start_spacing;
    *lut_index = localVal.bit_field.lut_index;
    *read_write = localVal.bit_field.read_write;
    *lut_operation_en = localVal.bit_field.lut_operation_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_mpdu_aggr_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.mpdu_aggr_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_mpdu_aggr_en_setf(uint8_t mpdu_aggr_en)
{
    hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.mpdu_aggr_en = mpdu_aggr_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_max_num_aggr_getf(void)
{
    return hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.max_num_aggr;
}

__STATIC_INLINE void mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_max_num_aggr_setf(uint8_t max_num_aggr)
{
    hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.max_num_aggr = max_num_aggr;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_max_rx_amdpu_factor_getf(void)
{
    return hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.max_rx_amdpu_factor;
}

__STATIC_INLINE void mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_max_rx_amdpu_factor_setf(uint8_t max_rx_amdpu_factor)
{
    hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.max_rx_amdpu_factor = max_rx_amdpu_factor;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_mini_mpdu_start_spacing_getf(void)
{
    return hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.mini_mpdu_start_spacing;
}

__STATIC_INLINE void mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_mini_mpdu_start_spacing_setf(uint8_t mini_mpdu_start_spacing)
{
    hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.mini_mpdu_start_spacing = mini_mpdu_start_spacing;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_lut_index_getf(void)
{
    return hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.lut_index;
}

__STATIC_INLINE void mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_lut_index_setf(uint8_t lut_index)
{
    hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.lut_index = lut_index;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_read_write_getf(void)
{
    return hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.read_write;
}

__STATIC_INLINE void mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_read_write_setf(uint8_t read_write)
{
    hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.read_write = read_write;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_lut_operation_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.lut_operation_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_ra_ampdu_specific_lut_ctrl_lut_operation_en_setf(uint8_t lut_operation_en)
{
    hwp_mac_pcu_reg->mac_ra_ampdu_specific_lut_ctrl.bit_field.lut_operation_en = lut_operation_en;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_ampdu_cnt_get(void)
{
    return hwp_mac_pcu_reg->mac_tx_ampdu_cnt.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_ampdu_cnt_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_TX_AMPDU_CNT localVal = hwp_mac_pcu_reg->mac_tx_ampdu_cnt;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_ampdu_cnt_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_ampdu_cnt.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_mpdu_in_ampdu_get(void)
{
    return hwp_mac_pcu_reg->mac_tx_mpdu_in_ampdu.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_mpdu_in_ampdu_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_TX_MPDU_IN_AMPDU localVal = hwp_mac_pcu_reg->mac_tx_mpdu_in_ampdu;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_mpdu_in_ampdu_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_mpdu_in_ampdu.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_octets_in_ampdu_get(void)
{
    return hwp_mac_pcu_reg->mac_tx_octets_in_ampdu.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_octets_in_ampdu_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_TX_OCTETS_IN_AMPDU localVal = hwp_mac_pcu_reg->mac_tx_octets_in_ampdu;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_octets_in_ampdu_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_octets_in_ampdu.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_max_aggr_non_ht_frames_get(void)
{
    return hwp_mac_pcu_reg->mac_max_aggr_non_ht_frames.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_max_aggr_non_ht_frames_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_max_aggr_non_ht_frames.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_max_aggr_non_ht_frames_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_max_aggr_non_ht_frames.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_max_aggr_non_ht_frames_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_MAX_AGGR_NON_HT_FRAMES localVal = hwp_mac_pcu_reg->mac_max_aggr_non_ht_frames;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_max_aggr_non_ht_frames_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_max_aggr_non_ht_frames.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_max_aggr_non_ht_frames_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_max_aggr_non_ht_frames.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_num_20mhz_txop_get(void)
{
    return hwp_mac_pcu_reg->mac_num_20mhz_txop.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_num_20mhz_txop_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_num_20mhz_txop.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_num_20mhz_txop_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_num_20mhz_txop.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_num_20mhz_txop_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_NUM_20MHZ_TXOP localVal = hwp_mac_pcu_reg->mac_num_20mhz_txop;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_num_20mhz_txop_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_num_20mhz_txop.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_num_20mhz_txop_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_num_20mhz_txop.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_num_40mhz_txop_get(void)
{
    return hwp_mac_pcu_reg->mac_num_40mhz_txop.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_num_40mhz_txop_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_num_40mhz_txop.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_num_40mhz_txop_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_num_40mhz_txop.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_num_40mhz_txop_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_NUM_40MHZ_TXOP localVal = hwp_mac_pcu_reg->mac_num_40mhz_txop;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_num_40mhz_txop_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_num_40mhz_txop.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_num_40mhz_txop_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_num_40mhz_txop.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_num_20mhz_mpdu_in_40mhz_txop_get(void)
{
    return hwp_mac_pcu_reg->mac_num_20mhz_mpdu_in_40mhz_txop.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_num_20mhz_mpdu_in_40mhz_txop_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_num_20mhz_mpdu_in_40mhz_txop.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_num_20mhz_mpdu_in_40mhz_txop_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_num_20mhz_mpdu_in_40mhz_txop.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_num_20mhz_mpdu_in_40mhz_txop_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_NUM_20MHZ_MPDU_IN_40MHZ_TXOP localVal = hwp_mac_pcu_reg->mac_num_20mhz_mpdu_in_40mhz_txop;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_num_20mhz_mpdu_in_40mhz_txop_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_num_20mhz_mpdu_in_40mhz_txop.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_num_20mhz_mpdu_in_40mhz_txop_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_num_20mhz_mpdu_in_40mhz_txop.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_num_promoted_mpdu_get(void)
{
    return hwp_mac_pcu_reg->mac_num_promoted_mpdu.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_num_promoted_mpdu_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_num_promoted_mpdu.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_num_promoted_mpdu_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_num_promoted_mpdu.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_num_promoted_mpdu_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_NUM_PROMOTED_MPDU localVal = hwp_mac_pcu_reg->mac_num_promoted_mpdu;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_num_promoted_mpdu_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_num_promoted_mpdu.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_num_promoted_mpdu_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_num_promoted_mpdu.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_slot_time_get(void)
{
    return hwp_mac_pcu_reg->mac_slot_time.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_slot_time_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_slot_time.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_slot_time_pack(uint8_t long_slot_time, uint8_t short_slot_time, uint8_t slot_sel)
{
    hwp_mac_pcu_reg->mac_slot_time.val = (((uint32_t)long_slot_time << 6) | ((uint32_t)short_slot_time << 1) | ((uint32_t)slot_sel << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_slot_time_unpack(uint8_t* long_slot_time, uint8_t* short_slot_time, uint8_t* slot_sel)
{
    T_MAC_PCU_REG_MAC_SLOT_TIME localVal = hwp_mac_pcu_reg->mac_slot_time;

    *long_slot_time = localVal.bit_field.long_slot_time;
    *short_slot_time = localVal.bit_field.short_slot_time;
    *slot_sel = localVal.bit_field.slot_sel;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_slot_time_long_slot_time_getf(void)
{
    return hwp_mac_pcu_reg->mac_slot_time.bit_field.long_slot_time;
}

__STATIC_INLINE void mac_pcu_reg_mac_slot_time_long_slot_time_setf(uint8_t long_slot_time)
{
    hwp_mac_pcu_reg->mac_slot_time.bit_field.long_slot_time = long_slot_time;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_slot_time_short_slot_time_getf(void)
{
    return hwp_mac_pcu_reg->mac_slot_time.bit_field.short_slot_time;
}

__STATIC_INLINE void mac_pcu_reg_mac_slot_time_short_slot_time_setf(uint8_t short_slot_time)
{
    hwp_mac_pcu_reg->mac_slot_time.bit_field.short_slot_time = short_slot_time;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_slot_time_slot_sel_getf(void)
{
    return hwp_mac_pcu_reg->mac_slot_time.bit_field.slot_sel;
}

__STATIC_INLINE void mac_pcu_reg_mac_slot_time_slot_sel_setf(uint8_t slot_sel)
{
    hwp_mac_pcu_reg->mac_slot_time.bit_field.slot_sel = slot_sel;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_sifs_time_get(void)
{
    return hwp_mac_pcu_reg->mac_sifs_time.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_sifs_time_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_sifs_time.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sifs_time_pack(uint8_t ofdm_sifs_time, uint8_t dsss_cck_sifs_time)
{
    hwp_mac_pcu_reg->mac_sifs_time.val = (((uint32_t)ofdm_sifs_time << 8) | ((uint32_t)dsss_cck_sifs_time << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_sifs_time_unpack(uint8_t* ofdm_sifs_time, uint8_t* dsss_cck_sifs_time)
{
    T_MAC_PCU_REG_MAC_SIFS_TIME localVal = hwp_mac_pcu_reg->mac_sifs_time;

    *ofdm_sifs_time = localVal.bit_field.ofdm_sifs_time;
    *dsss_cck_sifs_time = localVal.bit_field.dsss_cck_sifs_time;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_sifs_time_ofdm_sifs_time_getf(void)
{
    return hwp_mac_pcu_reg->mac_sifs_time.bit_field.ofdm_sifs_time;
}

__STATIC_INLINE void mac_pcu_reg_mac_sifs_time_ofdm_sifs_time_setf(uint8_t ofdm_sifs_time)
{
    hwp_mac_pcu_reg->mac_sifs_time.bit_field.ofdm_sifs_time = ofdm_sifs_time;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_sifs_time_dsss_cck_sifs_time_getf(void)
{
    return hwp_mac_pcu_reg->mac_sifs_time.bit_field.dsss_cck_sifs_time;
}

__STATIC_INLINE void mac_pcu_reg_mac_sifs_time_dsss_cck_sifs_time_setf(uint8_t dsss_cck_sifs_time)
{
    hwp_mac_pcu_reg->mac_sifs_time.bit_field.dsss_cck_sifs_time = dsss_cck_sifs_time;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_eifs_time_get(void)
{
    return hwp_mac_pcu_reg->mac_eifs_time.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_eifs_time_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_eifs_time.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_eifs_time_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_eifs_time.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_eifs_time_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_EIFS_TIME localVal = hwp_mac_pcu_reg->mac_eifs_time;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_eifs_time_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_eifs_time.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_eifs_time_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_eifs_time.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ppdu_max_time_get(void)
{
    return hwp_mac_pcu_reg->mac_ppdu_max_time.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ppdu_max_time_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ppdu_max_time.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ppdu_max_time_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_ppdu_max_time.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ppdu_max_time_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_PPDU_MAX_TIME localVal = hwp_mac_pcu_reg->mac_ppdu_max_time;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ppdu_max_time_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_ppdu_max_time.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ppdu_max_time_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_ppdu_max_time.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_sec_channel_slot_cnt_get(void)
{
    return hwp_mac_pcu_reg->mac_sec_channel_slot_cnt.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_sec_channel_slot_cnt_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_sec_channel_slot_cnt.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sec_channel_slot_cnt_pack(uint8_t slot_sel, uint8_t slot_cnt)
{
    hwp_mac_pcu_reg->mac_sec_channel_slot_cnt.val = (((uint32_t)slot_sel << 8) | ((uint32_t)slot_cnt << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_sec_channel_slot_cnt_unpack(uint8_t* slot_sel, uint8_t* slot_cnt)
{
    T_MAC_PCU_REG_MAC_SEC_CHANNEL_SLOT_CNT localVal = hwp_mac_pcu_reg->mac_sec_channel_slot_cnt;

    *slot_sel = localVal.bit_field.slot_sel;
    *slot_cnt = localVal.bit_field.slot_cnt;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_sec_channel_slot_cnt_slot_sel_getf(void)
{
    return hwp_mac_pcu_reg->mac_sec_channel_slot_cnt.bit_field.slot_sel;
}

__STATIC_INLINE void mac_pcu_reg_mac_sec_channel_slot_cnt_slot_sel_setf(uint8_t slot_sel)
{
    hwp_mac_pcu_reg->mac_sec_channel_slot_cnt.bit_field.slot_sel = slot_sel;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_sec_channel_slot_cnt_slot_cnt_getf(void)
{
    return hwp_mac_pcu_reg->mac_sec_channel_slot_cnt.bit_field.slot_cnt;
}

__STATIC_INLINE void mac_pcu_reg_mac_sec_channel_slot_cnt_slot_cnt_setf(uint8_t slot_cnt)
{
    hwp_mac_pcu_reg->mac_sec_channel_slot_cnt.bit_field.slot_cnt = slot_cnt;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_sifs_time2_get(void)
{
    return hwp_mac_pcu_reg->mac_sifs_time2.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_sifs_time2_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_sifs_time2.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sifs_time2_pack(uint8_t ofdm_sifs_time, uint8_t dsss_cck_sifs_time)
{
    hwp_mac_pcu_reg->mac_sifs_time2.val = (((uint32_t)ofdm_sifs_time << 8) | ((uint32_t)dsss_cck_sifs_time << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_sifs_time2_unpack(uint8_t* ofdm_sifs_time, uint8_t* dsss_cck_sifs_time)
{
    T_MAC_PCU_REG_MAC_SIFS_TIME2 localVal = hwp_mac_pcu_reg->mac_sifs_time2;

    *ofdm_sifs_time = localVal.bit_field.ofdm_sifs_time;
    *dsss_cck_sifs_time = localVal.bit_field.dsss_cck_sifs_time;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_sifs_time2_ofdm_sifs_time_getf(void)
{
    return hwp_mac_pcu_reg->mac_sifs_time2.bit_field.ofdm_sifs_time;
}

__STATIC_INLINE void mac_pcu_reg_mac_sifs_time2_ofdm_sifs_time_setf(uint8_t ofdm_sifs_time)
{
    hwp_mac_pcu_reg->mac_sifs_time2.bit_field.ofdm_sifs_time = ofdm_sifs_time;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_sifs_time2_dsss_cck_sifs_time_getf(void)
{
    return hwp_mac_pcu_reg->mac_sifs_time2.bit_field.dsss_cck_sifs_time;
}

__STATIC_INLINE void mac_pcu_reg_mac_sifs_time2_dsss_cck_sifs_time_setf(uint8_t dsss_cck_sifs_time)
{
    hwp_mac_pcu_reg->mac_sifs_time2.bit_field.dsss_cck_sifs_time = dsss_cck_sifs_time;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rifs_time_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_rifs_time_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rifs_time_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rifs_time_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rifs_time_ctrl_pack(uint8_t ctrl_path_rifs_wait_time, uint8_t data_path_rifs_timeout)
{
    hwp_mac_pcu_reg->mac_rifs_time_ctrl.val = (((uint32_t)ctrl_path_rifs_wait_time << 8) | ((uint32_t)data_path_rifs_timeout << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_rifs_time_ctrl_unpack(uint8_t* ctrl_path_rifs_wait_time, uint8_t* data_path_rifs_timeout)
{
    T_MAC_PCU_REG_MAC_RIFS_TIME_CTRL localVal = hwp_mac_pcu_reg->mac_rifs_time_ctrl;

    *ctrl_path_rifs_wait_time = localVal.bit_field.ctrl_path_rifs_wait_time;
    *data_path_rifs_timeout = localVal.bit_field.data_path_rifs_timeout;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rifs_time_ctrl_ctrl_path_rifs_wait_time_getf(void)
{
    return hwp_mac_pcu_reg->mac_rifs_time_ctrl.bit_field.ctrl_path_rifs_wait_time;
}

__STATIC_INLINE void mac_pcu_reg_mac_rifs_time_ctrl_ctrl_path_rifs_wait_time_setf(uint8_t ctrl_path_rifs_wait_time)
{
    hwp_mac_pcu_reg->mac_rifs_time_ctrl.bit_field.ctrl_path_rifs_wait_time = ctrl_path_rifs_wait_time;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rifs_time_ctrl_data_path_rifs_timeout_getf(void)
{
    return hwp_mac_pcu_reg->mac_rifs_time_ctrl.bit_field.data_path_rifs_timeout;
}

__STATIC_INLINE void mac_pcu_reg_mac_rifs_time_ctrl_data_path_rifs_timeout_setf(uint8_t data_path_rifs_timeout)
{
    hwp_mac_pcu_reg->mac_rifs_time_ctrl.bit_field.data_path_rifs_timeout = data_path_rifs_timeout;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_long_retry_limit_get(void)
{
    return hwp_mac_pcu_reg->mac_long_retry_limit.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_long_retry_limit_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_long_retry_limit.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_long_retry_limit_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_long_retry_limit.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_long_retry_limit_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_LONG_RETRY_LIMIT localVal = hwp_mac_pcu_reg->mac_long_retry_limit;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_long_retry_limit_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_long_retry_limit.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_long_retry_limit_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_long_retry_limit.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_short_retry_limit_get(void)
{
    return hwp_mac_pcu_reg->mac_short_retry_limit.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_short_retry_limit_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_short_retry_limit.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_short_retry_limit_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_short_retry_limit.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_short_retry_limit_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_SHORT_RETRY_LIMIT localVal = hwp_mac_pcu_reg->mac_short_retry_limit;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_short_retry_limit_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_short_retry_limit.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_short_retry_limit_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_short_retry_limit.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_seqnum_dup_det_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_seqnum_dup_det_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_seqnum_dup_det_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_seqnum_dup_det_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_seqnum_dup_det_ctrl_pack(uint8_t index, uint8_t tid, uint8_t read_write, uint8_t table_operation_en)
{
    hwp_mac_pcu_reg->mac_seqnum_dup_det_ctrl.val = (((uint32_t)index << 8) | ((uint32_t)tid << 4) | ((uint32_t)read_write << 1) | ((uint32_t)table_operation_en << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_seqnum_dup_det_ctrl_unpack(uint8_t* index, uint8_t* tid, uint8_t* read_write, uint8_t* table_operation_en)
{
    T_MAC_PCU_REG_MAC_SEQNUM_DUP_DET_CTRL localVal = hwp_mac_pcu_reg->mac_seqnum_dup_det_ctrl;

    *index = localVal.bit_field.index;
    *tid = localVal.bit_field.tid;
    *read_write = localVal.bit_field.read_write;
    *table_operation_en = localVal.bit_field.table_operation_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_seqnum_dup_det_ctrl_index_getf(void)
{
    return hwp_mac_pcu_reg->mac_seqnum_dup_det_ctrl.bit_field.index;
}

__STATIC_INLINE void mac_pcu_reg_mac_seqnum_dup_det_ctrl_index_setf(uint8_t index)
{
    hwp_mac_pcu_reg->mac_seqnum_dup_det_ctrl.bit_field.index = index;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_seqnum_dup_det_ctrl_tid_getf(void)
{
    return hwp_mac_pcu_reg->mac_seqnum_dup_det_ctrl.bit_field.tid;
}

__STATIC_INLINE void mac_pcu_reg_mac_seqnum_dup_det_ctrl_tid_setf(uint8_t tid)
{
    hwp_mac_pcu_reg->mac_seqnum_dup_det_ctrl.bit_field.tid = tid;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_seqnum_dup_det_ctrl_read_write_getf(void)
{
    return hwp_mac_pcu_reg->mac_seqnum_dup_det_ctrl.bit_field.read_write;
}

__STATIC_INLINE void mac_pcu_reg_mac_seqnum_dup_det_ctrl_read_write_setf(uint8_t read_write)
{
    hwp_mac_pcu_reg->mac_seqnum_dup_det_ctrl.bit_field.read_write = read_write;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_seqnum_dup_det_ctrl_table_operation_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_seqnum_dup_det_ctrl.bit_field.table_operation_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_seqnum_dup_det_ctrl_table_operation_en_setf(uint8_t table_operation_en)
{
    hwp_mac_pcu_reg->mac_seqnum_dup_det_ctrl.bit_field.table_operation_en = table_operation_en;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_sta_addr_high_get(void)
{
    return hwp_mac_pcu_reg->mac_sta_addr_high.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_sta_addr_high_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_sta_addr_high.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sta_addr_high_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_sta_addr_high.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_sta_addr_high_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_STA_ADDR_HIGH localVal = hwp_mac_pcu_reg->mac_sta_addr_high;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_sta_addr_high_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_sta_addr_high.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sta_addr_high_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_sta_addr_high.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_sta_addr_low_get(void)
{
    return hwp_mac_pcu_reg->mac_sta_addr_low.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_sta_addr_low_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_sta_addr_low.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sta_addr_low_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_sta_addr_low.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_sta_addr_low_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_STA_ADDR_LOW localVal = hwp_mac_pcu_reg->mac_sta_addr_low;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_sta_addr_low_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_sta_addr_low.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sta_addr_low_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_sta_addr_low.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_sequence_number_get(void)
{
    return hwp_mac_pcu_reg->mac_tx_sequence_number.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_sequence_number_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_TX_SEQUENCE_NUMBER localVal = hwp_mac_pcu_reg->mac_tx_sequence_number;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_tx_sequence_number_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_sequence_number.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_pcf_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_pcf_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_pcf_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_pcf_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_pcf_ctrl_pack(uint8_t piggyback_en, uint8_t pcf_mode)
{
    hwp_mac_pcu_reg->mac_pcf_ctrl.val = (((uint32_t)piggyback_en << 2) | ((uint32_t)pcf_mode << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_pcf_ctrl_unpack(uint8_t* piggyback_en, uint8_t* pcf_mode)
{
    T_MAC_PCU_REG_MAC_PCF_CTRL localVal = hwp_mac_pcu_reg->mac_pcf_ctrl;

    *piggyback_en = localVal.bit_field.piggyback_en;
    *pcf_mode = localVal.bit_field.pcf_mode;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pcf_ctrl_piggyback_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_pcf_ctrl.bit_field.piggyback_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_pcf_ctrl_piggyback_en_setf(uint8_t piggyback_en)
{
    hwp_mac_pcu_reg->mac_pcf_ctrl.bit_field.piggyback_en = piggyback_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_pcf_ctrl_pcf_mode_getf(void)
{
    return hwp_mac_pcu_reg->mac_pcf_ctrl.bit_field.pcf_mode;
}

__STATIC_INLINE void mac_pcu_reg_mac_pcf_ctrl_pcf_mode_setf(uint8_t pcf_mode)
{
    hwp_mac_pcu_reg->mac_pcf_ctrl.bit_field.pcf_mode = pcf_mode;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_cfp_max_duration_get(void)
{
    return hwp_mac_pcu_reg->mac_cfp_max_duration.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_max_duration_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_cfp_max_duration.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_max_duration_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_cfp_max_duration.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_max_duration_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_CFP_MAX_DURATION localVal = hwp_mac_pcu_reg->mac_cfp_max_duration;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_cfp_max_duration_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_cfp_max_duration.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_max_duration_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_cfp_max_duration.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_cfp_interval_get(void)
{
    return hwp_mac_pcu_reg->mac_cfp_interval.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_interval_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_cfp_interval.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_interval_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_cfp_interval.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_interval_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_CFP_INTERVAL localVal = hwp_mac_pcu_reg->mac_cfp_interval;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_cfp_interval_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_cfp_interval.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_interval_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_cfp_interval.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_cfp_para_set_element_byte_num_get(void)
{
    return hwp_mac_pcu_reg->mac_cfp_para_set_element_byte_num.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_para_set_element_byte_num_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_cfp_para_set_element_byte_num.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_para_set_element_byte_num_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_cfp_para_set_element_byte_num.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_para_set_element_byte_num_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_CFP_PARA_SET_ELEMENT_BYTE_NUM localVal = hwp_mac_pcu_reg->mac_cfp_para_set_element_byte_num;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_cfp_para_set_element_byte_num_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_cfp_para_set_element_byte_num.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_para_set_element_byte_num_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_cfp_para_set_element_byte_num.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_medium_occupancy_limit_get(void)
{
    return hwp_mac_pcu_reg->mac_medium_occupancy_limit.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_medium_occupancy_limit_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_medium_occupancy_limit.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_medium_occupancy_limit_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_medium_occupancy_limit.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_medium_occupancy_limit_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_MEDIUM_OCCUPANCY_LIMIT localVal = hwp_mac_pcu_reg->mac_medium_occupancy_limit;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_medium_occupancy_limit_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_medium_occupancy_limit.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_medium_occupancy_limit_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_medium_occupancy_limit.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_pcf_q_pointer_get(void)
{
    return hwp_mac_pcu_reg->mac_pcf_q_pointer.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_pcf_q_pointer_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_pcf_q_pointer.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_pcf_q_pointer_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_pcf_q_pointer.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_pcf_q_pointer_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_PCF_Q_POINTER localVal = hwp_mac_pcu_reg->mac_pcf_q_pointer;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_pcf_q_pointer_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_pcf_q_pointer.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_pcf_q_pointer_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_pcf_q_pointer.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_cfp_count_get(void)
{
    return hwp_mac_pcu_reg->mac_cfp_count.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_count_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_CFP_COUNT localVal = hwp_mac_pcu_reg->mac_cfp_count;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_cfp_count_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_cfp_count.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_cfp_unused_duration_get(void)
{
    return hwp_mac_pcu_reg->mac_cfp_unused_duration.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_cfp_unused_duration_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_CFP_UNUSED_DURATION localVal = hwp_mac_pcu_reg->mac_cfp_unused_duration;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_cfp_unused_duration_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_cfp_unused_duration.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_power_management_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_power_management_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_power_management_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_power_management_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_power_management_ctrl_pack(uint8_t ps_clk_switch, uint8_t tx_pwrmgmt, uint8_t rx_dtims, uint8_t pm_status)
{
    hwp_mac_pcu_reg->mac_power_management_ctrl.val = (((uint32_t)ps_clk_switch << 3) | ((uint32_t)tx_pwrmgmt << 2) | ((uint32_t)rx_dtims << 1) | ((uint32_t)pm_status << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_power_management_ctrl_unpack(uint8_t* ps_clk_switch, uint8_t* tx_pwrmgmt, uint8_t* rx_dtims, uint8_t* pm_status)
{
    T_MAC_PCU_REG_MAC_POWER_MANAGEMENT_CTRL localVal = hwp_mac_pcu_reg->mac_power_management_ctrl;

    *ps_clk_switch = localVal.bit_field.ps_clk_switch;
    *tx_pwrmgmt = localVal.bit_field.tx_pwrmgmt;
    *rx_dtims = localVal.bit_field.rx_dtims;
    *pm_status = localVal.bit_field.pm_status;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_power_management_ctrl_ps_clk_switch_getf(void)
{
    return hwp_mac_pcu_reg->mac_power_management_ctrl.bit_field.ps_clk_switch;
}

__STATIC_INLINE void mac_pcu_reg_mac_power_management_ctrl_ps_clk_switch_setf(uint8_t ps_clk_switch)
{
    hwp_mac_pcu_reg->mac_power_management_ctrl.bit_field.ps_clk_switch = ps_clk_switch;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_power_management_ctrl_tx_pwrmgmt_getf(void)
{
    return hwp_mac_pcu_reg->mac_power_management_ctrl.bit_field.tx_pwrmgmt;
}

__STATIC_INLINE void mac_pcu_reg_mac_power_management_ctrl_tx_pwrmgmt_setf(uint8_t tx_pwrmgmt)
{
    hwp_mac_pcu_reg->mac_power_management_ctrl.bit_field.tx_pwrmgmt = tx_pwrmgmt;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_power_management_ctrl_rx_dtims_getf(void)
{
    return hwp_mac_pcu_reg->mac_power_management_ctrl.bit_field.rx_dtims;
}

__STATIC_INLINE void mac_pcu_reg_mac_power_management_ctrl_rx_dtims_setf(uint8_t rx_dtims)
{
    hwp_mac_pcu_reg->mac_power_management_ctrl.bit_field.rx_dtims = rx_dtims;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_power_management_ctrl_pm_status_getf(void)
{
    return hwp_mac_pcu_reg->mac_power_management_ctrl.bit_field.pm_status;
}

__STATIC_INLINE void mac_pcu_reg_mac_power_management_ctrl_pm_status_setf(uint8_t pm_status)
{
    hwp_mac_pcu_reg->mac_power_management_ctrl.bit_field.pm_status = pm_status;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_atim_win_get(void)
{
    return hwp_mac_pcu_reg->mac_atim_win.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_atim_win_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_atim_win.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_atim_win_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_atim_win.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_atim_win_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_ATIM_WIN localVal = hwp_mac_pcu_reg->mac_atim_win;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_atim_win_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_atim_win.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_atim_win_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_atim_win.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_listen_interval_get(void)
{
    return hwp_mac_pcu_reg->mac_listen_interval.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_listen_interval_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_listen_interval.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_listen_interval_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_listen_interval.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_listen_interval_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_LISTEN_INTERVAL localVal = hwp_mac_pcu_reg->mac_listen_interval;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_listen_interval_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_listen_interval.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_listen_interval_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_listen_interval.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_offset_interval_get(void)
{
    return hwp_mac_pcu_reg->mac_offset_interval.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_offset_interval_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_offset_interval.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_offset_interval_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_offset_interval.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_offset_interval_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_OFFSET_INTERVAL localVal = hwp_mac_pcu_reg->mac_offset_interval;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_offset_interval_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_offset_interval.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_offset_interval_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_offset_interval.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_sapsd_ssp_get(void)
{
    return hwp_mac_pcu_reg->mac_sapsd_ssp.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_sapsd_ssp_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_sapsd_ssp.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sapsd_ssp_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_sapsd_ssp.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_sapsd_ssp_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_SAPSD_SSP localVal = hwp_mac_pcu_reg->mac_sapsd_ssp;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_sapsd_ssp_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_sapsd_ssp.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sapsd_ssp_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_sapsd_ssp.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_sapsd_si_get(void)
{
    return hwp_mac_pcu_reg->mac_sapsd_si.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_sapsd_si_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_sapsd_si.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sapsd_si_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_sapsd_si.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_sapsd_si_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_SAPSD_SI localVal = hwp_mac_pcu_reg->mac_sapsd_si;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_sapsd_si_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_sapsd_si.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sapsd_si_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_sapsd_si.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_smps_mode_get(void)
{
    return hwp_mac_pcu_reg->mac_smps_mode.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_smps_mode_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_smps_mode.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_smps_mode_pack(uint8_t mode, uint8_t mcs)
{
    hwp_mac_pcu_reg->mac_smps_mode.val = (((uint32_t)mode << 8) | ((uint32_t)mcs << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_smps_mode_unpack(uint8_t* mode, uint8_t* mcs)
{
    T_MAC_PCU_REG_MAC_SMPS_MODE localVal = hwp_mac_pcu_reg->mac_smps_mode;

    *mode = localVal.bit_field.mode;
    *mcs = localVal.bit_field.mcs;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_smps_mode_mode_getf(void)
{
    return hwp_mac_pcu_reg->mac_smps_mode.bit_field.mode;
}

__STATIC_INLINE void mac_pcu_reg_mac_smps_mode_mode_setf(uint8_t mode)
{
    hwp_mac_pcu_reg->mac_smps_mode.bit_field.mode = mode;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_smps_mode_mcs_getf(void)
{
    return hwp_mac_pcu_reg->mac_smps_mode.bit_field.mcs;
}

__STATIC_INLINE void mac_pcu_reg_mac_smps_mode_mcs_setf(uint8_t mcs)
{
    hwp_mac_pcu_reg->mac_smps_mode.bit_field.mcs = mcs;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_listen_interval_timer_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_listen_interval_timer_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_listen_interval_timer_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_listen_interval_timer_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_listen_interval_timer_ctrl_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_listen_interval_timer_ctrl.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_listen_interval_timer_ctrl_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_LISTEN_INTERVAL_TIMER_CTRL localVal = hwp_mac_pcu_reg->mac_listen_interval_timer_ctrl;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_listen_interval_timer_ctrl_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_listen_interval_timer_ctrl.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_listen_interval_timer_ctrl_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_listen_interval_timer_ctrl.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_interrupt_status_get(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_status.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_status_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_interrupt_status.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_status_pack(uint8_t end_of_ctwindow, uint8_t end_of_absent_period, uint8_t start_of_absent_period, uint8_t de_authentication, uint8_t pa_tx_suspend, uint8_t radar_detected, uint8_t hw_tx_abort_request_end, uint8_t hw_tx_abort_request_start, uint8_t hi_priority_q_rx_complete, uint8_t rf_vco_unlock_interrupt, uint8_t cf_end_interrupt, uint8_t cap_end_interrupt, uint8_t error, uint8_t wake_up_interrupt, uint8_t atim_interrupt, uint8_t hcca_txop_interrupt, uint8_t tbtt, uint8_t tx_complete, uint8_t rx_complete)
{
    hwp_mac_pcu_reg->mac_interrupt_status.val = (((uint32_t)end_of_ctwindow << 18) | ((uint32_t)end_of_absent_period << 17) | ((uint32_t)start_of_absent_period << 16) | ((uint32_t)de_authentication << 15) | ((uint32_t)pa_tx_suspend << 14) | ((uint32_t)radar_detected << 13) | ((uint32_t)hw_tx_abort_request_end << 12) | ((uint32_t)hw_tx_abort_request_start << 11) | ((uint32_t)hi_priority_q_rx_complete << 10) | ((uint32_t)rf_vco_unlock_interrupt << 9) | ((uint32_t)cf_end_interrupt << 8) | ((uint32_t)cap_end_interrupt << 7) | ((uint32_t)error << 6) | ((uint32_t)wake_up_interrupt << 5) | ((uint32_t)atim_interrupt << 4) | ((uint32_t)hcca_txop_interrupt << 3) | ((uint32_t)tbtt << 2) | ((uint32_t)tx_complete << 1) | ((uint32_t)rx_complete << 0));
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_interrupt_mask_get(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_pack(uint8_t end_of_ctwindow, uint8_t end_of_absent_period, uint8_t start_of_absent_period, uint8_t de_authentication, uint8_t pa_tx_suspend, uint8_t radar_detected, uint8_t hw_tx_abort_request_end, uint8_t hw_tx_abort_request_start, uint8_t hi_priority_q_rx_complete, uint8_t rf_vco_unlock_interrupt, uint8_t cf_end_interrupt, uint8_t cap_end_interrupt, uint8_t error, uint8_t wake_up_interrupt, uint8_t atim_interrupt, uint8_t hcca_txop_interrupt, uint8_t tbtt, uint8_t tx_complete, uint8_t rx_complete)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.val = (((uint32_t)end_of_ctwindow << 18) | ((uint32_t)end_of_absent_period << 17) | ((uint32_t)start_of_absent_period << 16) | ((uint32_t)de_authentication << 15) | ((uint32_t)pa_tx_suspend << 14) | ((uint32_t)radar_detected << 13) | ((uint32_t)hw_tx_abort_request_end << 12) | ((uint32_t)hw_tx_abort_request_start << 11) | ((uint32_t)hi_priority_q_rx_complete << 10) | ((uint32_t)rf_vco_unlock_interrupt << 9) | ((uint32_t)cf_end_interrupt << 8) | ((uint32_t)cap_end_interrupt << 7) | ((uint32_t)error << 6) | ((uint32_t)wake_up_interrupt << 5) | ((uint32_t)atim_interrupt << 4) | ((uint32_t)hcca_txop_interrupt << 3) | ((uint32_t)tbtt << 2) | ((uint32_t)tx_complete << 1) | ((uint32_t)rx_complete << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_unpack(uint8_t* end_of_ctwindow, uint8_t* end_of_absent_period, uint8_t* start_of_absent_period, uint8_t* de_authentication, uint8_t* pa_tx_suspend, uint8_t* radar_detected, uint8_t* hw_tx_abort_request_end, uint8_t* hw_tx_abort_request_start, uint8_t* hi_priority_q_rx_complete, uint8_t* rf_vco_unlock_interrupt, uint8_t* cf_end_interrupt, uint8_t* cap_end_interrupt, uint8_t* error, uint8_t* wake_up_interrupt, uint8_t* atim_interrupt, uint8_t* hcca_txop_interrupt, uint8_t* tbtt, uint8_t* tx_complete, uint8_t* rx_complete)
{
    T_MAC_PCU_REG_MAC_INTERRUPT_MASK localVal = hwp_mac_pcu_reg->mac_interrupt_mask;

    *end_of_ctwindow = localVal.bit_field.end_of_ctwindow;
    *end_of_absent_period = localVal.bit_field.end_of_absent_period;
    *start_of_absent_period = localVal.bit_field.start_of_absent_period;
    *de_authentication = localVal.bit_field.de_authentication;
    *pa_tx_suspend = localVal.bit_field.pa_tx_suspend;
    *radar_detected = localVal.bit_field.radar_detected;
    *hw_tx_abort_request_end = localVal.bit_field.hw_tx_abort_request_end;
    *hw_tx_abort_request_start = localVal.bit_field.hw_tx_abort_request_start;
    *hi_priority_q_rx_complete = localVal.bit_field.hi_priority_q_rx_complete;
    *rf_vco_unlock_interrupt = localVal.bit_field.rf_vco_unlock_interrupt;
    *cf_end_interrupt = localVal.bit_field.cf_end_interrupt;
    *cap_end_interrupt = localVal.bit_field.cap_end_interrupt;
    *error = localVal.bit_field.error;
    *wake_up_interrupt = localVal.bit_field.wake_up_interrupt;
    *atim_interrupt = localVal.bit_field.atim_interrupt;
    *hcca_txop_interrupt = localVal.bit_field.hcca_txop_interrupt;
    *tbtt = localVal.bit_field.tbtt;
    *tx_complete = localVal.bit_field.tx_complete;
    *rx_complete = localVal.bit_field.rx_complete;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_end_of_ctwindow_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.end_of_ctwindow;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_end_of_ctwindow_setf(uint8_t end_of_ctwindow)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.end_of_ctwindow = end_of_ctwindow;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_end_of_absent_period_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.end_of_absent_period;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_end_of_absent_period_setf(uint8_t end_of_absent_period)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.end_of_absent_period = end_of_absent_period;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_start_of_absent_period_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.start_of_absent_period;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_start_of_absent_period_setf(uint8_t start_of_absent_period)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.start_of_absent_period = start_of_absent_period;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_de_authentication_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.de_authentication;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_de_authentication_setf(uint8_t de_authentication)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.de_authentication = de_authentication;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_pa_tx_suspend_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.pa_tx_suspend;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_pa_tx_suspend_setf(uint8_t pa_tx_suspend)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.pa_tx_suspend = pa_tx_suspend;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_radar_detected_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.radar_detected;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_radar_detected_setf(uint8_t radar_detected)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.radar_detected = radar_detected;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_hw_tx_abort_request_end_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.hw_tx_abort_request_end;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_hw_tx_abort_request_end_setf(uint8_t hw_tx_abort_request_end)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.hw_tx_abort_request_end = hw_tx_abort_request_end;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_hw_tx_abort_request_start_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.hw_tx_abort_request_start;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_hw_tx_abort_request_start_setf(uint8_t hw_tx_abort_request_start)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.hw_tx_abort_request_start = hw_tx_abort_request_start;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_hi_priority_q_rx_complete_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.hi_priority_q_rx_complete;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_hi_priority_q_rx_complete_setf(uint8_t hi_priority_q_rx_complete)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.hi_priority_q_rx_complete = hi_priority_q_rx_complete;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_rf_vco_unlock_interrupt_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.rf_vco_unlock_interrupt;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_rf_vco_unlock_interrupt_setf(uint8_t rf_vco_unlock_interrupt)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.rf_vco_unlock_interrupt = rf_vco_unlock_interrupt;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_cf_end_interrupt_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.cf_end_interrupt;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_cf_end_interrupt_setf(uint8_t cf_end_interrupt)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.cf_end_interrupt = cf_end_interrupt;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_cap_end_interrupt_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.cap_end_interrupt;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_cap_end_interrupt_setf(uint8_t cap_end_interrupt)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.cap_end_interrupt = cap_end_interrupt;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_error_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.error;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_error_setf(uint8_t error)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.error = error;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_wake_up_interrupt_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.wake_up_interrupt;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_wake_up_interrupt_setf(uint8_t wake_up_interrupt)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.wake_up_interrupt = wake_up_interrupt;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_atim_interrupt_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.atim_interrupt;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_atim_interrupt_setf(uint8_t atim_interrupt)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.atim_interrupt = atim_interrupt;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_hcca_txop_interrupt_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.hcca_txop_interrupt;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_hcca_txop_interrupt_setf(uint8_t hcca_txop_interrupt)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.hcca_txop_interrupt = hcca_txop_interrupt;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_tbtt_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.tbtt;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_tbtt_setf(uint8_t tbtt)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.tbtt = tbtt;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_tx_complete_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.tx_complete;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_tx_complete_setf(uint8_t tx_complete)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.tx_complete = tx_complete;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_interrupt_mask_rx_complete_getf(void)
{
    return hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.rx_complete;
}

__STATIC_INLINE void mac_pcu_reg_mac_interrupt_mask_rx_complete_setf(uint8_t rx_complete)
{
    hwp_mac_pcu_reg->mac_interrupt_mask.bit_field.rx_complete = rx_complete;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_frame_pointer_get(void)
{
    return hwp_mac_pcu_reg->mac_tx_frame_pointer.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_frame_pointer_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_TX_FRAME_POINTER localVal = hwp_mac_pcu_reg->mac_tx_frame_pointer;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_frame_pointer_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_frame_pointer.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_frame_pointer_get(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_pointer.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_frame_pointer_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_RX_FRAME_POINTER localVal = hwp_mac_pcu_reg->mac_rx_frame_pointer;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_frame_pointer_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_frame_pointer.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_error_code_get(void)
{
    return hwp_mac_pcu_reg->mac_error_code.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_code_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_ERROR_CODE localVal = hwp_mac_pcu_reg->mac_error_code;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_code_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_code.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_mpdu_cnt_get(void)
{
    return hwp_mac_pcu_reg->mac_tx_mpdu_cnt.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_mpdu_cnt_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_TX_MPDU_CNT localVal = hwp_mac_pcu_reg->mac_tx_mpdu_cnt;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tx_mpdu_cnt_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_mpdu_cnt.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_mpdu_cnt_get(void)
{
    return hwp_mac_pcu_reg->mac_rx_mpdu_cnt.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_mpdu_cnt_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_RX_MPDU_CNT localVal = hwp_mac_pcu_reg->mac_rx_mpdu_cnt;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_mpdu_cnt_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_mpdu_cnt.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hi_priority_rx_frame_pointer_get(void)
{
    return hwp_mac_pcu_reg->mac_hi_priority_rx_frame_pointer.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_hi_priority_rx_frame_pointer_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_HI_PRIORITY_RX_FRAME_POINTER localVal = hwp_mac_pcu_reg->mac_hi_priority_rx_frame_pointer;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_hi_priority_rx_frame_pointer_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_hi_priority_rx_frame_pointer.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_de_authen_reason_code_get(void)
{
    return hwp_mac_pcu_reg->mac_de_authen_reason_code.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_de_authen_reason_code_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_DE_AUTHEN_REASON_CODE localVal = hwp_mac_pcu_reg->mac_de_authen_reason_code;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_de_authen_reason_code_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_de_authen_reason_code.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_error_interrupt_status_get(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_status.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_status_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_error_interrupt_status.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_status_pack(uint8_t ec_19, uint8_t ec_18, uint8_t ec_17, uint8_t ec_16, uint8_t ec_15, uint8_t ec_14, uint8_t ec_13, uint8_t ec_12, uint8_t ec_11, uint8_t ec_10, uint8_t ec_9, uint8_t ec_8, uint8_t ec_7, uint8_t ec_6, uint8_t ec_5, uint8_t ec_4, uint8_t ec_3, uint8_t ec_2, uint8_t ec_1)
{
    hwp_mac_pcu_reg->mac_error_interrupt_status.val = (((uint32_t)ec_19 << 19) | ((uint32_t)ec_18 << 18) | ((uint32_t)ec_17 << 17) | ((uint32_t)ec_16 << 16) | ((uint32_t)ec_15 << 15) | ((uint32_t)ec_14 << 14) | ((uint32_t)ec_13 << 13) | ((uint32_t)ec_12 << 12) | ((uint32_t)ec_11 << 11) | ((uint32_t)ec_10 << 10) | ((uint32_t)ec_9 << 9) | ((uint32_t)ec_8 << 8) | ((uint32_t)ec_7 << 7) | ((uint32_t)ec_6 << 6) | ((uint32_t)ec_5 << 5) | ((uint32_t)ec_4 << 4) | ((uint32_t)ec_3 << 3) | ((uint32_t)ec_2 << 2) | ((uint32_t)ec_1 << 1));
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_error_interrupt_mask_get(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_pack(uint8_t ec_19, uint8_t ec_18, uint8_t ec_17, uint8_t ec_16, uint8_t ec_15, uint8_t ec_14, uint8_t ec_13, uint8_t ec_12, uint8_t ec_11, uint8_t ec_10, uint8_t ec_9, uint8_t ec_8, uint8_t ec_7, uint8_t ec_6, uint8_t ec_5, uint8_t ec_4, uint8_t ec_3, uint8_t ec_2, uint8_t ec_1)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.val = (((uint32_t)ec_19 << 19) | ((uint32_t)ec_18 << 18) | ((uint32_t)ec_17 << 17) | ((uint32_t)ec_16 << 16) | ((uint32_t)ec_15 << 15) | ((uint32_t)ec_14 << 14) | ((uint32_t)ec_13 << 13) | ((uint32_t)ec_12 << 12) | ((uint32_t)ec_11 << 11) | ((uint32_t)ec_10 << 10) | ((uint32_t)ec_9 << 9) | ((uint32_t)ec_8 << 8) | ((uint32_t)ec_7 << 7) | ((uint32_t)ec_6 << 6) | ((uint32_t)ec_5 << 5) | ((uint32_t)ec_4 << 4) | ((uint32_t)ec_3 << 3) | ((uint32_t)ec_2 << 2) | ((uint32_t)ec_1 << 1));
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_unpack(uint8_t* ec_19, uint8_t* ec_18, uint8_t* ec_17, uint8_t* ec_16, uint8_t* ec_15, uint8_t* ec_14, uint8_t* ec_13, uint8_t* ec_12, uint8_t* ec_11, uint8_t* ec_10, uint8_t* ec_9, uint8_t* ec_8, uint8_t* ec_7, uint8_t* ec_6, uint8_t* ec_5, uint8_t* ec_4, uint8_t* ec_3, uint8_t* ec_2, uint8_t* ec_1)
{
    T_MAC_PCU_REG_MAC_ERROR_INTERRUPT_MASK localVal = hwp_mac_pcu_reg->mac_error_interrupt_mask;

    *ec_19 = localVal.bit_field.ec_19;
    *ec_18 = localVal.bit_field.ec_18;
    *ec_17 = localVal.bit_field.ec_17;
    *ec_16 = localVal.bit_field.ec_16;
    *ec_15 = localVal.bit_field.ec_15;
    *ec_14 = localVal.bit_field.ec_14;
    *ec_13 = localVal.bit_field.ec_13;
    *ec_12 = localVal.bit_field.ec_12;
    *ec_11 = localVal.bit_field.ec_11;
    *ec_10 = localVal.bit_field.ec_10;
    *ec_9 = localVal.bit_field.ec_9;
    *ec_8 = localVal.bit_field.ec_8;
    *ec_7 = localVal.bit_field.ec_7;
    *ec_6 = localVal.bit_field.ec_6;
    *ec_5 = localVal.bit_field.ec_5;
    *ec_4 = localVal.bit_field.ec_4;
    *ec_3 = localVal.bit_field.ec_3;
    *ec_2 = localVal.bit_field.ec_2;
    *ec_1 = localVal.bit_field.ec_1;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_19_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_19;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_19_setf(uint8_t ec_19)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_19 = ec_19;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_18_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_18;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_18_setf(uint8_t ec_18)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_18 = ec_18;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_17_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_17;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_17_setf(uint8_t ec_17)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_17 = ec_17;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_16_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_16;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_16_setf(uint8_t ec_16)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_16 = ec_16;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_15_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_15;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_15_setf(uint8_t ec_15)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_15 = ec_15;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_14_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_14;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_14_setf(uint8_t ec_14)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_14 = ec_14;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_13_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_13;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_13_setf(uint8_t ec_13)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_13 = ec_13;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_12_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_12;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_12_setf(uint8_t ec_12)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_12 = ec_12;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_11_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_11;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_11_setf(uint8_t ec_11)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_11 = ec_11;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_10_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_10;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_10_setf(uint8_t ec_10)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_10 = ec_10;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_9_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_9;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_9_setf(uint8_t ec_9)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_9 = ec_9;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_8_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_8;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_8_setf(uint8_t ec_8)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_8 = ec_8;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_7_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_7;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_7_setf(uint8_t ec_7)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_7 = ec_7;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_6_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_6;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_6_setf(uint8_t ec_6)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_6 = ec_6;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_5_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_5;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_5_setf(uint8_t ec_5)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_5 = ec_5;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_4_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_4;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_4_setf(uint8_t ec_4)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_4 = ec_4;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_3_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_3;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_3_setf(uint8_t ec_3)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_3 = ec_3;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_2_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_2;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_2_setf(uint8_t ec_2)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_2 = ec_2;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_error_interrupt_mask_ec_1_getf(void)
{
    return hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_1;
}

__STATIC_INLINE void mac_pcu_reg_mac_error_interrupt_mask_ec_1_setf(uint8_t ec_1)
{
    hwp_mac_pcu_reg->mac_error_interrupt_mask.bit_field.ec_1 = ec_1;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_phy_spi_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_phy_spi_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_spi_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_phy_spi_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_spi_ctrl_pack(uint8_t phy_register_addr, uint8_t phy_register_read_write, uint8_t phy_register_data)
{
    hwp_mac_pcu_reg->mac_phy_spi_ctrl.val = (((uint32_t)phy_register_addr << 9) | ((uint32_t)phy_register_read_write << 8) | ((uint32_t)phy_register_data << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_spi_ctrl_unpack(uint8_t* phy_register_addr, uint8_t* phy_register_read_write, uint8_t* phy_register_data)
{
    T_MAC_PCU_REG_MAC_PHY_SPI_CTRL localVal = hwp_mac_pcu_reg->mac_phy_spi_ctrl;

    *phy_register_addr = localVal.bit_field.phy_register_addr;
    *phy_register_read_write = localVal.bit_field.phy_register_read_write;
    *phy_register_data = localVal.bit_field.phy_register_data;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_spi_ctrl_phy_register_addr_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_spi_ctrl.bit_field.phy_register_addr;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_spi_ctrl_phy_register_addr_setf(uint8_t phy_register_addr)
{
    hwp_mac_pcu_reg->mac_phy_spi_ctrl.bit_field.phy_register_addr = phy_register_addr;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_spi_ctrl_phy_register_read_write_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_spi_ctrl.bit_field.phy_register_read_write;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_spi_ctrl_phy_register_read_write_setf(uint8_t phy_register_read_write)
{
    hwp_mac_pcu_reg->mac_phy_spi_ctrl.bit_field.phy_register_read_write = phy_register_read_write;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_spi_ctrl_phy_register_data_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_spi_ctrl.bit_field.phy_register_data;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_spi_ctrl_phy_register_data_setf(uint8_t phy_register_data)
{
    hwp_mac_pcu_reg->mac_phy_spi_ctrl.bit_field.phy_register_data = phy_register_data;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_phy_spi_lock_get(void)
{
    return hwp_mac_pcu_reg->mac_phy_spi_lock.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_spi_lock_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_phy_spi_lock.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_spi_lock_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_phy_spi_lock.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_spi_lock_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_PHY_SPI_LOCK localVal = hwp_mac_pcu_reg->mac_phy_spi_lock;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_spi_lock_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_spi_lock.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_spi_lock_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_phy_spi_lock.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_phy_spi_status_get(void)
{
    return hwp_mac_pcu_reg->mac_phy_spi_status.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_spi_status_unpack(uint8_t* spi_busy)
{
    T_MAC_PCU_REG_MAC_PHY_SPI_STATUS localVal = hwp_mac_pcu_reg->mac_phy_spi_status;

    *spi_busy = localVal.bit_field.spi_busy;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_spi_status_spi_busy_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_spi_status.bit_field.spi_busy;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rf_vco_spi_ctrl_width_get(void)
{
    return hwp_mac_pcu_reg->mac_rf_vco_spi_ctrl_width.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rf_vco_spi_ctrl_width_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rf_vco_spi_ctrl_width.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rf_vco_spi_ctrl_width_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_rf_vco_spi_ctrl_width.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_rf_vco_spi_ctrl_width_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_RF_VCO_SPI_CTRL_WIDTH localVal = hwp_mac_pcu_reg->mac_rf_vco_spi_ctrl_width;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rf_vco_spi_ctrl_width_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_rf_vco_spi_ctrl_width.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rf_vco_spi_ctrl_width_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_rf_vco_spi_ctrl_width.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rf_vco_spi_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_rf_vco_spi_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rf_vco_spi_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rf_vco_spi_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rf_vco_spi_ctrl_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rf_vco_spi_ctrl.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_rf_vco_spi_ctrl_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_RF_VCO_SPI_CTRL localVal = hwp_mac_pcu_reg->mac_rf_vco_spi_ctrl;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rf_vco_spi_ctrl_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_rf_vco_spi_ctrl.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rf_vco_spi_ctrl_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rf_vco_spi_ctrl.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rf_vco_spi_status_get(void)
{
    return hwp_mac_pcu_reg->mac_rf_vco_spi_status.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rf_vco_spi_status_unpack(uint8_t* rf_spi_busy)
{
    T_MAC_PCU_REG_MAC_RF_VCO_SPI_STATUS localVal = hwp_mac_pcu_reg->mac_rf_vco_spi_status;

    *rf_spi_busy = localVal.bit_field.rf_spi_busy;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rf_vco_spi_status_rf_spi_busy_getf(void)
{
    return hwp_mac_pcu_reg->mac_rf_vco_spi_status.bit_field.rf_spi_busy;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_phy_11a_reg_base_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_phy_11a_reg_base_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_11a_reg_base_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_phy_11a_reg_base_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_11a_reg_base_addr_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_phy_11a_reg_base_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_11a_reg_base_addr_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_PHY_11A_REG_BASE_ADDR localVal = hwp_mac_pcu_reg->mac_phy_11a_reg_base_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_11a_reg_base_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_11a_reg_base_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_11a_reg_base_addr_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_phy_11a_reg_base_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_phy_11b_reg_base_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_phy_11b_reg_base_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_11b_reg_base_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_phy_11b_reg_base_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_11b_reg_base_addr_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_phy_11b_reg_base_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_11b_reg_base_addr_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_PHY_11B_REG_BASE_ADDR localVal = hwp_mac_pcu_reg->mac_phy_11b_reg_base_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_11b_reg_base_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_11b_reg_base_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_11b_reg_base_addr_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_phy_11b_reg_base_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_phy_11g_reg_base_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_phy_11g_reg_base_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_11g_reg_base_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_phy_11g_reg_base_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_11g_reg_base_addr_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_phy_11g_reg_base_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_11g_reg_base_addr_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_PHY_11G_REG_BASE_ADDR localVal = hwp_mac_pcu_reg->mac_phy_11g_reg_base_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_11g_reg_base_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_11g_reg_base_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_11g_reg_base_addr_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_phy_11g_reg_base_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_phy_rf_reg_base_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_phy_rf_reg_base_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_rf_reg_base_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_phy_rf_reg_base_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_rf_reg_base_addr_pack(uint8_t value)
{
    hwp_mac_pcu_reg->mac_phy_rf_reg_base_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_rf_reg_base_addr_unpack(uint8_t* value)
{
    T_MAC_PCU_REG_MAC_PHY_RF_REG_BASE_ADDR localVal = hwp_mac_pcu_reg->mac_phy_rf_reg_base_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_rf_reg_base_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_rf_reg_base_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_rf_reg_base_addr_value_setf(uint8_t value)
{
    hwp_mac_pcu_reg->mac_phy_rf_reg_base_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_plcp_delay_get(void)
{
    return hwp_mac_pcu_reg->mac_tx_plcp_delay.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_plcp_delay_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tx_plcp_delay.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_plcp_delay_pack(uint8_t ht_tx_plcp_delay, uint8_t ofdm_tx_plcp_delay, uint8_t dsss_cck_tx_plcp_delay)
{
    hwp_mac_pcu_reg->mac_tx_plcp_delay.val = (((uint32_t)ht_tx_plcp_delay << 8) | ((uint32_t)ofdm_tx_plcp_delay << 4) | ((uint32_t)dsss_cck_tx_plcp_delay << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_plcp_delay_unpack(uint8_t* ht_tx_plcp_delay, uint8_t* ofdm_tx_plcp_delay, uint8_t* dsss_cck_tx_plcp_delay)
{
    T_MAC_PCU_REG_MAC_TX_PLCP_DELAY localVal = hwp_mac_pcu_reg->mac_tx_plcp_delay;

    *ht_tx_plcp_delay = localVal.bit_field.ht_tx_plcp_delay;
    *ofdm_tx_plcp_delay = localVal.bit_field.ofdm_tx_plcp_delay;
    *dsss_cck_tx_plcp_delay = localVal.bit_field.dsss_cck_tx_plcp_delay;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tx_plcp_delay_ht_tx_plcp_delay_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_plcp_delay.bit_field.ht_tx_plcp_delay;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_plcp_delay_ht_tx_plcp_delay_setf(uint8_t ht_tx_plcp_delay)
{
    hwp_mac_pcu_reg->mac_tx_plcp_delay.bit_field.ht_tx_plcp_delay = ht_tx_plcp_delay;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tx_plcp_delay_ofdm_tx_plcp_delay_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_plcp_delay.bit_field.ofdm_tx_plcp_delay;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_plcp_delay_ofdm_tx_plcp_delay_setf(uint8_t ofdm_tx_plcp_delay)
{
    hwp_mac_pcu_reg->mac_tx_plcp_delay.bit_field.ofdm_tx_plcp_delay = ofdm_tx_plcp_delay;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tx_plcp_delay_dsss_cck_tx_plcp_delay_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_plcp_delay.bit_field.dsss_cck_tx_plcp_delay;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_plcp_delay_dsss_cck_tx_plcp_delay_setf(uint8_t dsss_cck_tx_plcp_delay)
{
    hwp_mac_pcu_reg->mac_tx_plcp_delay.bit_field.dsss_cck_tx_plcp_delay = dsss_cck_tx_plcp_delay;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_plcp_delay_get(void)
{
    return hwp_mac_pcu_reg->mac_rx_plcp_delay.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_plcp_delay_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_plcp_delay.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_plcp_delay_pack(uint8_t ht_tx_plcp_delay, uint8_t ofdm_tx_plcp_delay, uint8_t dsss_cck_tx_plcp_delay)
{
    hwp_mac_pcu_reg->mac_rx_plcp_delay.val = (((uint32_t)ht_tx_plcp_delay << 8) | ((uint32_t)ofdm_tx_plcp_delay << 4) | ((uint32_t)dsss_cck_tx_plcp_delay << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_plcp_delay_unpack(uint8_t* ht_tx_plcp_delay, uint8_t* ofdm_tx_plcp_delay, uint8_t* dsss_cck_tx_plcp_delay)
{
    T_MAC_PCU_REG_MAC_RX_PLCP_DELAY localVal = hwp_mac_pcu_reg->mac_rx_plcp_delay;

    *ht_tx_plcp_delay = localVal.bit_field.ht_tx_plcp_delay;
    *ofdm_tx_plcp_delay = localVal.bit_field.ofdm_tx_plcp_delay;
    *dsss_cck_tx_plcp_delay = localVal.bit_field.dsss_cck_tx_plcp_delay;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_plcp_delay_ht_tx_plcp_delay_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_plcp_delay.bit_field.ht_tx_plcp_delay;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_plcp_delay_ht_tx_plcp_delay_setf(uint8_t ht_tx_plcp_delay)
{
    hwp_mac_pcu_reg->mac_rx_plcp_delay.bit_field.ht_tx_plcp_delay = ht_tx_plcp_delay;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_plcp_delay_ofdm_tx_plcp_delay_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_plcp_delay.bit_field.ofdm_tx_plcp_delay;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_plcp_delay_ofdm_tx_plcp_delay_setf(uint8_t ofdm_tx_plcp_delay)
{
    hwp_mac_pcu_reg->mac_rx_plcp_delay.bit_field.ofdm_tx_plcp_delay = ofdm_tx_plcp_delay;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_plcp_delay_dsss_cck_tx_plcp_delay_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_plcp_delay.bit_field.dsss_cck_tx_plcp_delay;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_plcp_delay_dsss_cck_tx_plcp_delay_setf(uint8_t dsss_cck_tx_plcp_delay)
{
    hwp_mac_pcu_reg->mac_rx_plcp_delay.bit_field.dsss_cck_tx_plcp_delay = dsss_cck_tx_plcp_delay;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rxtx_turnaround_time_get(void)
{
    return hwp_mac_pcu_reg->mac_rxtx_turnaround_time.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rxtx_turnaround_time_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rxtx_turnaround_time.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rxtx_turnaround_time_pack(uint8_t tx_to_rx, uint8_t rx_to_tx)
{
    hwp_mac_pcu_reg->mac_rxtx_turnaround_time.val = (((uint32_t)tx_to_rx << 8) | ((uint32_t)rx_to_tx << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_rxtx_turnaround_time_unpack(uint8_t* tx_to_rx, uint8_t* rx_to_tx)
{
    T_MAC_PCU_REG_MAC_RXTX_TURNAROUND_TIME localVal = hwp_mac_pcu_reg->mac_rxtx_turnaround_time;

    *tx_to_rx = localVal.bit_field.tx_to_rx;
    *rx_to_tx = localVal.bit_field.rx_to_tx;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rxtx_turnaround_time_tx_to_rx_getf(void)
{
    return hwp_mac_pcu_reg->mac_rxtx_turnaround_time.bit_field.tx_to_rx;
}

__STATIC_INLINE void mac_pcu_reg_mac_rxtx_turnaround_time_tx_to_rx_setf(uint8_t tx_to_rx)
{
    hwp_mac_pcu_reg->mac_rxtx_turnaround_time.bit_field.tx_to_rx = tx_to_rx;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rxtx_turnaround_time_rx_to_tx_getf(void)
{
    return hwp_mac_pcu_reg->mac_rxtx_turnaround_time.bit_field.rx_to_tx;
}

__STATIC_INLINE void mac_pcu_reg_mac_rxtx_turnaround_time_rx_to_tx_setf(uint8_t rx_to_tx)
{
    hwp_mac_pcu_reg->mac_rxtx_turnaround_time.bit_field.rx_to_tx = rx_to_tx;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_phy_timeout_adjust_get(void)
{
    return hwp_mac_pcu_reg->mac_phy_timeout_adjust.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_timeout_adjust_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_phy_timeout_adjust.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_timeout_adjust_pack(uint8_t watchdog_timeout_value, uint8_t watchdog_timeout_en)
{
    hwp_mac_pcu_reg->mac_phy_timeout_adjust.val = (((uint32_t)watchdog_timeout_value << 9) | ((uint32_t)watchdog_timeout_en << 8));
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_timeout_adjust_unpack(uint8_t* watchdog_timeout_value, uint8_t* watchdog_timeout_en)
{
    T_MAC_PCU_REG_MAC_PHY_TIMEOUT_ADJUST localVal = hwp_mac_pcu_reg->mac_phy_timeout_adjust;

    *watchdog_timeout_value = localVal.bit_field.watchdog_timeout_value;
    *watchdog_timeout_en = localVal.bit_field.watchdog_timeout_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_timeout_adjust_watchdog_timeout_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_timeout_adjust.bit_field.watchdog_timeout_value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_timeout_adjust_watchdog_timeout_value_setf(uint8_t watchdog_timeout_value)
{
    hwp_mac_pcu_reg->mac_phy_timeout_adjust.bit_field.watchdog_timeout_value = watchdog_timeout_value;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_timeout_adjust_watchdog_timeout_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_timeout_adjust.bit_field.watchdog_timeout_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_timeout_adjust_watchdog_timeout_en_setf(uint8_t watchdog_timeout_en)
{
    hwp_mac_pcu_reg->mac_phy_timeout_adjust.bit_field.watchdog_timeout_en = watchdog_timeout_en;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_phy_reg_access_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_phy_reg_access_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_reg_access_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_phy_reg_access_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_reg_access_ctrl_pack(uint8_t phy_reg_addr, uint8_t read_write, uint8_t phy_reg_access_flag)
{
    hwp_mac_pcu_reg->mac_phy_reg_access_ctrl.val = (((uint32_t)phy_reg_addr << 2) | ((uint32_t)read_write << 1) | ((uint32_t)phy_reg_access_flag << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_reg_access_ctrl_unpack(uint8_t* phy_reg_addr, uint8_t* read_write, uint8_t* phy_reg_access_flag)
{
    T_MAC_PCU_REG_MAC_PHY_REG_ACCESS_CTRL localVal = hwp_mac_pcu_reg->mac_phy_reg_access_ctrl;

    *phy_reg_addr = localVal.bit_field.phy_reg_addr;
    *read_write = localVal.bit_field.read_write;
    *phy_reg_access_flag = localVal.bit_field.phy_reg_access_flag;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_reg_access_ctrl_phy_reg_addr_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_reg_access_ctrl.bit_field.phy_reg_addr;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_reg_access_ctrl_phy_reg_addr_setf(uint8_t phy_reg_addr)
{
    hwp_mac_pcu_reg->mac_phy_reg_access_ctrl.bit_field.phy_reg_addr = phy_reg_addr;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_reg_access_ctrl_read_write_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_reg_access_ctrl.bit_field.read_write;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_reg_access_ctrl_read_write_setf(uint8_t read_write)
{
    hwp_mac_pcu_reg->mac_phy_reg_access_ctrl.bit_field.read_write = read_write;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_reg_access_ctrl_phy_reg_access_flag_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_reg_access_ctrl.bit_field.phy_reg_access_flag;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_reg_access_ctrl_phy_reg_access_flag_setf(uint8_t phy_reg_access_flag)
{
    hwp_mac_pcu_reg->mac_phy_reg_access_ctrl.bit_field.phy_reg_access_flag = phy_reg_access_flag;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_phy_reg_access_data_get(void)
{
    return hwp_mac_pcu_reg->mac_phy_reg_access_data.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_reg_access_data_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_phy_reg_access_data.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_reg_access_data_pack(uint32_t read_write_data)
{
    hwp_mac_pcu_reg->mac_phy_reg_access_data.val = (((uint32_t)read_write_data << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_reg_access_data_unpack(uint32_t* read_write_data)
{
    T_MAC_PCU_REG_MAC_PHY_REG_ACCESS_DATA localVal = hwp_mac_pcu_reg->mac_phy_reg_access_data;

    *read_write_data = localVal.bit_field.read_write_data;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_phy_reg_access_data_read_write_data_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_reg_access_data.bit_field.read_write_data;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_reg_access_data_read_write_data_setf(uint32_t read_write_data)
{
    hwp_mac_pcu_reg->mac_phy_reg_access_data.bit_field.read_write_data = read_write_data;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_phy_tx_service_get(void)
{
    return hwp_mac_pcu_reg->mac_phy_tx_service.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_tx_service_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_phy_tx_service.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_tx_service_pack(uint16_t dsss_service_field, uint16_t ofdm_service_field)
{
    hwp_mac_pcu_reg->mac_phy_tx_service.val = (((uint32_t)dsss_service_field << 16) | ((uint32_t)ofdm_service_field << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_tx_service_unpack(uint16_t* dsss_service_field, uint16_t* ofdm_service_field)
{
    T_MAC_PCU_REG_MAC_PHY_TX_SERVICE localVal = hwp_mac_pcu_reg->mac_phy_tx_service;

    *dsss_service_field = localVal.bit_field.dsss_service_field;
    *ofdm_service_field = localVal.bit_field.ofdm_service_field;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_phy_tx_service_dsss_service_field_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_tx_service.bit_field.dsss_service_field;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_tx_service_dsss_service_field_setf(uint16_t dsss_service_field)
{
    hwp_mac_pcu_reg->mac_phy_tx_service.bit_field.dsss_service_field = dsss_service_field;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_phy_tx_service_ofdm_service_field_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_tx_service.bit_field.ofdm_service_field;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_tx_service_ofdm_service_field_setf(uint16_t ofdm_service_field)
{
    hwp_mac_pcu_reg->mac_phy_tx_service.bit_field.ofdm_service_field = ofdm_service_field;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_power_level_get(void)
{
    return hwp_mac_pcu_reg->mac_tx_power_level.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_power_level_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tx_power_level.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_power_level_pack(uint8_t ht_tx_power_level, uint8_t ofdm_tx_power_level, uint8_t dsss_tx_power_level)
{
    hwp_mac_pcu_reg->mac_tx_power_level.val = (((uint32_t)ht_tx_power_level << 16) | ((uint32_t)ofdm_tx_power_level << 8) | ((uint32_t)dsss_tx_power_level << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_power_level_unpack(uint8_t* ht_tx_power_level, uint8_t* ofdm_tx_power_level, uint8_t* dsss_tx_power_level)
{
    T_MAC_PCU_REG_MAC_TX_POWER_LEVEL localVal = hwp_mac_pcu_reg->mac_tx_power_level;

    *ht_tx_power_level = localVal.bit_field.ht_tx_power_level;
    *ofdm_tx_power_level = localVal.bit_field.ofdm_tx_power_level;
    *dsss_tx_power_level = localVal.bit_field.dsss_tx_power_level;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tx_power_level_ht_tx_power_level_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_power_level.bit_field.ht_tx_power_level;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_power_level_ht_tx_power_level_setf(uint8_t ht_tx_power_level)
{
    hwp_mac_pcu_reg->mac_tx_power_level.bit_field.ht_tx_power_level = ht_tx_power_level;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tx_power_level_ofdm_tx_power_level_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_power_level.bit_field.ofdm_tx_power_level;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_power_level_ofdm_tx_power_level_setf(uint8_t ofdm_tx_power_level)
{
    hwp_mac_pcu_reg->mac_tx_power_level.bit_field.ofdm_tx_power_level = ofdm_tx_power_level;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tx_power_level_dsss_tx_power_level_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_power_level.bit_field.dsss_tx_power_level;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_power_level_dsss_tx_power_level_setf(uint8_t dsss_tx_power_level)
{
    hwp_mac_pcu_reg->mac_tx_power_level.bit_field.dsss_tx_power_level = dsss_tx_power_level;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_phy_cca_delay_get(void)
{
    return hwp_mac_pcu_reg->mac_phy_cca_delay.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_cca_delay_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_phy_cca_delay.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_cca_delay_pack(uint8_t delay_in_sec_channel, uint8_t delay_in_pri_channel)
{
    hwp_mac_pcu_reg->mac_phy_cca_delay.val = (((uint32_t)delay_in_sec_channel << 8) | ((uint32_t)delay_in_pri_channel << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_cca_delay_unpack(uint8_t* delay_in_sec_channel, uint8_t* delay_in_pri_channel)
{
    T_MAC_PCU_REG_MAC_PHY_CCA_DELAY localVal = hwp_mac_pcu_reg->mac_phy_cca_delay;

    *delay_in_sec_channel = localVal.bit_field.delay_in_sec_channel;
    *delay_in_pri_channel = localVal.bit_field.delay_in_pri_channel;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_cca_delay_delay_in_sec_channel_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_cca_delay.bit_field.delay_in_sec_channel;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_cca_delay_delay_in_sec_channel_setf(uint8_t delay_in_sec_channel)
{
    hwp_mac_pcu_reg->mac_phy_cca_delay.bit_field.delay_in_sec_channel = delay_in_sec_channel;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_phy_cca_delay_delay_in_pri_channel_getf(void)
{
    return hwp_mac_pcu_reg->mac_phy_cca_delay.bit_field.delay_in_pri_channel;
}

__STATIC_INLINE void mac_pcu_reg_mac_phy_cca_delay_delay_in_pri_channel_setf(uint8_t delay_in_pri_channel)
{
    hwp_mac_pcu_reg->mac_phy_cca_delay.bit_field.delay_in_pri_channel = delay_in_pri_channel;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_tx_plcp_adjust_get(void)
{
    return hwp_mac_pcu_reg->mac_tx_plcp_adjust.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_plcp_adjust_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_tx_plcp_adjust.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_plcp_adjust_pack(uint8_t ht_ofdm_txplcp2_adjust, uint8_t ht_ofdm_txplcp1_adjust, uint8_t non_ht_ofdm_txplcp_adjust, uint8_t dsss_cck_tx_plcp_adjust)
{
    hwp_mac_pcu_reg->mac_tx_plcp_adjust.val = (((uint32_t)ht_ofdm_txplcp2_adjust << 24) | ((uint32_t)ht_ofdm_txplcp1_adjust << 16) | ((uint32_t)non_ht_ofdm_txplcp_adjust << 8) | ((uint32_t)dsss_cck_tx_plcp_adjust << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_plcp_adjust_unpack(uint8_t* ht_ofdm_txplcp2_adjust, uint8_t* ht_ofdm_txplcp1_adjust, uint8_t* non_ht_ofdm_txplcp_adjust, uint8_t* dsss_cck_tx_plcp_adjust)
{
    T_MAC_PCU_REG_MAC_TX_PLCP_ADJUST localVal = hwp_mac_pcu_reg->mac_tx_plcp_adjust;

    *ht_ofdm_txplcp2_adjust = localVal.bit_field.ht_ofdm_txplcp2_adjust;
    *ht_ofdm_txplcp1_adjust = localVal.bit_field.ht_ofdm_txplcp1_adjust;
    *non_ht_ofdm_txplcp_adjust = localVal.bit_field.non_ht_ofdm_txplcp_adjust;
    *dsss_cck_tx_plcp_adjust = localVal.bit_field.dsss_cck_tx_plcp_adjust;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tx_plcp_adjust_ht_ofdm_txplcp2_adjust_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_plcp_adjust.bit_field.ht_ofdm_txplcp2_adjust;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_plcp_adjust_ht_ofdm_txplcp2_adjust_setf(uint8_t ht_ofdm_txplcp2_adjust)
{
    hwp_mac_pcu_reg->mac_tx_plcp_adjust.bit_field.ht_ofdm_txplcp2_adjust = ht_ofdm_txplcp2_adjust;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tx_plcp_adjust_ht_ofdm_txplcp1_adjust_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_plcp_adjust.bit_field.ht_ofdm_txplcp1_adjust;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_plcp_adjust_ht_ofdm_txplcp1_adjust_setf(uint8_t ht_ofdm_txplcp1_adjust)
{
    hwp_mac_pcu_reg->mac_tx_plcp_adjust.bit_field.ht_ofdm_txplcp1_adjust = ht_ofdm_txplcp1_adjust;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tx_plcp_adjust_non_ht_ofdm_txplcp_adjust_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_plcp_adjust.bit_field.non_ht_ofdm_txplcp_adjust;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_plcp_adjust_non_ht_ofdm_txplcp_adjust_setf(uint8_t non_ht_ofdm_txplcp_adjust)
{
    hwp_mac_pcu_reg->mac_tx_plcp_adjust.bit_field.non_ht_ofdm_txplcp_adjust = non_ht_ofdm_txplcp_adjust;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_tx_plcp_adjust_dsss_cck_tx_plcp_adjust_getf(void)
{
    return hwp_mac_pcu_reg->mac_tx_plcp_adjust.bit_field.dsss_cck_tx_plcp_adjust;
}

__STATIC_INLINE void mac_pcu_reg_mac_tx_plcp_adjust_dsss_cck_tx_plcp_adjust_setf(uint8_t dsss_cck_tx_plcp_adjust)
{
    hwp_mac_pcu_reg->mac_tx_plcp_adjust.bit_field.dsss_cck_tx_plcp_adjust = dsss_cck_tx_plcp_adjust;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_plcp_delay2_get(void)
{
    return hwp_mac_pcu_reg->mac_rx_plcp_delay2.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_plcp_delay2_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_plcp_delay2.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_plcp_delay2_pack(uint8_t ht_ofdm_rxplcp_delay, uint8_t non_ht_ofdm_rxplcp_delay, uint8_t dsss_cck_rx_plcp_delay)
{
    hwp_mac_pcu_reg->mac_rx_plcp_delay2.val = (((uint32_t)ht_ofdm_rxplcp_delay << 16) | ((uint32_t)non_ht_ofdm_rxplcp_delay << 8) | ((uint32_t)dsss_cck_rx_plcp_delay << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_plcp_delay2_unpack(uint8_t* ht_ofdm_rxplcp_delay, uint8_t* non_ht_ofdm_rxplcp_delay, uint8_t* dsss_cck_rx_plcp_delay)
{
    T_MAC_PCU_REG_MAC_RX_PLCP_DELAY2 localVal = hwp_mac_pcu_reg->mac_rx_plcp_delay2;

    *ht_ofdm_rxplcp_delay = localVal.bit_field.ht_ofdm_rxplcp_delay;
    *non_ht_ofdm_rxplcp_delay = localVal.bit_field.non_ht_ofdm_rxplcp_delay;
    *dsss_cck_rx_plcp_delay = localVal.bit_field.dsss_cck_rx_plcp_delay;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_plcp_delay2_ht_ofdm_rxplcp_delay_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_plcp_delay2.bit_field.ht_ofdm_rxplcp_delay;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_plcp_delay2_ht_ofdm_rxplcp_delay_setf(uint8_t ht_ofdm_rxplcp_delay)
{
    hwp_mac_pcu_reg->mac_rx_plcp_delay2.bit_field.ht_ofdm_rxplcp_delay = ht_ofdm_rxplcp_delay;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_plcp_delay2_non_ht_ofdm_rxplcp_delay_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_plcp_delay2.bit_field.non_ht_ofdm_rxplcp_delay;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_plcp_delay2_non_ht_ofdm_rxplcp_delay_setf(uint8_t non_ht_ofdm_rxplcp_delay)
{
    hwp_mac_pcu_reg->mac_rx_plcp_delay2.bit_field.non_ht_ofdm_rxplcp_delay = non_ht_ofdm_rxplcp_delay;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_plcp_delay2_dsss_cck_rx_plcp_delay_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_plcp_delay2.bit_field.dsss_cck_rx_plcp_delay;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_plcp_delay2_dsss_cck_rx_plcp_delay_setf(uint8_t dsss_cck_rx_plcp_delay)
{
    hwp_mac_pcu_reg->mac_rx_plcp_delay2.bit_field.dsss_cck_rx_plcp_delay = dsss_cck_rx_plcp_delay;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_rx_start_delay_get(void)
{
    return hwp_mac_pcu_reg->mac_rx_start_delay.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_start_delay_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_rx_start_delay.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_start_delay_pack(uint8_t ht_phy_tx_start_delay, uint8_t ofdm_phy_rx_start_delay, uint8_t dsss_phy_rx_start_short_preamble, uint8_t dsss_phy_rx_start_long_preamble)
{
    hwp_mac_pcu_reg->mac_rx_start_delay.val = (((uint32_t)ht_phy_tx_start_delay << 24) | ((uint32_t)ofdm_phy_rx_start_delay << 16) | ((uint32_t)dsss_phy_rx_start_short_preamble << 8) | ((uint32_t)dsss_phy_rx_start_long_preamble << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_start_delay_unpack(uint8_t* ht_phy_tx_start_delay, uint8_t* ofdm_phy_rx_start_delay, uint8_t* dsss_phy_rx_start_short_preamble, uint8_t* dsss_phy_rx_start_long_preamble)
{
    T_MAC_PCU_REG_MAC_RX_START_DELAY localVal = hwp_mac_pcu_reg->mac_rx_start_delay;

    *ht_phy_tx_start_delay = localVal.bit_field.ht_phy_tx_start_delay;
    *ofdm_phy_rx_start_delay = localVal.bit_field.ofdm_phy_rx_start_delay;
    *dsss_phy_rx_start_short_preamble = localVal.bit_field.dsss_phy_rx_start_short_preamble;
    *dsss_phy_rx_start_long_preamble = localVal.bit_field.dsss_phy_rx_start_long_preamble;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_start_delay_ht_phy_tx_start_delay_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_start_delay.bit_field.ht_phy_tx_start_delay;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_start_delay_ht_phy_tx_start_delay_setf(uint8_t ht_phy_tx_start_delay)
{
    hwp_mac_pcu_reg->mac_rx_start_delay.bit_field.ht_phy_tx_start_delay = ht_phy_tx_start_delay;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_start_delay_ofdm_phy_rx_start_delay_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_start_delay.bit_field.ofdm_phy_rx_start_delay;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_start_delay_ofdm_phy_rx_start_delay_setf(uint8_t ofdm_phy_rx_start_delay)
{
    hwp_mac_pcu_reg->mac_rx_start_delay.bit_field.ofdm_phy_rx_start_delay = ofdm_phy_rx_start_delay;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_start_delay_dsss_phy_rx_start_short_preamble_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_start_delay.bit_field.dsss_phy_rx_start_short_preamble;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_start_delay_dsss_phy_rx_start_short_preamble_setf(uint8_t dsss_phy_rx_start_short_preamble)
{
    hwp_mac_pcu_reg->mac_rx_start_delay.bit_field.dsss_phy_rx_start_short_preamble = dsss_phy_rx_start_short_preamble;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_rx_start_delay_dsss_phy_rx_start_long_preamble_getf(void)
{
    return hwp_mac_pcu_reg->mac_rx_start_delay.bit_field.dsss_phy_rx_start_long_preamble;
}

__STATIC_INLINE void mac_pcu_reg_mac_rx_start_delay_dsss_phy_rx_start_long_preamble_setf(uint8_t dsss_phy_rx_start_long_preamble)
{
    hwp_mac_pcu_reg->mac_rx_start_delay.bit_field.dsss_phy_rx_start_long_preamble = dsss_phy_rx_start_long_preamble;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_antenna_set_get(void)
{
    return hwp_mac_pcu_reg->mac_antenna_set.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_antenna_set_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_antenna_set.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_antenna_set_pack(uint8_t nss4, uint8_t nss3, uint8_t nss2, uint8_t nss1)
{
    hwp_mac_pcu_reg->mac_antenna_set.val = (((uint32_t)nss4 << 24) | ((uint32_t)nss3 << 16) | ((uint32_t)nss2 << 8) | ((uint32_t)nss1 << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_antenna_set_unpack(uint8_t* nss4, uint8_t* nss3, uint8_t* nss2, uint8_t* nss1)
{
    T_MAC_PCU_REG_MAC_ANTENNA_SET localVal = hwp_mac_pcu_reg->mac_antenna_set;

    *nss4 = localVal.bit_field.nss4;
    *nss3 = localVal.bit_field.nss3;
    *nss2 = localVal.bit_field.nss2;
    *nss1 = localVal.bit_field.nss1;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_antenna_set_nss4_getf(void)
{
    return hwp_mac_pcu_reg->mac_antenna_set.bit_field.nss4;
}

__STATIC_INLINE void mac_pcu_reg_mac_antenna_set_nss4_setf(uint8_t nss4)
{
    hwp_mac_pcu_reg->mac_antenna_set.bit_field.nss4 = nss4;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_antenna_set_nss3_getf(void)
{
    return hwp_mac_pcu_reg->mac_antenna_set.bit_field.nss3;
}

__STATIC_INLINE void mac_pcu_reg_mac_antenna_set_nss3_setf(uint8_t nss3)
{
    hwp_mac_pcu_reg->mac_antenna_set.bit_field.nss3 = nss3;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_antenna_set_nss2_getf(void)
{
    return hwp_mac_pcu_reg->mac_antenna_set.bit_field.nss2;
}

__STATIC_INLINE void mac_pcu_reg_mac_antenna_set_nss2_setf(uint8_t nss2)
{
    hwp_mac_pcu_reg->mac_antenna_set.bit_field.nss2 = nss2;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_antenna_set_nss1_getf(void)
{
    return hwp_mac_pcu_reg->mac_antenna_set.bit_field.nss1;
}

__STATIC_INLINE void mac_pcu_reg_mac_antenna_set_nss1_setf(uint8_t nss1)
{
    hwp_mac_pcu_reg->mac_antenna_set.bit_field.nss1 = nss1;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_oddr_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_oddr_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_oddr_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_oddr_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_oddr_ctrl_pack(uint8_t invert_phy_regif_clk, uint8_t invert_phy_tx_clk)
{
    hwp_mac_pcu_reg->mac_oddr_ctrl.val = (((uint32_t)invert_phy_regif_clk << 1) | ((uint32_t)invert_phy_tx_clk << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_oddr_ctrl_unpack(uint8_t* invert_phy_regif_clk, uint8_t* invert_phy_tx_clk)
{
    T_MAC_PCU_REG_MAC_ODDR_CTRL localVal = hwp_mac_pcu_reg->mac_oddr_ctrl;

    *invert_phy_regif_clk = localVal.bit_field.invert_phy_regif_clk;
    *invert_phy_tx_clk = localVal.bit_field.invert_phy_tx_clk;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_oddr_ctrl_invert_phy_regif_clk_getf(void)
{
    return hwp_mac_pcu_reg->mac_oddr_ctrl.bit_field.invert_phy_regif_clk;
}

__STATIC_INLINE void mac_pcu_reg_mac_oddr_ctrl_invert_phy_regif_clk_setf(uint8_t invert_phy_regif_clk)
{
    hwp_mac_pcu_reg->mac_oddr_ctrl.bit_field.invert_phy_regif_clk = invert_phy_regif_clk;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_oddr_ctrl_invert_phy_tx_clk_getf(void)
{
    return hwp_mac_pcu_reg->mac_oddr_ctrl.bit_field.invert_phy_tx_clk;
}

__STATIC_INLINE void mac_pcu_reg_mac_oddr_ctrl_invert_phy_tx_clk_setf(uint8_t invert_phy_tx_clk)
{
    hwp_mac_pcu_reg->mac_oddr_ctrl.bit_field.invert_phy_tx_clk = invert_phy_tx_clk;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_peer_station_addr_msb_get(void)
{
    return hwp_mac_pcu_reg->mac_peer_station_addr_msb.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_station_addr_msb_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_peer_station_addr_msb.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_station_addr_msb_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_peer_station_addr_msb.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_station_addr_msb_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_PEER_STATION_ADDR_MSB localVal = hwp_mac_pcu_reg->mac_peer_station_addr_msb;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_peer_station_addr_msb_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_peer_station_addr_msb.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_station_addr_msb_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_peer_station_addr_msb.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_peer_station_addr_lsb_get(void)
{
    return hwp_mac_pcu_reg->mac_peer_station_addr_lsb.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_station_addr_lsb_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_peer_station_addr_lsb.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_station_addr_lsb_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_peer_station_addr_lsb.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_station_addr_lsb_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_PEER_STATION_ADDR_LSB localVal = hwp_mac_pcu_reg->mac_peer_station_addr_lsb;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_peer_station_addr_lsb_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_peer_station_addr_lsb.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_station_addr_lsb_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_peer_station_addr_lsb.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_sequence_number_tid_get(void)
{
    return hwp_mac_pcu_reg->mac_sequence_number_tid.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_sequence_number_tid_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_sequence_number_tid.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_sequence_number_tid_pack(uint16_t sn, uint8_t tid)
{
    hwp_mac_pcu_reg->mac_sequence_number_tid.val = (((uint32_t)sn << 4) | ((uint32_t)tid << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_sequence_number_tid_unpack(uint16_t* sn, uint8_t* tid)
{
    T_MAC_PCU_REG_MAC_SEQUENCE_NUMBER_TID localVal = hwp_mac_pcu_reg->mac_sequence_number_tid;

    *sn = localVal.bit_field.sn;
    *tid = localVal.bit_field.tid;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_sequence_number_tid_sn_getf(void)
{
    return hwp_mac_pcu_reg->mac_sequence_number_tid.bit_field.sn;
}

__STATIC_INLINE void mac_pcu_reg_mac_sequence_number_tid_sn_setf(uint16_t sn)
{
    hwp_mac_pcu_reg->mac_sequence_number_tid.bit_field.sn = sn;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_sequence_number_tid_tid_getf(void)
{
    return hwp_mac_pcu_reg->mac_sequence_number_tid.bit_field.tid;
}

__STATIC_INLINE void mac_pcu_reg_mac_sequence_number_tid_tid_setf(uint8_t tid)
{
    hwp_mac_pcu_reg->mac_sequence_number_tid.bit_field.tid = tid;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ba_frame_header_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_ba_frame_header_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_frame_header_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ba_frame_header_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_frame_header_addr_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ba_frame_header_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_frame_header_addr_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_BA_FRAME_HEADER_ADDR localVal = hwp_mac_pcu_reg->mac_ba_frame_header_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ba_frame_header_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_frame_header_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_frame_header_addr_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ba_frame_header_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_bitmap_location_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_bitmap_location_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_bitmap_location_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_bitmap_location_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_bitmap_location_addr_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_bitmap_location_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_bitmap_location_addr_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_BITMAP_LOCATION_ADDR localVal = hwp_mac_pcu_reg->mac_bitmap_location_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_bitmap_location_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_bitmap_location_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_bitmap_location_addr_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_bitmap_location_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ba_lut_update_get(void)
{
    return hwp_mac_pcu_reg->mac_ba_lut_update.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_lut_update_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ba_lut_update.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_lut_update_pack(uint8_t lut_index, uint8_t lut_operation, uint8_t lut_en)
{
    hwp_mac_pcu_reg->mac_ba_lut_update.val = (((uint32_t)lut_index << 3) | ((uint32_t)lut_operation << 1) | ((uint32_t)lut_en << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_lut_update_unpack(uint8_t* lut_index, uint8_t* lut_operation, uint8_t* lut_en)
{
    T_MAC_PCU_REG_MAC_BA_LUT_UPDATE localVal = hwp_mac_pcu_reg->mac_ba_lut_update;

    *lut_index = localVal.bit_field.lut_index;
    *lut_operation = localVal.bit_field.lut_operation;
    *lut_en = localVal.bit_field.lut_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ba_lut_update_lut_index_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_lut_update.bit_field.lut_index;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_lut_update_lut_index_setf(uint8_t lut_index)
{
    hwp_mac_pcu_reg->mac_ba_lut_update.bit_field.lut_index = lut_index;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ba_lut_update_lut_operation_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_lut_update.bit_field.lut_operation;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_lut_update_lut_operation_setf(uint8_t lut_operation)
{
    hwp_mac_pcu_reg->mac_ba_lut_update.bit_field.lut_operation = lut_operation;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ba_lut_update_lut_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_lut_update.bit_field.lut_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_lut_update_lut_en_setf(uint8_t lut_en)
{
    hwp_mac_pcu_reg->mac_ba_lut_update.bit_field.lut_en = lut_en;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ba_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_ba_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ba_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_ctrl_pack(uint8_t lut_index, uint8_t lut_operation, uint8_t lut_en, uint8_t ba_en)
{
    hwp_mac_pcu_reg->mac_ba_ctrl.val = (((uint32_t)lut_index << 3) | ((uint32_t)lut_operation << 2) | ((uint32_t)lut_en << 1) | ((uint32_t)ba_en << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_ctrl_unpack(uint8_t* lut_index, uint8_t* lut_operation, uint8_t* lut_en, uint8_t* ba_en)
{
    T_MAC_PCU_REG_MAC_BA_CTRL localVal = hwp_mac_pcu_reg->mac_ba_ctrl;

    *lut_index = localVal.bit_field.lut_index;
    *lut_operation = localVal.bit_field.lut_operation;
    *lut_en = localVal.bit_field.lut_en;
    *ba_en = localVal.bit_field.ba_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ba_ctrl_lut_index_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_ctrl.bit_field.lut_index;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_ctrl_lut_index_setf(uint8_t lut_index)
{
    hwp_mac_pcu_reg->mac_ba_ctrl.bit_field.lut_index = lut_index;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ba_ctrl_lut_operation_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_ctrl.bit_field.lut_operation;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_ctrl_lut_operation_setf(uint8_t lut_operation)
{
    hwp_mac_pcu_reg->mac_ba_ctrl.bit_field.lut_operation = lut_operation;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ba_ctrl_lut_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_ctrl.bit_field.lut_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_ctrl_lut_en_setf(uint8_t lut_en)
{
    hwp_mac_pcu_reg->mac_ba_ctrl.bit_field.lut_en = lut_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ba_ctrl_ba_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_ctrl.bit_field.ba_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_ctrl_ba_en_setf(uint8_t ba_en)
{
    hwp_mac_pcu_reg->mac_ba_ctrl.bit_field.ba_en = ba_en;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_peer_sta_addr_msb_get(void)
{
    return hwp_mac_pcu_reg->mac_peer_sta_addr_msb.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_sta_addr_msb_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_peer_sta_addr_msb.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_sta_addr_msb_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_peer_sta_addr_msb.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_sta_addr_msb_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_PEER_STA_ADDR_MSB localVal = hwp_mac_pcu_reg->mac_peer_sta_addr_msb;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_peer_sta_addr_msb_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_peer_sta_addr_msb.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_sta_addr_msb_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_peer_sta_addr_msb.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_peer_sta_addr_lsb_get(void)
{
    return hwp_mac_pcu_reg->mac_peer_sta_addr_lsb.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_sta_addr_lsb_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_peer_sta_addr_lsb.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_sta_addr_lsb_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_peer_sta_addr_lsb.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_sta_addr_lsb_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_PEER_STA_ADDR_LSB localVal = hwp_mac_pcu_reg->mac_peer_sta_addr_lsb;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_peer_sta_addr_lsb_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_peer_sta_addr_lsb.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_peer_sta_addr_lsb_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_peer_sta_addr_lsb.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ba_para_size_get(void)
{
    return hwp_mac_pcu_reg->mac_ba_para_size.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_para_size_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ba_para_size.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_para_size_pack(uint8_t win_size, uint8_t partial_state, uint16_t sn, uint8_t tid)
{
    hwp_mac_pcu_reg->mac_ba_para_size.val = (((uint32_t)win_size << 24) | ((uint32_t)partial_state << 16) | ((uint32_t)sn << 4) | ((uint32_t)tid << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_para_size_unpack(uint8_t* win_size, uint8_t* partial_state, uint16_t* sn, uint8_t* tid)
{
    T_MAC_PCU_REG_MAC_BA_PARA_SIZE localVal = hwp_mac_pcu_reg->mac_ba_para_size;

    *win_size = localVal.bit_field.win_size;
    *partial_state = localVal.bit_field.partial_state;
    *sn = localVal.bit_field.sn;
    *tid = localVal.bit_field.tid;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ba_para_size_win_size_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_para_size.bit_field.win_size;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_para_size_win_size_setf(uint8_t win_size)
{
    hwp_mac_pcu_reg->mac_ba_para_size.bit_field.win_size = win_size;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ba_para_size_partial_state_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_para_size.bit_field.partial_state;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_para_size_partial_state_setf(uint8_t partial_state)
{
    hwp_mac_pcu_reg->mac_ba_para_size.bit_field.partial_state = partial_state;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_ba_para_size_sn_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_para_size.bit_field.sn;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_para_size_sn_setf(uint16_t sn)
{
    hwp_mac_pcu_reg->mac_ba_para_size.bit_field.sn = sn;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_ba_para_size_tid_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_para_size.bit_field.tid;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_para_size_tid_setf(uint8_t tid)
{
    hwp_mac_pcu_reg->mac_ba_para_size.bit_field.tid = tid;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ba_compressed_bitmap_msw_get(void)
{
    return hwp_mac_pcu_reg->mac_ba_compressed_bitmap_msw.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_compressed_bitmap_msw_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ba_compressed_bitmap_msw.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_compressed_bitmap_msw_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ba_compressed_bitmap_msw.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_compressed_bitmap_msw_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_BA_COMPRESSED_BITMAP_MSW localVal = hwp_mac_pcu_reg->mac_ba_compressed_bitmap_msw;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ba_compressed_bitmap_msw_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_compressed_bitmap_msw.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_compressed_bitmap_msw_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ba_compressed_bitmap_msw.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ba_compressed_bitmap_lsw_get(void)
{
    return hwp_mac_pcu_reg->mac_ba_compressed_bitmap_lsw.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_compressed_bitmap_lsw_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ba_compressed_bitmap_lsw.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_compressed_bitmap_lsw_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ba_compressed_bitmap_lsw.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_compressed_bitmap_lsw_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_BA_COMPRESSED_BITMAP_LSW localVal = hwp_mac_pcu_reg->mac_ba_compressed_bitmap_lsw;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_ba_compressed_bitmap_lsw_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_ba_compressed_bitmap_lsw.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_ba_compressed_bitmap_lsw_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_ba_compressed_bitmap_lsw.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_schedule_link_addr_get(void)
{
    return hwp_mac_pcu_reg->mac_schedule_link_addr.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_schedule_link_addr_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_schedule_link_addr.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_schedule_link_addr_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_schedule_link_addr.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_schedule_link_addr_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_SCHEDULE_LINK_ADDR localVal = hwp_mac_pcu_reg->mac_schedule_link_addr;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_schedule_link_addr_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_schedule_link_addr.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_schedule_link_addr_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_schedule_link_addr.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_cap_start_time_get(void)
{
    return hwp_mac_pcu_reg->mac_cap_start_time.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_cap_start_time_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_cap_start_time.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_cap_start_time_pack(uint8_t pre_start_offset_time, uint8_t start_time_before_tbtt)
{
    hwp_mac_pcu_reg->mac_cap_start_time.val = (((uint32_t)pre_start_offset_time << 16) | ((uint32_t)start_time_before_tbtt << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_cap_start_time_unpack(uint8_t* pre_start_offset_time, uint8_t* start_time_before_tbtt)
{
    T_MAC_PCU_REG_MAC_CAP_START_TIME localVal = hwp_mac_pcu_reg->mac_cap_start_time;

    *pre_start_offset_time = localVal.bit_field.pre_start_offset_time;
    *start_time_before_tbtt = localVal.bit_field.start_time_before_tbtt;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_cap_start_time_pre_start_offset_time_getf(void)
{
    return hwp_mac_pcu_reg->mac_cap_start_time.bit_field.pre_start_offset_time;
}

__STATIC_INLINE void mac_pcu_reg_mac_cap_start_time_pre_start_offset_time_setf(uint8_t pre_start_offset_time)
{
    hwp_mac_pcu_reg->mac_cap_start_time.bit_field.pre_start_offset_time = pre_start_offset_time;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_cap_start_time_start_time_before_tbtt_getf(void)
{
    return hwp_mac_pcu_reg->mac_cap_start_time.bit_field.start_time_before_tbtt;
}

__STATIC_INLINE void mac_pcu_reg_mac_cap_start_time_start_time_before_tbtt_setf(uint8_t start_time_before_tbtt)
{
    hwp_mac_pcu_reg->mac_cap_start_time.bit_field.start_time_before_tbtt = start_time_before_tbtt;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_p2p_ctrl_get(void)
{
    return hwp_mac_pcu_reg->mac_p2p_ctrl.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_p2p_ctrl_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_p2p_ctrl.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_p2p_ctrl_pack(uint8_t cancel_noa, uint16_t rx_beacon_timeout, uint8_t ctwindow, uint8_t ctwindow_txop_allowed, uint8_t mode, uint8_t ops_en, uint8_t p2p_en)
{
    hwp_mac_pcu_reg->mac_p2p_ctrl.val = (((uint32_t)cancel_noa << 28) | ((uint32_t)rx_beacon_timeout << 12) | ((uint32_t)ctwindow << 4) | ((uint32_t)ctwindow_txop_allowed << 3) | ((uint32_t)mode << 2) | ((uint32_t)ops_en << 1) | ((uint32_t)p2p_en << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_p2p_ctrl_unpack(uint8_t* cancel_noa, uint16_t* rx_beacon_timeout, uint8_t* ctwindow, uint8_t* ctwindow_txop_allowed, uint8_t* mode, uint8_t* ops_en, uint8_t* p2p_en)
{
    T_MAC_PCU_REG_MAC_P2P_CTRL localVal = hwp_mac_pcu_reg->mac_p2p_ctrl;

    *cancel_noa = localVal.bit_field.cancel_noa;
    *rx_beacon_timeout = localVal.bit_field.rx_beacon_timeout;
    *ctwindow = localVal.bit_field.ctwindow;
    *ctwindow_txop_allowed = localVal.bit_field.ctwindow_txop_allowed;
    *mode = localVal.bit_field.mode;
    *ops_en = localVal.bit_field.ops_en;
    *p2p_en = localVal.bit_field.p2p_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_p2p_ctrl_cancel_noa_getf(void)
{
    return hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.cancel_noa;
}

__STATIC_INLINE void mac_pcu_reg_mac_p2p_ctrl_cancel_noa_setf(uint8_t cancel_noa)
{
    hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.cancel_noa = cancel_noa;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_p2p_ctrl_rx_beacon_timeout_getf(void)
{
    return hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.rx_beacon_timeout;
}

__STATIC_INLINE void mac_pcu_reg_mac_p2p_ctrl_rx_beacon_timeout_setf(uint16_t rx_beacon_timeout)
{
    hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.rx_beacon_timeout = rx_beacon_timeout;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_p2p_ctrl_ctwindow_getf(void)
{
    return hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.ctwindow;
}

__STATIC_INLINE void mac_pcu_reg_mac_p2p_ctrl_ctwindow_setf(uint8_t ctwindow)
{
    hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.ctwindow = ctwindow;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_p2p_ctrl_ctwindow_txop_allowed_getf(void)
{
    return hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.ctwindow_txop_allowed;
}

__STATIC_INLINE void mac_pcu_reg_mac_p2p_ctrl_ctwindow_txop_allowed_setf(uint8_t ctwindow_txop_allowed)
{
    hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.ctwindow_txop_allowed = ctwindow_txop_allowed;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_p2p_ctrl_mode_getf(void)
{
    return hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.mode;
}

__STATIC_INLINE void mac_pcu_reg_mac_p2p_ctrl_mode_setf(uint8_t mode)
{
    hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.mode = mode;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_p2p_ctrl_ops_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.ops_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_p2p_ctrl_ops_en_setf(uint8_t ops_en)
{
    hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.ops_en = ops_en;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_p2p_ctrl_p2p_en_getf(void)
{
    return hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.p2p_en;
}

__STATIC_INLINE void mac_pcu_reg_mac_p2p_ctrl_p2p_en_setf(uint8_t p2p_en)
{
    hwp_mac_pcu_reg->mac_p2p_ctrl.bit_field.p2p_en = p2p_en;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_noa_count_status_get(void)
{
    return hwp_mac_pcu_reg->mac_noa_count_status.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa_count_status_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_noa_count_status.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa_count_status_pack(uint8_t noa2_status, uint8_t noa1_status, uint8_t noa2_cnt, uint8_t noa1_cnt)
{
    hwp_mac_pcu_reg->mac_noa_count_status.val = (((uint32_t)noa2_status << 18) | ((uint32_t)noa1_status << 16) | ((uint32_t)noa2_cnt << 8) | ((uint32_t)noa1_cnt << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_noa_count_status_unpack(uint8_t* noa2_status, uint8_t* noa1_status, uint8_t* noa2_cnt, uint8_t* noa1_cnt)
{
    T_MAC_PCU_REG_MAC_NOA_COUNT_STATUS localVal = hwp_mac_pcu_reg->mac_noa_count_status;

    *noa2_status = localVal.bit_field.noa2_status;
    *noa1_status = localVal.bit_field.noa1_status;
    *noa2_cnt = localVal.bit_field.noa2_cnt;
    *noa1_cnt = localVal.bit_field.noa1_cnt;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_noa_count_status_noa2_status_getf(void)
{
    return hwp_mac_pcu_reg->mac_noa_count_status.bit_field.noa2_status;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa_count_status_noa2_status_setf(uint8_t noa2_status)
{
    hwp_mac_pcu_reg->mac_noa_count_status.bit_field.noa2_status = noa2_status;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_noa_count_status_noa1_status_getf(void)
{
    return hwp_mac_pcu_reg->mac_noa_count_status.bit_field.noa1_status;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa_count_status_noa1_status_setf(uint8_t noa1_status)
{
    hwp_mac_pcu_reg->mac_noa_count_status.bit_field.noa1_status = noa1_status;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_noa_count_status_noa2_cnt_getf(void)
{
    return hwp_mac_pcu_reg->mac_noa_count_status.bit_field.noa2_cnt;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa_count_status_noa2_cnt_setf(uint8_t noa2_cnt)
{
    hwp_mac_pcu_reg->mac_noa_count_status.bit_field.noa2_cnt = noa2_cnt;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_noa_count_status_noa1_cnt_getf(void)
{
    return hwp_mac_pcu_reg->mac_noa_count_status.bit_field.noa1_cnt;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa_count_status_noa1_cnt_setf(uint8_t noa1_cnt)
{
    hwp_mac_pcu_reg->mac_noa_count_status.bit_field.noa1_cnt = noa1_cnt;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_noa1_duration_get(void)
{
    return hwp_mac_pcu_reg->mac_noa1_duration.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa1_duration_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_noa1_duration.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa1_duration_pack(uint32_t duration)
{
    hwp_mac_pcu_reg->mac_noa1_duration.val = (((uint32_t)duration << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_noa1_duration_unpack(uint32_t* duration)
{
    T_MAC_PCU_REG_MAC_NOA1_DURATION localVal = hwp_mac_pcu_reg->mac_noa1_duration;

    *duration = localVal.bit_field.duration;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_noa1_duration_duration_getf(void)
{
    return hwp_mac_pcu_reg->mac_noa1_duration.bit_field.duration;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa1_duration_duration_setf(uint32_t duration)
{
    hwp_mac_pcu_reg->mac_noa1_duration.bit_field.duration = duration;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_noa1_interval_get(void)
{
    return hwp_mac_pcu_reg->mac_noa1_interval.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa1_interval_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_noa1_interval.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa1_interval_pack(uint32_t interval)
{
    hwp_mac_pcu_reg->mac_noa1_interval.val = (((uint32_t)interval << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_noa1_interval_unpack(uint32_t* interval)
{
    T_MAC_PCU_REG_MAC_NOA1_INTERVAL localVal = hwp_mac_pcu_reg->mac_noa1_interval;

    *interval = localVal.bit_field.interval;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_noa1_interval_interval_getf(void)
{
    return hwp_mac_pcu_reg->mac_noa1_interval.bit_field.interval;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa1_interval_interval_setf(uint32_t interval)
{
    hwp_mac_pcu_reg->mac_noa1_interval.bit_field.interval = interval;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_noa1_start_time_get(void)
{
    return hwp_mac_pcu_reg->mac_noa1_start_time.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa1_start_time_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_noa1_start_time.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa1_start_time_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_noa1_start_time.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_noa1_start_time_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_NOA1_START_TIME localVal = hwp_mac_pcu_reg->mac_noa1_start_time;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_noa1_start_time_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_noa1_start_time.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa1_start_time_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_noa1_start_time.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_noa2_duration_get(void)
{
    return hwp_mac_pcu_reg->mac_noa2_duration.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa2_duration_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_noa2_duration.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa2_duration_pack(uint32_t duration)
{
    hwp_mac_pcu_reg->mac_noa2_duration.val = (((uint32_t)duration << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_noa2_duration_unpack(uint32_t* duration)
{
    T_MAC_PCU_REG_MAC_NOA2_DURATION localVal = hwp_mac_pcu_reg->mac_noa2_duration;

    *duration = localVal.bit_field.duration;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_noa2_duration_duration_getf(void)
{
    return hwp_mac_pcu_reg->mac_noa2_duration.bit_field.duration;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa2_duration_duration_setf(uint32_t duration)
{
    hwp_mac_pcu_reg->mac_noa2_duration.bit_field.duration = duration;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_noa2_interval_get(void)
{
    return hwp_mac_pcu_reg->mac_noa2_interval.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa2_interval_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_noa2_interval.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa2_interval_pack(uint32_t interval)
{
    hwp_mac_pcu_reg->mac_noa2_interval.val = (((uint32_t)interval << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_noa2_interval_unpack(uint32_t* interval)
{
    T_MAC_PCU_REG_MAC_NOA2_INTERVAL localVal = hwp_mac_pcu_reg->mac_noa2_interval;

    *interval = localVal.bit_field.interval;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_noa2_interval_interval_getf(void)
{
    return hwp_mac_pcu_reg->mac_noa2_interval.bit_field.interval;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa2_interval_interval_setf(uint32_t interval)
{
    hwp_mac_pcu_reg->mac_noa2_interval.bit_field.interval = interval;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_noa2_start_time_get(void)
{
    return hwp_mac_pcu_reg->mac_noa2_start_time.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa2_start_time_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_noa2_start_time.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa2_start_time_pack(uint32_t value)
{
    hwp_mac_pcu_reg->mac_noa2_start_time.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_noa2_start_time_unpack(uint32_t* value)
{
    T_MAC_PCU_REG_MAC_NOA2_START_TIME localVal = hwp_mac_pcu_reg->mac_noa2_start_time;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_noa2_start_time_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_noa2_start_time.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_noa2_start_time_value_setf(uint32_t value)
{
    hwp_mac_pcu_reg->mac_noa2_start_time.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_eoa_offset_get(void)
{
    return hwp_mac_pcu_reg->mac_eoa_offset.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_eoa_offset_set(uint32_t value)
{
    hwp_mac_pcu_reg->mac_eoa_offset.val = value;
}

__STATIC_INLINE void mac_pcu_reg_mac_eoa_offset_pack(uint16_t value)
{
    hwp_mac_pcu_reg->mac_eoa_offset.val = (((uint32_t)value << 0));
}

__STATIC_INLINE void mac_pcu_reg_mac_eoa_offset_unpack(uint16_t* value)
{
    T_MAC_PCU_REG_MAC_EOA_OFFSET localVal = hwp_mac_pcu_reg->mac_eoa_offset;

    *value = localVal.bit_field.value;
}

__STATIC_INLINE uint16_t mac_pcu_reg_mac_eoa_offset_value_getf(void)
{
    return hwp_mac_pcu_reg->mac_eoa_offset.bit_field.value;
}

__STATIC_INLINE void mac_pcu_reg_mac_eoa_offset_value_setf(uint16_t value)
{
    hwp_mac_pcu_reg->mac_eoa_offset.bit_field.value = value;
}

__STATIC_INLINE uint32_t mac_pcu_reg_mac_p2p_status_get(void)
{
    return hwp_mac_pcu_reg->mac_p2p_status.val;
}

__STATIC_INLINE void mac_pcu_reg_mac_p2p_status_unpack(uint8_t* ctwindow_progress, uint8_t* noa2_periodic, uint8_t* noa2_progress, uint8_t* noa1_periodic, uint8_t* noa1_progress)
{
    T_MAC_PCU_REG_MAC_P2P_STATUS localVal = hwp_mac_pcu_reg->mac_p2p_status;

    *ctwindow_progress = localVal.bit_field.ctwindow_progress;
    *noa2_periodic = localVal.bit_field.noa2_periodic;
    *noa2_progress = localVal.bit_field.noa2_progress;
    *noa1_periodic = localVal.bit_field.noa1_periodic;
    *noa1_progress = localVal.bit_field.noa1_progress;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_p2p_status_ctwindow_progress_getf(void)
{
    return hwp_mac_pcu_reg->mac_p2p_status.bit_field.ctwindow_progress;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_p2p_status_noa2_periodic_getf(void)
{
    return hwp_mac_pcu_reg->mac_p2p_status.bit_field.noa2_periodic;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_p2p_status_noa2_progress_getf(void)
{
    return hwp_mac_pcu_reg->mac_p2p_status.bit_field.noa2_progress;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_p2p_status_noa1_periodic_getf(void)
{
    return hwp_mac_pcu_reg->mac_p2p_status.bit_field.noa1_periodic;
}

__STATIC_INLINE uint8_t mac_pcu_reg_mac_p2p_status_noa1_progress_getf(void)
{
    return hwp_mac_pcu_reg->mac_p2p_status.bit_field.noa1_progress;
}
#endif


