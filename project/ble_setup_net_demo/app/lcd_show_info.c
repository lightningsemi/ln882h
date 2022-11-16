/**
 * @file      lcd_show_info.c
 * @brief 
 * @author    Ju Chao (chao.ju@qq.com)
 * @version   1.0
 * @date      2022-07-10
*/


#include "get_internet_info.h"
#include "lcd_show_info.h"

#include "osal/osal.h"
#include "utils/debug/log.h"
#include "utils/debug/ln_assert.h"

#include "lwip/dns.h"
#include "lwip/tcpip.h"
#include "lwip/ip.h"
#include "lwip/api.h"
#include "netif/ethernetif.h"


#include "usr_app.h"
#include "cJSON.h"
#include "ln882h.h"
#include "utils/debug/log.h"
#include "ln_drv_spi_lcd.h"
#include "ln_spi_lcd_pic.h"
#include "weather_num.h"
#include "lcd_font.h"
#include "hal/hal_rtc.h"
#include "hal/hal_misc.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"

#if USE_IR_DEVICE
    #include "ln_drv_ir.h"
#endif

wifi_state_machine current_state = wifi_sta_init;
wifi_state_machine next_state    = wifi_sta_init;

uint32_t system_second = 0;
uint32_t ntp_second = 0;
uint8_t  update_flag = 0;
uint8_t  weather_flag = 0;
time_t   sys_time;
time_t   last_update_time;
weather_t *my_weather;
 
void rtc_init(uint32_t ms);
void ntp_2_utc(uint32_t second,time_t *time);

uint8_t week_turn(uint8_t num,char * str)
{
    switch(num)
    {
        case 0: memcpy(str,"日",sizeof("日")); return 0;
        case 1: memcpy(str,"一",sizeof("日")); return 0;
        case 2: memcpy(str,"二",sizeof("日")); return 0;
        case 3: memcpy(str,"三",sizeof("日")); return 0;
        case 4: memcpy(str,"四",sizeof("日")); return 0;
        case 5: memcpy(str,"五",sizeof("日")); return 0;
        case 6: memcpy(str,"六",sizeof("日")); return 0;
    }
    return 1;
}

const lv_img_dsc_t* selectWeatherImg(uint32_t weather_code)
{
    //LV_IMG_DECLARE(w0)
    switch (weather_code)
    {
        case 0:;
        case 1:;
        case 2:;
        case 3: LV_IMG_DECLARE(w0) return &w0;
        case 4: LV_IMG_DECLARE(w4) return &w4;
        case 5:;
        case 6: LV_IMG_DECLARE(w5) return &w5;
        case 7:
        case 8: LV_IMG_DECLARE(w7) return &w7;
        case 9: LV_IMG_DECLARE(w9) return &w9;
        
        case 10: LV_IMG_DECLARE(w10) return &w10;
        case 11: LV_IMG_DECLARE(w11) return &w11;
        case 12: LV_IMG_DECLARE(w12) return &w12;
        
        case 13: LV_IMG_DECLARE(w13) return &w13;
        case 14: LV_IMG_DECLARE(w14) return &w14;
        case 15: LV_IMG_DECLARE(w15) return &w15;
        case 16: LV_IMG_DECLARE(w16) return &w16;
        case 17: LV_IMG_DECLARE(w16) return &w16;
        case 18: LV_IMG_DECLARE(w16) return &w16;
        case 19: LV_IMG_DECLARE(w19) return &w19;
        
        case 20: LV_IMG_DECLARE(w20) return &w20;
        case 21: LV_IMG_DECLARE(w21) return &w21;
        case 22: LV_IMG_DECLARE(w22) return &w22;
        case 23: LV_IMG_DECLARE(w23) return &w23;
        case 24: LV_IMG_DECLARE(w24) return &w24;
        case 25: LV_IMG_DECLARE(w25) return &w25;
        
        case 26: LV_IMG_DECLARE(w26) return &w26;
        case 27: LV_IMG_DECLARE(w27) return &w27;
        
        case 28: LV_IMG_DECLARE(w28) return &w28;
        case 29: LV_IMG_DECLARE(w29) return &w29;
        
        case 30: LV_IMG_DECLARE(w30) return &w30;
        case 31: LV_IMG_DECLARE(w31) return &w31;
        
        case 32: LV_IMG_DECLARE(w32) return &w32;
        case 33: LV_IMG_DECLARE(w33) return &w33;
        case 34: LV_IMG_DECLARE(w34) return &w34;
        case 35: LV_IMG_DECLARE(w35) return &w35;
        case 36: LV_IMG_DECLARE(w36) return &w36;
        
        case 37: LV_IMG_DECLARE(w37) return &w37;
        case 38: LV_IMG_DECLARE(w38) return &w38;
        
        default: LV_IMG_DECLARE(w99) return &w99;
    }
    return 0;   
}

