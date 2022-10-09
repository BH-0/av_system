#include "menu.h"
//----------------------------------------------------------------
//此文件为菜单文件，所有主要程序功能此处实现
//----------------------------------------------------------------

//用户管理
//int usr_data()
//{
//    bmp_t *bg = NULL;
//    bg = open_bmp("./menu/usr_data.bmp");
//    show_bmp(LCD_addr, bg, 0, 0);    //显示背景
//}

//随机图片
int api_jpg() {
    bmp_t *bg = NULL;
    bg = open_bmp("./menu/download.bmp");
    show_bmp(LCD_addr, bg, 0, 0);    //显示背景
    destroy_bmp_t(bg);  //销毁背景
    int collect_bit = 0;    //收藏标志
    char buf[512];
    bmp_t *jpg_buf = NULL;
    bmp_t *dp = NULL;
    char *url = NULL;
    struct list_icon *api_jpg = create_list_icon();  //初始化按钮图标管理结构体
    icon_init(api_jpg, "./menu/api_jpg.txt"); //导入图标组

    Display_utf8(WIN_TITLE_X, WIN_TITLE_Y, "动漫星城", 0xffffff, 1, 1);  //显示标题
    Display_utf8(250, 220, "图片加载中...", 0x000000, 2, 1);

    //下载第一张图
    while (1) //循环获取，找到符合条件的图片
    {
        url = get_jpg_api();  //获取一张图片的地址
        download_jpg(url, "/tmp/1.jpg"); //下载图片
        dp = open_jpeg("/tmp/1.jpg");
        if (dp != NULL)
            break;
        else
            sleep(0.5);
    }
    jpg_buf = pic_rebuild_pro(dp, 708, 440, BG_COLOR); //图片尺寸转换
    show_bmp(LCD_addr, jpg_buf, 4, 35);    //显示

    //显示当前图片信息
    sprintf(buf, "--在线随机获取二次元图片  %d*%d", dp->width, dp->height);
    Display_utf8(100, 10, buf, 0xffffff, 1, 1);
    destroy_bmp_t(dp);  //销毁
    destroy_bmp_t(jpg_buf);  //销毁
    dp = NULL;
    jpg_buf = NULL;
    while (1)
    {
        if(get_xy(fd_ts, &x_ts, &y_ts) == 0)    //松手

        {
            int touch_icon = touch_button(api_jpg, x_ts, y_ts);
            if (touch_icon == 2) //退出
            {
                break;
            } else if (touch_icon == 3)   //下一张
            {
                display_icons(LCD_addr, api_jpg, 1);  //覆写标题栏
                Display_utf8(250, 220, "图片加载中...", 0x000000, 2, 1);

                display_icons(LCD_addr, api_jpg, 4);//显示收藏按钮

                //下载
                while (1) //循环获取，找到符合条件的图片
                {
                    url = get_jpg_api();  //获取一张图片的地址
                    download_jpg(url, "/tmp/1.jpg"); //下载图片
                    dp = open_jpeg("/tmp/1.jpg");
                    if (dp != NULL)
                        break;
                    else
                        sleep(1);
                }
                jpg_buf = pic_rebuild_pro(dp, 708, 440, BG_COLOR); //图片尺寸转换
                show_bmp(LCD_addr, jpg_buf, 4, 35);    //显示

                //显示当前图片信息
                sprintf(buf, "--在线随机获取二次元图片  %d*%d", dp->width, dp->height);
                Display_utf8(100, 10, buf, 0xffffff, 1, 1);
                destroy_bmp_t(dp);  //销毁
                destroy_bmp_t(jpg_buf);  //销毁
                dp = NULL;
                jpg_buf = NULL;

                collect_bit = 0;

            }else if (touch_icon == 4 && collect_bit == 0)  //收藏
            {
                collect_bit = 1;
                char *name = get_find_name(url);
                sprintf(buf, "cp -v /tmp/1.jpg ./pic/%s",name);  //复制到相册目录下
                system(buf);
                Display_utf8(720, 180, "(已收藏)", 0x000000, 1, 0); //提示已收藏
                //创建节点并有序插入
                sprintf(buf, "./pic/%s",name);
                system(buf);
                //open_jpeg(buf);
                insert_list_sort(gallery,new_pic(open_jpeg(buf),name));
                print_allToList(gallery);   //链表遍历
            }
        }
    }
    del_icon(api_jpg);    //销毁图标
    return 0;
}



