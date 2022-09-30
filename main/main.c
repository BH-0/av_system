#include "main.h"

int main(int argc, char **argv)
{
    LCD_addr = LCD_init();
    fd_ts = touch_init("/dev/input/event0");

    start_in = create_list_icon();  //初始化按钮图标管理结构体
    icon_init(start_in,"./menu/start_in.txt");

    start_out = create_list_icon();  //初始化按钮图标管理结构体
    icon_init(start_out,"./menu/start_out.txt");

//    bmp_t *bmp[3] = {0};
//    bmp[0] = open_bmp("./menu/R-C2.bmp");
//    show_bmp(LCD_addr,bmp[0],0,0); //桌面

    while (1)
    {
        menu_main();
//        show_bmp(LCD_addr,bmp[0],0,0); //桌面
//        display_icons(LCD_addr, start_in, 0);  //显示所有按钮
//
//        sleep(1);
//
//        display_icons(LCD_addr, start_out, 0);  //显示所有按钮
//        sleep(1);
    }
    return 0;
}