uint8_t weather_turn(uint8_t num, char* str)
{
    switch (num)
    {
    case 0:;
    case 1:;
    case 2:;
    case 3: memcpy(str,"晴",sizeof("晴")); return 0;
    case 4: memcpy(str,"多云",sizeof("多云")); return 0;
    case 5:;
    case 6: memcpy(str,"晴间多云",sizeof("晴间多云")); return 0;
    case 7:
    case 8: memcpy(str,"大部多云",sizeof("大部多云")); return 0;
    case 9: memcpy(str,"阴",sizeof("阴")); return 0;
        
    case 10: memcpy(str,"阵雨",sizeof("阵雨")); return 0;
    case 11: memcpy(str,"雷阵雨",sizeof("雷阵雨")); return 0;
    case 12: memcpy(str,"雷阵雨伴有冰雹",sizeof("雷阵雨伴有冰雹")); return 0;
        
    case 13: memcpy(str,"小雨",sizeof("小雨")); return 0;
    case 14: memcpy(str,"中雨",sizeof("中雨")); return 0;
    case 15: memcpy(str,"大雨",sizeof("大雨")); return 0;
    case 16: memcpy(str,"暴雨",sizeof("暴雨")); return 0;
    case 17: memcpy(str,"大暴雨",sizeof("大暴雨")); return 0;
    case 18: memcpy(str,"特大暴雨",sizeof("特大暴雨")); return 0;
    case 19: memcpy(str,"冻雨",sizeof("冻雨")); return 0;
        
    case 20: memcpy(str,"雨夹雪",sizeof("雨夹雪")); return 0;
    case 21: memcpy(str,"阵雪",sizeof("阵雪")); return 0;
    case 22: memcpy(str,"小雪",sizeof("小雪")); return 0;
    case 23: memcpy(str,"中雪",sizeof("中雪")); return 0;
    case 24: memcpy(str,"大雪",sizeof("大雪")); return 0;
    case 25: memcpy(str,"暴雪",sizeof("暴雪")); return 0;
        
    case 26: memcpy(str,"浮尘",sizeof("浮尘")); return 0;
    case 27: memcpy(str,"扬沙",sizeof("扬沙")); return 0;
        
    case 28: memcpy(str,"沙尘暴",sizeof("沙尘暴")); return 0;
    case 29: memcpy(str,"强沙尘暴",sizeof("强沙尘暴")); return 0;
        
    case 30: memcpy(str,"雾",sizeof("雾")); return 0;
    case 31: memcpy(str,"霾",sizeof("霾")); return 0;
        
    case 32: memcpy(str,"风",sizeof("风")); return 0;
    case 33: memcpy(str,"大风",sizeof("大风")); return 0;
    case 34: memcpy(str,"飓风",sizeof("飓风")); return 0;
    case 35: memcpy(str,"热带风暴",sizeof("热带风暴")); return 0;
    case 36: memcpy(str,"龙卷风",sizeof("龙卷风")); return 0;
        
    case 37: memcpy(str,"冷",sizeof("冷")); return 0;
    case 38: memcpy(str,"热",sizeof("热")); return 0;
        
    default: memcpy(str,"未知",sizeof("未知")); return 0;
    }
    return 1;
}


void lcd_show_info_task_entry(void *params)
{
    
    uint32_t cnt = 0;

    // 1. 初始化SPI LCD
    spi_lcd_init();
    spi_lcd_fill(0,0,320,240,WHITE);

    while(update_flag == 0){lvgl_state_machine();lv_task_handler();OS_MsDelay(10);}
   
    lvgl_set_next_state_machine(wifi_get_info_success);
    lvgl_state_machine();
    lv_task_handler();
    OS_MsDelay(20);
    
    lvgl_set_next_state_machine(wifi_get_info_success);
    lvgl_state_machine();
    lv_task_handler();
    OS_MsDelay(1000);
    lvgl_set_next_state_machine(wifi_sta_running);
    
    rtc_init(1000);
    while(1)
    {
        lvgl_state_machine();
        lv_task_handler();
        OS_MsDelay(1);
        
        
        
    #if USE_IR_DEVICE
        //定时发送开启空调命令
        static uint32_t cnt = 0;
        static uint32_t flag = 0;


        if(sys_time.hour == 7 && sys_time.minute == 00 && sys_time.second == 0 && flag == 0){
            ln_drv_ir_send_cmd();
            flag = 1;
            cnt = 0;
        }
        if(cnt > 10000){
            flag = 0;
        }
    }
    #endif
}