//播放页面
//入口参数：0：mp3 1：mp4
int mp3_mp4_player(int mp3_or_mp4)
{
    bmp_t *bg = NULL;
    if(mp3_or_mp4 == 0)
        bg = open_bmp("./menu/player.bmp");
    else
        bg = open_bmp("./menu/player_x.bmp");
    show_bmp(LCD_addr, bg, 0,0);    //显示背景
    unsigned int vol = 40;
    char buf[256]= {0};
    int init = 0;   //初始化标志
    int start_bit = 0; //播放标志
    struct list_icon *player_vol = create_list_icon();  //初始化按钮图标管理结构体
    icon_init(player_vol,"./menu/player_vol.txt"); //导入图标组
    struct list_icon *player = create_list_icon();  //初始化按钮图标管理结构体
    icon_init(player,"./menu/player.txt"); //导入图标组

    file_t *file = NULL;
    if(mp3_or_mp4 == 0)
    {
        Display_utf8(WIN_TITLE_X, WIN_TITLE_Y, "千千静听", 0xffffff, 1, 1);  //显示标题
        file = mp3_file->head;    //当前播放
    }else
    {
        Display_utf8(WIN_TITLE_X, WIN_TITLE_Y, "暴风影音", 0xffffff, 1, 1);  //显示标题
        file = mp4_file->head;    //当前播放
    }



    display_icons(LCD_addr, player_vol, 40);   //显示默认音量

    //显示当前媒体信息
    display_icons(LCD_addr, player, 1);  //覆写标题栏
    sscanf(file->find_name, "%70s", buf);  //限制文件名域宽
    Display_utf8(100,10,buf,0xffffff,1,1);

    while(1) {
        if (get_xy(fd_ts, &x_ts, &y_ts) == 0)    //松手
        {
            int touch_icon = touch_button(player, x_ts, y_ts);
            if (touch_icon == 2) //退出
            {
                if(init == 1)
                {
                    system("echo \"q\n\" > /tmp/fifo_cmd ");    //退出
                    if(mp3_or_mp4 == 0)
                        system("echo \"q\n\" > /tmp/fifo_cmd_b ");    //退出
                    system("killall -SIGKILL  mplayer");    //杀死进程
                    system("rm /tmp/fifo_cmd");    //清空管道
                    if(mp3_or_mp4 == 0)
                        system("rm /tmp/fifo_cmd_b");    //清空管道
                }
                break;
            } else if (touch_icon == 3 || touch_icon == 4)   //播放暂停
            {
                if(init == 0)   //第一次点击播放
                {
                    system("killall -SIGKILL  mplayer");    //杀死进程
                    system("rm /tmp/fifo_cmd");    //清空管道
                    if(mp3_or_mp4 == 0)
                        system("rm /tmp/fifo_cmd_b");    //清空管道
                    system("mkfifo /tmp/fifo_cmd"); //创建管道文件
                    if(mp3_or_mp4 == 0)
                        system("mkfifo /tmp/fifo_cmd_b"); //创建管道文件
                    if(mp3_or_mp4 == 0)
                    {
                        //单曲循环，永不退出
                        sprintf(buf, "mplayer -idle -loop 0 -af volume=-10 -softvol -softvol-max 80 -cache 8192 -slave \
                           -input file=/tmp/fifo_cmd -quiet ./mp3_mp4/%s &", file->find_name);
                        system(buf);
                        //显示音乐可视化
                        system("mplayer -geometry 25:65 -zoom -x 515 -y 333 -idle -loop 0 -af -cache 8192 -slave \
                           -input file=/tmp/fifo_cmd_b -quiet ./menu/Windows_Welcome_music.h264.m4v &");
                    }else
                    {
                        //单曲循环，永不退出
                        sprintf(buf, "mplayer -af volume=-10 -softvol -softvol-max 80 \
                            -geometry 103:64 -zoom -x 594 -y 334 -idle -loop 0 -cache 8192 -slave \
                           -input file=/tmp/fifo_cmd -quiet ./mp3_mp4/%s &", file->find_name);
                        system(buf);
                    }
                    display_icons(LCD_addr, player, 3);   //显示暂停图标
                    sprintf(buf, "echo \"volume %u 1\n\" >> /tmp/fifo_cmd ",vol);
                    system(buf);    //40为音量默认值，音量最大80
                    start_bit = 1;
                    init = 1;
                }else if(start_bit == 0)  //暂停状态继续
                {
                    display_icons(LCD_addr, player, 3);   //显示暂停图标
                    //system("killall -SIGCONT mplayer"); //继续
                    system("echo \"p\n\" >> /tmp/fifo_cmd ");
                    if(mp3_or_mp4 == 0)
                        system("echo \"p\n\" >> /tmp/fifo_cmd_b ");
                    start_bit = 1;
                }else   //播放状态暂停
                {
                    display_icons(LCD_addr, player, 4);   //显示播放图标
                    //system("killall -SIGSTOP mplayer &");   //暂停
                    system("echo \"p\n\" >> /tmp/fifo_cmd ");
                    if(mp3_or_mp4 == 0)
                        system("echo \"p\n\" >> /tmp/fifo_cmd_b ");
                    start_bit = 0;
                }


            } else if (touch_icon == 7 && init == 1)   //vol------------
            {
                if(start_bit == 0)
                {
                    display_icons(LCD_addr, player, 3);   //显示暂停图标
                    if(mp3_or_mp4 == 0)
                        system("echo \"p\n\" >> /tmp/fifo_cmd_b "); //让可视化音频继续播放
                }
                start_bit = 1;
                vol = (vol-8)<0? 0 : (vol-8);
                if(vol == 0)
                {
                    display_icons(LCD_addr, player_vol, 1);   //显示音量
                }else
                {
                    display_icons(LCD_addr, player_vol, vol);   //显示音量
                }
                sprintf(buf, "echo \"volume %u 1\n\" >> /tmp/fifo_cmd ",vol);
                system(buf);    //40为音量默认值，音量最大80
            } else if (touch_icon == 8 && init == 1)  //vol++++++++++
            {
                if(start_bit == 0)
                {
                    display_icons(LCD_addr, player, 3);   //显示暂停图标
                    if(mp3_or_mp4 == 0)
                        system("echo \"p\n\" >> /tmp/fifo_cmd_b "); //让可视化音频继续播放
                }
                start_bit = 1;
                vol = (vol+8)>80? 80 : (vol+8);
                display_icons(LCD_addr, player_vol, vol);   //显示音量
                sprintf(buf, "echo \"volume %u 1\n\" >> /tmp/fifo_cmd ",vol);
                system(buf);    //40为音量默认值，音量最大80
            } else if (touch_icon == 9 && init == 1)   //快退
            {
                if(start_bit == 0)
                {
                    display_icons(LCD_addr, player, 3);   //显示暂停图标
                    if(mp3_or_mp4 == 0)
                        system("echo \"p\n\" >> /tmp/fifo_cmd_b "); //让可视化音频继续播放
                }
                sprintf(buf, "echo \"seek %d\n\" >> /tmp/fifo_cmd ",-10);
                system(buf);
                start_bit = 1;

            } else if (touch_icon == 10 && init == 1)  //快进
            {
                if(start_bit == 0)
                {
                    display_icons(LCD_addr, player, 3);   //显示暂停图标
                    if(mp3_or_mp4 == 0)
                        system("echo \"p\n\" >> /tmp/fifo_cmd_b "); //让可视化音频继续播放
                }
                sprintf(buf, "echo \"seek %d\n\" >> /tmp/fifo_cmd ",10);
                system(buf);
                start_bit = 1;

            } else if (touch_icon == 5)   //上一首
            {
                file = file->prev;

                system("echo \"q\n\" > /tmp/fifo_cmd ");    //退出
                if(mp3_or_mp4 == 0)
                    system("echo \"q\n\" > /tmp/fifo_cmd_b ");    //退出
                system("killall -SIGKILL  mplayer");    //杀死进程
                system("rm /tmp/fifo_cmd");    //清空管道
                if(mp3_or_mp4 == 0)
                    system("rm /tmp/fifo_cmd_b");    //清空管道
                system("mkfifo /tmp/fifo_cmd"); //创建管道文件
                if(mp3_or_mp4 == 0)
                    system("mkfifo /tmp/fifo_cmd_b"); //创建管道文件
                if(mp3_or_mp4 == 0)
                {
                    //单曲循环，永不退出
                    sprintf(buf, "mplayer -idle -loop 0 -af volume=-10 -softvol -softvol-max 80 -cache 8192 -slave \
                       -input file=/tmp/fifo_cmd -quiet ./mp3_mp4/%s &", file->find_name);
                    system(buf);
                    //显示音乐可视化
                    system("mplayer -geometry 25:65 -zoom -x 515 -y 333 -idle -loop 0 -af -cache 8192 -slave \
                           -input file=/tmp/fifo_cmd_b -quiet ./menu/Windows_Welcome_music.h264.m4v &");
                }else
                {
                    //单曲循环，永不退出
                    sprintf(buf, "mplayer -af volume=-10 -softvol -softvol-max 80 \
                            -geometry 103:64 -zoom -x 594 -y 334 -idle -loop 0 -cache 8192 -slave \
                           -input file=/tmp/fifo_cmd -quiet ./mp3_mp4/%s &", file->find_name);
                    system(buf);
                }
                display_icons(LCD_addr, player, 3);   //显示暂停图标
                sprintf(buf, "echo \"volume %u 1\n\" >> /tmp/fifo_cmd ",vol);
                system(buf);    //40为音量默认值，音量最大80
                start_bit = 1;
                init = 1;

                //显示当前图片信息
                display_icons(LCD_addr, player, 1);  //覆写标题栏
                sscanf(file->find_name, "%70s", buf);  //限制文件名域宽
                Display_utf8(100,10,buf,0xffffff,1,1);


            } else if (touch_icon == 6)  //下一首
            {
                file = file->next;
                system("echo \"q\n\" > /tmp/fifo_cmd ");    //退出
                if(mp3_or_mp4 == 0)
                    system("echo \"q\n\" > /tmp/fifo_cmd_b ");    //退出
                system("killall -SIGKILL  mplayer");    //杀死进程
                system("rm /tmp/fifo_cmd");    //清空管道
                if(mp3_or_mp4 == 0)
                    system("rm /tmp/fifo_cmd_b");    //清空管道
                system("mkfifo /tmp/fifo_cmd"); //创建管道文件
                if(mp3_or_mp4 == 0)
                    system("mkfifo /tmp/fifo_cmd_b"); //创建管道文件
                if(mp3_or_mp4 == 0)
                {
                    //单曲循环，永不退出
                    sprintf(buf, "mplayer -idle -loop 0 -af volume=-10 -softvol -softvol-max 80 -cache 8192 -slave \
                       -input file=/tmp/fifo_cmd -quiet ./mp3_mp4/%s &", file->find_name);
                    system(buf);
                    //显示音乐可视化
                    system("mplayer -geometry 25:65 -zoom -x 515 -y 333 -idle -loop 0 -af -cache 8192 -slave \
                           -input file=/tmp/fifo_cmd_b -quiet ./menu/Windows_Welcome_music.h264.m4v &");
                }else
                {
                    //单曲循环，永不退出
                    sprintf(buf, "mplayer -af volume=-10 -softvol -softvol-max 80 \
                            -geometry 103:64 -zoom -x 594 -y 334 -idle -loop 0 -cache 8192 -slave \
                           -input file=/tmp/fifo_cmd -quiet ./mp3_mp4/%s &", file->find_name);
                    system(buf);
                }
                display_icons(LCD_addr, player, 3);   //显示暂停图标
                sprintf(buf, "echo \"volume %u 1\n\" >> /tmp/fifo_cmd ",vol);
                system(buf);    //40为音量默认值，音量最大80
                start_bit = 1;
                init = 1;

                //显示当前图片信息
                display_icons(LCD_addr, player, 1);  //覆写标题栏
                sscanf(file->find_name, "%70s", buf);  //限制文件名域宽
                Display_utf8(100,10,buf,0xffffff,1,1);

            }
        }
    }

    destroy_bmp_t(bg);  //销毁背景
    del_icon(player_vol);    //销毁图标
    del_icon(player);    //销毁图标
    //destroy_file_link(mp3_file); //销毁mp3_file
    return 0;
}



