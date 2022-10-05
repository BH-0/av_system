#include "menu.h"
//----------------------------------------------------------------
//此文件为菜单文件，所有菜单显示和切换在此处实现
//----------------------------------------------------------------



//图片全屏滑动浏览界面
//入口参数，图库链表某成员，会从此图片开始播放
int pic_slid_show(pic_t **p)
{
    //pic_t *p = gallery->head;
    bmp_t *pic = NULL;
    bmp_t *pic_next = NULL;

    //先显示第一张图
    pic = pic_rebuild_pro((*p)->pic, 800, 480, 0); //图片尺寸转换
    printf("display:%s\n",(*p)->find_name);
    show_bmp(LCD_addr, pic, 0,0);
    while(1)
    {
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
            }else if(x_ts>90 && x_ts<710 && y_ts>35 && y_ts<475)  //全屏预览
            {
                pic_slid_show(&p);

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
                    break;
                case 7: //千千静听
                printf("7\n");
                    break;
                case 8: //图片
                printf("8\n");
                    pic_click_show();
                    return 0;
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