void lvgl_normal_show()
{
    static uint8_t init_flag = 0;
    static lv_obj_t* btn1;
    static lv_obj_t* btn2;
    static lv_obj_t* btn3;
    static lv_obj_t* wea_img;
    static lv_obj_t* time_label;
    static lv_obj_t* gps_label;
    static lv_obj_t* wifi_label;
    static lv_obj_t* local_label;
    static lv_obj_t* data_label;
    static lv_obj_t* wea_code_label;
    static lv_obj_t* wea_tem_label;
    static lv_obj_t* wifi_name_label;
    static lv_obj_t* day_wea_label;
    static lv_obj_t* night_wea_label;
    static lv_obj_t* indoor_tem_label;
    static lv_obj_t* indoor_hum_label;

    static lv_style_t day_wea_style;
    static lv_style_t night_wea_style;

    char buf[50];
    if(init_flag == 0){
        init_flag = 1;
        spi_lcd_fill(0,0,320,240,WHITE);

        //背景绘制
        btn1 = lv_btn_create(lv_scr_act());
        lv_obj_set_pos(btn1, 5, 5);
        lv_obj_set_size(btn1, 310, 35);
        lv_obj_set_style_bg_color(btn1, lv_palette_main(LV_PALETTE_BLUE),0);

        btn2 = lv_btn_create(lv_scr_act());
        lv_obj_set_pos(btn2, 5, 45);
        lv_obj_set_size(btn2, 310, 120);
        lv_obj_set_style_bg_color(btn2, lv_palette_main(LV_PALETTE_BLUE), 0);

        btn3 = lv_btn_create(lv_scr_act());
        lv_obj_set_pos(btn3, 5, 170);
        lv_obj_set_size(btn3, 310, 65);
        lv_obj_set_style_bg_color(btn3, lv_palette_main(LV_PALETTE_BLUE), 0);

        //天气图片绘制
        wea_img = lv_img_create(lv_scr_act());
        lv_img_set_src(wea_img, &w0);
        lv_obj_set_pos(wea_img, 35, 55);
        
        //时间标签绘制
        time_label = lv_label_create(btn2);
        lv_obj_set_pos(time_label, 80, 10);
        lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, 0);
        lv_obj_set_size(time_label, 230, 48);
        lv_obj_set_style_text_align(time_label, LV_TEXT_ALIGN_CENTER, 0);
        
        //位置图标绘制
        gps_label = lv_label_create(btn1);
        lv_obj_set_pos(gps_label, 0, 5);
        lv_obj_set_style_text_font(gps_label, &lv_font_montserrat_14, 0);
        lv_label_set_text(gps_label, LV_SYMBOL_GPS);
        
        //wifi图标绘制
        wifi_label = lv_label_create(btn1);
        lv_obj_set_pos(wifi_label, 160, 0);
        lv_obj_set_style_text_font(wifi_label, &lv_font_montserrat_14, 0);
        lv_label_set_text(wifi_label, LV_SYMBOL_WIFI);

        //wifi名称标签绘制
        extern char wifi_ssid[50];
        wifi_name_label = lv_label_create(btn1);
        lv_obj_set_pos(wifi_name_label, 180, 0);
        lv_obj_set_style_text_font(wifi_name_label, &lv_font_montserrat_14, 0);
        lv_label_set_text(wifi_name_label, wifi_ssid);
        lv_label_set_long_mode(wifi_name_label, LV_LABEL_LONG_SCROLL_CIRCULAR);    
        lv_obj_set_width(wifi_name_label, 140);
        
        //位置标签绘制
        local_label = lv_label_create(btn1);
        lv_obj_set_pos(local_label, 15, 0);
        lv_obj_set_style_text_font(local_label, &my_chinese_font_24, 0);
        lv_label_set_text(local_label, "上海市");
        
        //日期标签绘制
        data_label = lv_label_create(btn2);
        lv_obj_set_pos(data_label, 90, 80);
        lv_obj_set_style_text_font(data_label, &my_chinese_font_24, 0);

        //天气标签绘制
        wea_code_label = lv_label_create(btn2);
        lv_obj_set_pos(wea_code_label, 0, 60);
        lv_obj_set_style_text_font(wea_code_label, &my_chinese_font_24, 0);
        lv_label_set_long_mode(wea_code_label, LV_LABEL_LONG_WRAP);
        lv_obj_set_size(wea_code_label, 85, 25);
        lv_obj_set_style_text_align(wea_code_label, LV_TEXT_ALIGN_CENTER, 0);
        
        //温度标签绘制
        wea_tem_label = lv_label_create(btn2);
        lv_obj_set_pos(wea_tem_label, 0, 85);
        lv_obj_set_style_text_font(wea_tem_label, &lv_font_montserrat_24, 0);
        lv_label_set_long_mode(wea_tem_label, LV_LABEL_LONG_WRAP);
        lv_obj_set_size(wea_tem_label, 85, 25);
        lv_obj_set_style_text_align(wea_tem_label, LV_TEXT_ALIGN_CENTER, 0);


        //白天天气标签绘制
        day_wea_label = lv_label_create(btn3); 
        lv_obj_set_pos(day_wea_label, 0, 5);
        lv_obj_set_style_text_font(day_wea_label, &my_chinese_font_24, 0);
        lv_style_init(&day_wea_style);
        //lv_style_set_text_color(&day_wea_style, lv_palette_main(LV_PALETTE_YELLOW));
        lv_obj_add_style(day_wea_label, &day_wea_style, 0);
        lv_label_set_long_mode(day_wea_label, LV_LABEL_LONG_SCROLL_CIRCULAR);    
        lv_obj_set_width(day_wea_label, 135);


        //夜晚天气标签绘制
        night_wea_label = lv_label_create(btn3); 
        lv_obj_set_pos(night_wea_label, 0, 35);
        lv_obj_set_style_text_font(night_wea_label, &my_chinese_font_24, 0);
        lv_style_init(&night_wea_style);
        //lv_style_set_text_color(&night_wea_style, lv_palette_main(LV_PALETTE_WHITE));
        lv_obj_add_style(night_wea_label, &night_wea_style, 0);
        lv_label_set_long_mode(night_wea_label, LV_LABEL_LONG_SCROLL_CIRCULAR);    
        lv_obj_set_width(night_wea_label, 135);
  

        //室内温度标签绘制
        indoor_tem_label = lv_label_create(btn3);
        lv_obj_set_pos(indoor_tem_label, 140, 5);
        lv_obj_set_style_text_font(indoor_tem_label, &my_chinese_font_24, 0);

        //室内湿度标签绘制
        indoor_hum_label = lv_label_create(btn3);
        lv_obj_set_pos(indoor_hum_label, 140, 35);
        lv_obj_set_style_text_font(indoor_hum_label, &my_chinese_font_24, 0);
    }   

    lv_img_set_src(wea_img, selectWeatherImg(my_weather->code));

    memset(buf,0,sizeof(buf));
    sprintf(buf, "%02d:%02d:%02d", sys_time.hour,sys_time.minute,sys_time.second);
    lv_label_set_text(time_label, buf);
    
    uint8_t buf1[10];
    memset(buf1,0,sizeof(buf1));
    memset(buf,0,sizeof(buf));
    week_turn(sys_time.week,(char *)buf1);
    sprintf(buf, "%04d-%02d-%02d  星期%s", sys_time.year,sys_time.month,sys_time.day,buf1);
    lv_label_set_text(data_label, buf);

    memset(buf,0,sizeof(buf));
    weather_turn(my_weather->code, buf);
    lv_label_set_text(wea_code_label, buf);
    
    memset(buf,0,sizeof(buf));
    sprintf(buf, "%d°",my_weather->tem);
    lv_label_set_text(wea_tem_label, buf);

    memset(buf,0,sizeof(buf));
    memset(buf1,0,sizeof(buf1));
    weather_turn(my_weather->code_day, (char *)buf1);
    sprintf(buf, "白天:%s %d℃ 湿度%d\%%,风速%.2f m/S",buf1,my_weather->high,my_weather->humidity,my_weather->wind_speed);
    lv_label_set_text(day_wea_label, buf);

    memset(buf,0,sizeof(buf));
    memset(buf1,0,sizeof(buf1));
    weather_turn(my_weather->code_night, (char *)buf1);
    sprintf(buf, "夜晚:%s %d℃ 湿度%d\%% %02d:%02d:%02d",buf1,my_weather->low,my_weather->humidity,last_update_time.hour,last_update_time.minute,last_update_time.second);
    lv_label_set_text(night_wea_label, buf);

    memset(buf,0,sizeof(buf));
    sprintf(buf, "室内温度:%d℃",(uint32_t)my_weather->indoor_tem);
    lv_label_set_text(indoor_tem_label, buf);

    memset(buf,0,sizeof(buf));
    sprintf(buf, "室内湿度:%d%%",(uint32_t)my_weather->indoor_hum);
    lv_label_set_text(indoor_hum_label, buf);
}