//图片全屏滑动浏览界面
//入口参数，p:图库链表某成员，会从此图片开始播放，auto_cmd:0手动滑动 ，非0自动播放延时秒数
int pic_slid_show(pic_t **p, float auto_sec)
{
    //pic_t *p = gallery->head;
    bmp_t *pic = NULL;
    bmp_t *pic_next = NULL;
    int num = 0;
    int sum = 0;
    //先显示第一张图
    pic = pic_rebuild_pro((*p)->pic, 800, 480, 0); //图片尺寸转换
    printf("display:%s\n",(*p)->find_name);
    show_bmp(LCD_addr, pic, 0,0);

    while(1)
    {
        if(auto_sec !=0)    //自动播放
        {
//            if(sum == 0)
//                *p = gallery->head; //从第一张开始播放
            while(1)
            {
                sum++;
                sleep(auto_sec);
                if(sum>=gallery->nodeNumber)
                {
                    destroy_bmp_t(pic);
                    return -1; //返回上级菜单
                }
                if (num == 0)
                {
                    num = 1;
                    goto next0;
                } else if (num == 1)
                {
                    num = 0;
                    goto next1;
                }
            }
        }

        switch(get_xy_plus(fd_ts, &x_ts, &y_ts))
        {
            case up:
                (*p) = (*p)->prev;
                pic_next = pic_rebuild_pro((*p)->pic, 800, 480, 0);   //图片尺寸转换
                printf("display:%s\n",(*p)->find_name);

                //显示动画
                show_bmp_plus(LCD_addr, (char (*)[800*3])pic->bmp_buf, \
                    (char (*)[800*3])pic_next->bmp_buf, 0.5, up);

                destroy_bmp_t(pic); //销毁上一张图片
                pic=pic_next;
                break;
            case down:
            next1:
                (*p) = (*p)->next;
                pic_next = pic_rebuild_pro((*p)->pic, 800, 480, 0);   //图片尺寸转换
                printf("display:%s\n",(*p)->find_name);

                //显示动画
                show_bmp_plus(LCD_addr, (char (*)[800*3])pic->bmp_buf, \
                    (char (*)[800*3])pic_next->bmp_buf, 0.5, down);

                destroy_bmp_t(pic); //销毁上一张图片
                pic=pic_next;
                break;
            case left:
                (*p) = (*p)->prev;
                pic_next = pic_rebuild_pro((*p)->pic, 800, 480, 0);   //图片尺寸转换
                printf("display:%s\n",(*p)->find_name);

                //显示动画
                show_bmp_plus(LCD_addr, (char (*)[800*3])pic->bmp_buf, \
                    (char (*)[800*3])pic_next->bmp_buf, 0.5, left);

                destroy_bmp_t(pic); //销毁上一张图片
                pic=pic_next;
                break;
            case right:
            next0:
                (*p) = (*p)->next;
                pic_next = pic_rebuild_pro((*p)->pic, 800, 480, 0);   //图片尺寸转换
                printf("display:%s\n",(*p)->find_name);

                //显示动画
                show_bmp_plus(LCD_addr, (char (*)[800*3])pic->bmp_buf, \
                    (char (*)[800*3])pic_next->bmp_buf, 0.5, right);

                destroy_bmp_t(pic); //销毁上一张图片
                pic=pic_next;
                break;
            case click:
                destroy_bmp_t(pic);
                return -1; //返回上级菜单
                break;
        }
    }
    return 0;
}

