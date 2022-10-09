#include "main.h"
//----------------------------------------------------------------
//此为主程序，所有程序的入口
//----------------------------------------------------------------
int main(int argc, char **argv)
{
    /*硬件初始化*/
    LCD_addr = LCD_init();  //初始化LCD显示
    fd_ts = touch_init("/dev/input/event0");    //初始化触摸屏

    /*字库初始化*/
    Init_Font();

    /*开始菜单按键初始化*/
    start_in = create_list_icon();  //初始化按钮图标管理结构体
    icon_init(start_in,"./menu/start_in.txt"); //导入图标组
    start_out = create_list_icon();  //初始化按钮图标管理结构体
    icon_init(start_out,"./menu/start_out.txt"); //导入图标组

    /*图库初始化*/
    gallery = pic_link_init(); //初始化图库
//    print_allToList(gallery); //遍历链表

    /*其他文件初始化*/
    mp3_file = file_link_init("./mp3_mp4", ".mp3");    //导入MP3文件列表
    mp4_file = file_link_init("./mp3_mp4", ".mp4");    //导入MP4文件列表

    /*杀死次要进程*/
    system("killall -SIGKILL  mplayer");    //杀死进程


    /*开机动画*/
    memset(LCD_addr,'\0',800*480*4);//黑屏
    Display_utf8(WIN_TITLE_X, WIN_TITLE_Y, "开机动画", 0xffffff, 1, 0);
    system("mplayer -geometry 160:0 -zoom -x 480 -y 480 -af -cache 8192 -slave \
                           -input file=/tmp/fifo_cmd_b -quiet ./menu/321.avi");

#if 1 //进入线程
    while (1)
    {
        menu_main();
    }
#endif
    return 0;
}