void show_gif(uint8_t end_flag)
{
    static uint8_t init_flag = 0;
    static lv_obj_t* gif_img;
    static lv_obj_t* btn5;
    if(init_flag == 0){
        gif_img = lv_img_create(lv_scr_act());
        lv_obj_set_pos(gif_img, 0, 0);
        
        btn5 = lv_btn_create(lv_layer_top());
        lv_obj_set_size(btn5, 320, 240);
        lv_obj_set_pos(btn5, 0, 0);
        lv_obj_set_style_bg_color(btn5, lv_palette_main(LV_PALETTE_NONE), 0);

        init_flag = 1;

        LV_IMG_DECLARE(frame_1_1) 
        LV_IMG_DECLARE(frame_2_1) 
        LV_IMG_DECLARE(frame_3_1) 
        LV_IMG_DECLARE(frame_4_1) 
        LV_IMG_DECLARE(frame_5_1) 
        LV_IMG_DECLARE(frame_6_1) 
        LV_IMG_DECLARE(frame_7_1) 
        LV_IMG_DECLARE(frame_8_1) 
    }

    if(end_flag == 1){
        init_flag = 0;

        lv_obj_del(gif_img);
        lv_obj_del(btn5);
    }
    
    static uint8_t cnt = 0;
    switch(cnt++){
        case 0:lv_img_set_src(gif_img,&frame_1_1);break;
        case 1:lv_img_set_src(gif_img,&frame_2_1);break;
        case 2:lv_img_set_src(gif_img,&frame_3_1);break;
        case 3:lv_img_set_src(gif_img,&frame_4_1);break;

        case 4:lv_img_set_src(gif_img,&frame_5_1);break;
        case 5:lv_img_set_src(gif_img,&frame_6_1);break;
        case 6:lv_img_set_src(gif_img,&frame_7_1);break;
        case 7:lv_img_set_src(gif_img,&frame_8_1);cnt = 0;break;
    }
}


