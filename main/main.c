#include "main.h"

int main(int argc, char **argv)
{
    LCD_addr = LCD_init();
    fd_ts = touch_init("/dev/input/event0");

    start_in = create_list_icon();  //初始化按钮图标管理结构体
    icon_init(start_in,"./menu/start_in.txt");

    start_out = create_list_icon();  //初始化按钮图标管理结构体
    icon_init(start_out,"./menu/start_out.txt");

    gallery = pic_link_init(); //初始化图库

    pic_rebuild(NULL,1,2);


#if 0 //进入线程
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
#endif
    return 0;
}
