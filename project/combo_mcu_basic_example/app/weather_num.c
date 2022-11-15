#include "weather_num.h"

//int numx;
//int numy;
//int numw;


//#include "tianqi/t0.h"
//#include "tianqi/t1.h"
//#include "tianqi/t2.h"
//#include "tianqi/t3.h"
//#include "tianqi/t4.h"
//#include "tianqi/t5.h"
//#include "tianqi/t6.h"
//#include "tianqi/t7.h"
//#include "tianqi/t9.h"
//#include "tianqi/t11.h"
//#include "tianqi/t13.h"
//#include "tianqi/t14.h"
//#include "tianqi/t15.h"
//#include "tianqi/t16.h"
//#include "tianqi/t18.h"
//#include "tianqi/t19.h"
//#include "tianqi/t20.h"
//#include "tianqi/t26.h"
//#include "tianqi/t29.h"
//#include "tianqi/t30.h"
//#include "tianqi/t31.h"
//#include "tianqi/t53.h"
//#include "tianqi/t99.h"

//显示天气图标
void printfweather(int numx,int numy,int numw)
{
    //spi_lcd_show_picture_no_dma(0, 0, 50, 50, t0);
    if(numw==00)
    {
        //TJpgDec.drawJpg(numx,numy,t0, sizeof(t0));

        //spi_lcd_show_picture(0, 0, 240, 233, gImage_pic);
    }
    else if(numw==01)
    {
        //TJpgDec.drawJpg(numx,numy,t1, sizeof(t1));
    }
    else if(numw==02)
    {
        //TJpgDec.drawJpg(numx,numy,t2, sizeof(t2));
    }
    else if(numw==03)
    {
        //TJpgDec.drawJpg(numx,numy,t3, sizeof(t3));
    }
    else if(numw==04)
    {
        //TJpgDec.drawJpg(numx,numy,t4, sizeof(t4));
    }
    else if(numw==05)
    {
        //TJpgDec.drawJpg(numx,numy,t5, sizeof(t5));
    }
    else if(numw==06)
    {
        //TJpgDec.drawJpg(numx,numy,t6, sizeof(t6));
    }
    else if(numw==07||numw==8||numw==21||numw==22)
    {
        //TJpgDec.drawJpg(numx,numy,t7, sizeof(t7));
    }
    else if(numw==9||numw==10||numw==23||numw==24)
    {
        //TJpgDec.drawJpg(numx,numy,t9, sizeof(t9));
    }
    else if(numw==11||numw==12||numw==25||numw==301)
    {
        //TJpgDec.drawJpg(numx,numy,t11, sizeof(t11));
    }
    else if(numw==13)
    {
        //TJpgDec.drawJpg(numx,numy,t13, sizeof(t13));
    }
    else if(numw==14||numw==26)
    {
        //TJpgDec.drawJpg(numx,numy,t14, sizeof(t14));
    }
    else if(numw==15||numw==27)
    {
        //TJpgDec.drawJpg(numx,numy,t15, sizeof(t15));
    }
    else if(numw==16||numw==17||numw==28||numw==302)
    {
        //TJpgDec.drawJpg(numx,numy,t16, sizeof(t16));
    }
    else if(numw==18)
    {
        //TJpgDec.drawJpg(numx,numy,t18, sizeof(t18));
    }
    else if(numw==19)
    {
        //TJpgDec.drawJpg(numx,numy,t19, sizeof(t19));
    }
    else if(numw==20)
    {
        //TJpgDec.drawJpg(numx,numy,t20, sizeof(t20));
    }
    else if(numw==29)
    {
        //TJpgDec.drawJpg(numx,numy,t29, sizeof(t29));
    }
    else if(numw==30)
    {
        //TJpgDec.drawJpg(numx,numy,t30, sizeof(t30));
    }
    else if(numw==31)
    {
        //TJpgDec.drawJpg(numx,numy,t31, sizeof(t31));
    }
    else if(numw==53||numw==32||numw==49||numw==54||numw==55||numw==56||numw==57||numw==58)
    {
        //TJpgDec.drawJpg(numx,numy,t53, sizeof(t53));
    }
    else
    {
        //TJpgDec.drawJpg(numx,numy,t99, sizeof(t99));
    }

  
}