/**
 * A clock from a meter
 */
void lv_example_meter_3(uint8_t hour,uint8_t min,uint8_t second,uint8_t end_flag)
{
    static uint8_t init_flag = 0;
    static lv_obj_t * meter;
    static lv_meter_scale_t * scale_min;
    static lv_meter_scale_t * scale_hour;
    static lv_meter_scale_t * scale_second;
    static lv_meter_indicator_t * indic_min;
    static lv_meter_indicator_t * indic_hour;
    static lv_meter_indicator_t * indic_second;
    static lv_obj_t* btn5;
    if(init_flag == 0){
    
        btn5 = lv_btn_create(lv_layer_top());
        lv_obj_set_size(btn5, 320, 240);
        lv_obj_set_pos(btn5, 0, 0);
        lv_obj_set_style_bg_color(btn5, lv_palette_main(LV_PALETTE_NONE), 0);
    
        meter = lv_meter_create(btn5);
        lv_obj_set_size(meter, 220, 220);
        lv_obj_center(meter);

        /*Create a scale for the minutes*/
        /*61 ticks in a 360 degrees range (the last and the first line overlaps)*/
        scale_min = lv_meter_add_scale(meter);
        lv_meter_set_scale_ticks(meter, scale_min, 61, 1, 10, lv_palette_main(LV_PALETTE_BLUE));
        lv_meter_set_scale_range(meter, scale_min, 0, 60, 360, 270);
        
        scale_second = lv_meter_add_scale(meter);
        lv_meter_set_scale_ticks(meter, scale_second, 61, 1, 10, lv_palette_main(LV_PALETTE_GREY));
        lv_meter_set_scale_range(meter, scale_second, 0, 60, 360, 270);

        /*Create another scale for the hours. It's only visual and contains only major ticks*/
        scale_hour = lv_meter_add_scale(meter);
        lv_meter_set_scale_ticks(meter, scale_hour, 12, 0, 0, lv_palette_main(LV_PALETTE_BLUE));               /*12 ticks*/
        lv_meter_set_scale_major_ticks(meter, scale_hour, 1, 2, 20, lv_color_black(), 10);    /*Every tick is major*/
        lv_meter_set_scale_range(meter, scale_hour, 1, 12, 330, 300);       /*[1..12] values in an almost full circle*/

        LV_IMG_DECLARE(img_hand_second)
        LV_IMG_DECLARE(img_hand_min)
        LV_IMG_DECLARE(img_hand_hour)

        /*Add a the hands from images*/
        indic_min = lv_meter_add_needle_img(meter, scale_min, &img_hand_min, 5, 5);
        indic_hour = lv_meter_add_needle_img(meter, scale_hour, &img_hand_hour, 5, 5);
        indic_second = lv_meter_add_needle_img(meter, scale_second, &img_hand_second, 5, 5);
        init_flag = 1;
    }
    if(end_flag == 1){
        init_flag = 0;
        lv_obj_del(meter);
        lv_obj_del(btn5);
    }
    lv_meter_set_indicator_end_value(meter,indic_second,second);
    lv_meter_set_indicator_end_value(meter,indic_min,min);
    lv_meter_set_indicator_end_value(meter,indic_hour,hour);
    

    /*Create an animation to set the value*/
//    lv_anim_t a;
//    lv_anim_init(&a);
//    lv_anim_set_exec_cb(&a, set_value);
//    lv_anim_set_values(&a, 0, 60);
//    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
//    lv_anim_set_time(&a, 2000);     /*2 sec for 1 turn of the minute hand (1 hour)*/
//    lv_anim_set_var(&a, indic_min);
//    lv_anim_start(&a);

//    lv_anim_set_var(&a, indic_hour);
//    lv_anim_set_time(&a, 24000);    /*24 sec for 1 turn of the hour hand*/
//    lv_anim_set_values(&a, 0, 60);
//    lv_anim_start(&a);
}

