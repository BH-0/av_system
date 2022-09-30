#include "menu.h"
//----------------------------------------------------------------
//此文件为菜单文件，所有菜单显示和切换在此处实现
//----------------------------------------------------------------

//桌面
int menu_main()
{
    bmp_t *bmp[3] = {0};
    bmp[0] = open_bmp("./menu/R-C2.bmp");
    show_bmp(LCD_addr, bmp[0], 0,0); //桌面
    display_icons(LCD_addr, start_in, 0);  //显示所有按钮
    while(1)
    {

    }
}

//当点击开始时，此函数被调用
int menu_start()
{
    display_icons(LCD_addr, start_out, 0);  //显示所有按钮
    while(1)
    {
//        get_xy(LCD_addr,);
    }
}