#include "menu.h"
//----------------------------------------------------------------
//此文件为菜单文件，所有菜单显示和切换在此处实现
//----------------------------------------------------------------

//桌面
int menu_main()
{
    int x_buf = 0;
    int y_buf = 0;
    bmp_t *bmp[3] = {0};
    bmp[0] = open_bmp("./menu/R-C2.bmp");
    show_bmp(LCD_addr, bmp[0], 0,0); //桌面
    display_icons(LCD_addr, start_in, 0);  //显示所有按钮
    while(1)
    {
        if(get_xy(fd_ts, &x_ts, &y_ts) == 1)    //按下
        {
            x_buf = x_ts;
            y_buf = y_ts;
        }
        if(get_xy(fd_ts, &x_ts, &y_ts) == 0)    //松手
        {
            if(touch_button(start_in, x_buf, y_buf) == 0) //点击开始
            {
                menu_start();
                return 0;
            }
        }
    }
}

//当点击开始时，此函数被调用
int menu_start()
{
    display_icons(LCD_addr, start_out, 0);  //显示所有按钮
    while(1)
    {
        if(get_xy(fd_ts, &x_ts, &y_ts) == 0)    //松手
        {
            switch(touch_button(start_out, x_ts, y_ts))
            {
                case 1: //关机
                printf("1\n");
                    break;
                case 2: //注销
                printf("2\n");
                    break;
                case 3: //用户管理
                printf("3\n");
                    break;
                case 4: //搜索
                printf("4\n");
                    break;
                case 5: //门诊
                printf("5\n");
                    break;
                case 6: //视频
                printf("6\n");
                    break;
                case 7: //千千静听
                printf("7\n");
                    break;
                case 8: //图片
                printf("8\n");
                    bmp_t *bmp[3] = {0};
                    bmp[0] = open_bmp("./menu/win.bmp");
                    show_bmp(LCD_addr, bmp[0], 0,0); //桌面
                    while(1);
                    break;
                case 9: //download
                printf("9\n");
                    break;
                default ://返回上级菜单
                    return -1;
            }
        }
    }
}