void rtc_init(uint32_t ms)
{
    hal_misc_awo_set_o_cpu_sleep_counter_bp(0);
    hal_rtc_en(RTC_BASE,HAL_DISABLE);      

    for(int i = 0; i < 10000; i++);
    
    rtc_init_t_def rtc_init;
    rtc_init.rtc_warp_en = RTC_WRAP_EN;       
   
    hal_rtc_init(RTC_BASE,&rtc_init);
    hal_rtc_set_cnt_match(RTC_BASE, ((ms * 1000000.0f) /  hal_misc_awo_get_i_rco32k_period_ns()) + 0.5f  + 1);                         
    hal_rtc_set_cnt_load(RTC_BASE,0);                           
    hal_rtc_it_cfg(RTC_BASE,RTC_IT_FLAG_ACTIVE,HAL_ENABLE);

    
    NVIC_SetPriority(RTC_IRQn,0);
    NVIC_EnableIRQ(RTC_IRQn);
    
    hal_rtc_en(RTC_BASE,HAL_ENABLE);                       
}

void RTC_IRQHandler()
{
    hal_rtc_set_cnt_match(RTC_BASE, ((1000 * 1000000.0f) /  hal_misc_awo_get_i_rco32k_period_ns()) + 0.5f  + 1);
    hal_misc_awo_set_o_cpu_sleep_counter_bp(0);
    hal_rtc_clr_it_flag(RTC_BASE,RTC_IT_FLAG_ACTIVE);
    system_second++;
}

void update_from_ntp(uint32_t second)
{
    ntp_second = second;
    hal_rtc_set_cnt_load(RTC_BASE,0);   
    update_flag = 1;
}

void update_weather(weather_t *weather)
{
    my_weather = weather;
    weather_flag = 1;
}