//图片窗口化点击浏览界面
int pic_click_show()
{
    pic_t *p = gallery->head;  //获取图库头指针
    bmp_t *pic = NULL;

    bmp_t *bg = open_bmp("./menu/win.bmp");
    show_bmp(LCD_addr, bg, 0,0);    //显示背景


    struct list_icon *pic_click = create_list_icon();  //初始化按钮图标管理结构体
    icon_init(pic_click,"./menu/pic_click_show.txt"); //导入图标组


    Display_utf8(WIN_TITLE_X,WIN_TITLE_Y,"图库预览",0xffffff,1,1);  //显示标题

    //先显示第一张图
    pic = pic_rebuild_pro(p->pic, 620, 440, BG_COLOR); //图片尺寸转换
    printf("display:%s\n",p->find_name);
    show_bmp(LCD_addr, pic, 90,35);

    //显示当前图片信息
    char buf[128]= {0};
    sscanf(p->find_name, "%70s", buf);  //限制文件名域宽
    sprintf(buf, "%s  %d*%d",buf,p->pic->width,p->pic->height);
    Display_utf8(100,10,buf,0xffffff,1,1);

    while(1)
    {
        if(get_xy(fd_ts, &x_ts, &y_ts) == 0)    //松手
        {
            int touch_icon = touch_button(pic_click, x_ts, y_ts);
            if(touch_icon == 2) //退出
            {
                break;
            }else if(touch_icon == 3)   //下一张
            {
                destroy_bmp_t(pic); //销毁上一张图片
                p = p->next;
                pic = pic_rebuild_pro(p->pic, 620, 440, BG_COLOR);   //图片尺寸转换
                show_bmp(LCD_addr, pic, 90,35);

                //显示当前图片信息
                display_icons(LCD_addr, pic_click, 1);  //覆写标题栏
                char buf[128]= {0};
                sscanf(p->find_name, "%70s", buf);  //限制文件名域宽
                sprintf(buf, "%s  %d*%d",buf,p->pic->width,p->pic->height);
                Display_utf8(100,10,buf,0xffffff,1,1);

            }else if(touch_icon == 4)   //上一张
            {
                destroy_bmp_t(pic); //销毁上一张图片
                p = p->prev;
                pic = pic_rebuild_pro(p->pic, 620, 440, BG_COLOR);   //图片尺寸转换
                show_bmp(LCD_addr, pic, 90,35);

                //显示当前图片信息
                display_icons(LCD_addr, pic_click, 1);  //覆写标题栏
                char buf[128]= {0};
                sscanf(p->find_name, "%70s", buf);  //限制文件名域宽
                sprintf(buf, "%s  %d*%d",buf,p->pic->width,p->pic->height);
                Display_utf8(100,10,buf,0xffffff,1,1);
            }else if((x_ts>90 && x_ts<710 && y_ts>35 && y_ts<475) || touch_icon == 5)  //全屏预览
            {
                if(touch_icon == 5)
                    pic_slid_show(&p,2);  //自动播放
                else
                    pic_slid_show(&p,0);

                //退出后重新显示
                show_bmp(LCD_addr, bg, 0,0);    //显示背景
                display_icons(LCD_addr, pic_click, 0);  //显示所有按钮
                Display_utf8(WIN_TITLE_X,WIN_TITLE_Y,"图库预览",0xffffff,1,1);  //显示标题

                //先显示第一张图
                pic = pic_rebuild_pro(p->pic, 620, 440, BG_COLOR); //图片尺寸转换
                printf("display:%s\n",p->find_name);
                show_bmp(LCD_addr, pic, 90,35);

                //显示当前图片信息
                char buf[128]= {0};
                sscanf(p->find_name, "%70s", buf);  //限制文件名域宽
                sprintf(buf, "%s  %d*%d",buf,p->pic->width,p->pic->height);
                Display_utf8(100,10,buf,0xffffff,1,1);
            }
        }
    }
    destroy_bmp_t(pic); //销毁预览图
    destroy_bmp_t(bg);  //销毁背景
    del_icon(pic_click);    //销毁图标
    return 0;
}


//当点击开始按钮时，此函数被调用
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
                    mp3_mp4_player(1);
                    return 0;
                case 7: //千千静听
                printf("7\n");
                    mp3_mp4_player(0);
                    return 0;
                case 8: //图片
                printf("8\n");
                    pic_click_show();
                    return 0;
                case 9: //download
                printf("9\n");
                    api_jpg();
                    return 0;
                default ://返回上级菜单
                    return -1;
            }
        }
    }
}

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