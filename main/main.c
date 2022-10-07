#include "main.h"

int main(int argc, char **argv)
{
    LCD_addr = LCD_init();
    fd_ts = touch_init("/dev/input/event0");

    Init_Font();    //初始化字库

    start_in = create_list_icon();  //初始化按钮图标管理结构体
    icon_init(start_in,"./menu/start_in.txt"); //导入图标组
    start_out = create_list_icon();  //初始化按钮图标管理结构体
    icon_init(start_out,"./menu/start_out.txt"); //导入图标组

    gallery = pic_link_init(); //初始化图库
//    print_allToList(gallery); //遍历链表


#if 0


    system("killall -SIGKILL  mplayer");    //杀死进程

    system("mkfifo /tmp/fifo_cmd"); //创建管道文件
    //通过管道控制
    //不打印信息并且后台运行
    system("mplayer -af volume=-10 -softvol -softvol-max 80 -cache 8192 -slave -input file=/tmp/fifo_cmd -quiet ./mp3_mp4/Bad_Apple.mp4 &");
    sleep(3);
    system("echo \"volume 40 1\n\" > /tmp/fifo_cmd ");    //40为音量默认值，音量最大80

    sleep(5);

//    system("killall -SIGSTOP mplayer &");   //暂停
//
//    system("killall -SIGCONT mplayer"); //继续
    system("echo \"p\n\" > /tmp/fifo_cmd ");    //暂停
    sleep(3);
    system("echo \"q\n\" > /tmp/fifo_cmd ");    //退出

    printf("finished\n");
#endif

#if 1 //进入线程
    while (1)
    {
        menu_main();
    }
#endif
    return 0;
}