void ntp_2_utc(uint32_t second,time_t *time)
{
    //NTP时间翻译
    #define YEAR400    (365*4*100 + (4*(100/4 - 1) + 1))
    #define YEAR100    (365*100 + (100/4 - 1))
    #define YEAR004    (365*4 + 1)
    #define YEAR001    365

    int day  = second / (60 * 60 * 24);
    int y400 = day/YEAR400;
    int y100 = (day - y400*YEAR400)/YEAR100;
    int y004 = (day - y400*YEAR400 - y100*YEAR100)/YEAR004;
    int y001 = (day - y400*YEAR400 - y100*YEAR100 - y004*YEAR004) / YEAR001;
    int year = y400*4*100 + y100*100 + y004*4 + y001*1 + 1900;
    int date = (day - y400*YEAR400 - y100*YEAR100 - y004*YEAR004) % YEAR001;
    int n    = 0;
    int month= 0;
    int week = 0;

    
    uint8_t month_day[13] = {0,31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (!(year%1000)){
        if (!(year/1000%4)){
            month_day[2] = 29;
        }
    }else{
        if (!(year%4)){
            month_day[2] = 29;
        }
    }

    for (n=1; n<=12; n++){
        if (date - month_day[n] <= 0){
            break;
        }else{
            date = date - month_day[n];
        }
    }

    date = date;
    month = n;
    
    time->year  = year;
    time->month = month;
    time->day   = date;
    
    if(month < 3)month += 12;
    
    week = (year-2000) + ((year-2000)/4) + (20 / 4) - 20 * 2 + (26 * (month + 1) / 10) + date - 1;
    week = week % 7;

    time->week   = week;
    time->hour   = second % (60 * 60 * 24) / (60 * 60);
    time->minute = second % (60 * 60) / (60);
    time->second = second % (60);

    static uint32_t time_cnt = 0;
    if(time_cnt > 100){
        LOG(LOG_LVL_ERROR, "%d-%d-%d %d %d:%d:%d\n",time->year,time->month,time->day,time->week,time->hour,time->minute,time->second);
        time_cnt = 0;
    }
    time_cnt ++;
}

void lvgl_set_next_state_machine(wifi_state_machine state_machine)
{
    next_state = state_machine;
}

wifi_state_machine lvgl_get_current_state_machine()
{
    return current_state;
}

void lvgl_show_samll_progarm_img(uint8_t free_flag)
{
    static uint8_t init_flag = 0;
    static lv_obj_t* img;
    static lv_obj_t* btn;
    if(init_flag == 0){
        btn = lv_btn_create(lv_layer_top());
        img = lv_img_create(btn);
        init_flag = 1;
    }
    lv_obj_set_size(btn, 320, 240);
    lv_obj_set_pos(btn, 0, 0);
    lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_NONE), 0);

    lv_obj_set_pos(img, 30, 0);
    LV_IMG_DECLARE(small_program) 
    lv_img_set_src(img,&small_program);

    if(free_flag == 1){
        lv_obj_del(img);
        lv_obj_del(btn);
        init_flag = 0;
    }
}

void lvgl_show_process(uint8_t process,char *show_str,uint8_t free_flag)
{
    static uint8_t init_flag = 0;
    static lv_obj_t* bar1;
    static lv_obj_t* label;
    static lv_obj_t* btn5;

    if(init_flag == 0){
        btn5 = lv_btn_create(lv_layer_top());
        lv_obj_set_size(btn5, 320, 240);
        lv_obj_set_pos(btn5, 0, 0);
        lv_obj_set_style_bg_color(btn5, lv_palette_main(LV_PALETTE_NONE), 0);

        static lv_style_t style_bg;
        static lv_style_t style_indic;
        static lv_style_t style_label;

        lv_style_init(&style_bg);
        lv_style_set_border_color(&style_bg, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_set_border_width(&style_bg, 1);
        lv_style_set_pad_all(&style_bg, 3); /*To make the indicator smaller*/

        lv_style_init(&style_indic);
        lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
        lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));

        bar1 = lv_bar_create(btn5);
        lv_obj_set_size(bar1, 200, 10);
        lv_obj_remove_style_all(bar1);  /*To have a clean start*/
        lv_obj_add_style(bar1, &style_bg, 0);
        lv_obj_add_style(bar1, &style_indic, LV_PART_INDICATOR);
        lv_obj_center(bar1);

        label = lv_label_create(btn5); 
        lv_obj_set_style_text_font(label, &my_chinese_font_16, 0);
        lv_style_init(&style_label);
        lv_style_set_text_color(&style_label, lv_palette_main(LV_PALETTE_YELLOW));
        lv_obj_add_style(label, &style_label, 0);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);    
        lv_obj_set_width(label, 250);
        init_flag = 1;
    }

    lv_obj_set_pos(label, lv_obj_get_x(bar1), lv_obj_get_y(bar1) - 18);
    lv_label_set_text(label, show_str);
    lv_bar_set_value(bar1, process, LV_ANIM_OFF);
    
    if(free_flag == 1){
        lv_obj_del(bar1);
        lv_obj_del(label);
        lv_obj_del(btn5);
        init_flag = 0;
    }
}


void lvgl_state_machine(void)
{
    switch(current_state)
    {
        case wifi_sta_init: 
        {
            lvgl_show_process(25,"WIFI STA模式初始化...",0);
            if(next_state != current_state){
                //lvgl_show_process(25,"WIFI STA模式初始化...",1);
                current_state = next_state;
            }
            break;
        }

        case wifi_sta_init_success: 
        {
            lvgl_show_process(75,"WIFI 已连接,查询信息中...",0);
            if(next_state != current_state){
                //lvgl_show_process(75,"WIFI 已连接,查询信息中...",1);
                current_state = next_state;
            }
            break;
        }

        case wifi_get_info_success: 
        {
            lvgl_show_process(100,"连接成功...",0);
            if(next_state != current_state){
                lvgl_show_process(100,"连接成功...",1);
                current_state = next_state;
            }
            break;
        }


        case wifi_sta_init_failed: 
        {
            lvgl_show_process(25,"WIFI 初始化失败,请配网!",0);
            if(next_state != current_state){
                lvgl_show_process(0,"",1);
                current_state = next_state;
            }
            break;
        }

        case wifi_sta_connecting: 
        {
            lvgl_show_process(50,"WIFI 连接中...",0);
            if(next_state != current_state){
                //lvgl_show_process(50,"WIFI 连接中...",1);
                current_state = next_state;
            }
            break;
        }
        
        case wifi_sta_connect_timeout: 
        {
            lvgl_show_process(50,"WIFI 连接失败!请配网!",0);
            if(next_state != current_state){
                lvgl_show_process(50,"WIFI 连接失败!请配网!",1);
                current_state = next_state;
            }
            break;
        }

        case wifi_sta_running: 
        {
            static uint32_t cnt = 0;
            static uint32_t end_flag = 2;
            cnt++;
            if(cnt > 100){
                end_flag++;
                lv_example_meter_3(0,0,0,1);
                //show_gif(1);
                cnt = 0;
            }
            
            if( end_flag == 1){
                lv_example_meter_3(sys_time.hour % 12,sys_time.minute % 60,sys_time.second % 60,0);
            }else if(end_flag == 2 || end_flag == 3 || end_flag == 4){
                lvgl_normal_show();
            // }else if(end_flag == 3){
            //     show_gif(0);
            }else if(end_flag == 5){
                end_flag = 1;
            }
            
            if(next_state != current_state){
                current_state = next_state;
            }
            
            if(update_flag == 1){
                hal_rtc_set_cnt_load(RTC_BASE,0);
                system_second = ntp_second;
                ntp_2_utc(system_second,&sys_time);
                update_flag = 0;
                last_update_time.second = sys_time.second;
                last_update_time.minute = sys_time.minute;
                last_update_time.hour = sys_time.hour;
            } 
            ntp_2_utc(system_second,&sys_time);
            
            break;
        }

        case wifi_ble_net_config:
        {
            lvgl_show_samll_progarm_img(0);
            if(next_state != current_state){
                lvgl_show_samll_progarm_img(1);
                current_state = next_state;
            }
            break;;
        }
        case wifi_ap_init: 
        {
            lvgl_show_process(25,"WIFI 配网模式初始化...请打开手机wifi,连接LN882H_Small_TV热点,浏览器中访问:192.168.4.1",0);
            
            if(next_state != current_state){
                lvgl_show_process(25,"WIFI 配网模式初始化...请打开手机wifi,连接LN882H_Small_TV热点,浏览器中访问:192.168.4.1",1);
                current_state = next_state;
            }
            break;
        }

        case wifi_ap_connect: 
        {
            lvgl_show_process(50,"查询到用户连接...",0);
            if(next_state != current_state){
                lvgl_show_process(50,"查询到用户连接...",1);
                current_state = next_state;
            }
            break;
        }

        case wifi_ap_connected: 
        {
            lvgl_show_process(75,"查询到用户已进入配网服务器...",0);
            if(next_state != current_state){
                lvgl_show_process(75,"查询到用户已进入配网服务器...",1);
                current_state = next_state;
            }
            break;
        }

        case wifi_ap_recv_info_sucessed: 
        {
            lvgl_show_process(100,"成功接收到配网信息...",0);
            if(next_state != current_state){
                lvgl_show_process(100,"成功接收到配网信息...",1);
                current_state = next_state;
            }
            break;
        }

        case wifi_ap_recv_info_failed: 
        {
            lvgl_show_process(75,"接收配网信息失败...",0);
            if(next_state != current_state){
                lvgl_show_process(75,"接收配网信息失败...",1);
                current_state = next_state;
            }
            break;
        }
    }
}

