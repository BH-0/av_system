//
// Created by 10031 on 2022/10/6.
//

#ifndef __MPLAYER_H
#define __MPLAYER_H
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include "picture.h"

#define DEVNAME "/dev/input/event3"

#define KEYDOWN	1
#define KEYUP	0

//文件双向链表节点
typedef struct file_t {
    void *p;    //内存占位用
    char find_name[256]; //文件名
    struct file_t *prev;
    struct file_t *next;
}file_t;

struct file_link{
    file_t *head;
    file_t *tail;
    int nodeNumber;//保存链表结点的数量
};

// 设置音量
int general_volume(int* data);

//文件后缀判断函数
int file_suffix_judgment_pro(char *filename,char *file_suffix);

//创建一条文件双向链表并初始化
//入口参数：目录路径，文件后缀
struct file_link *file_link_init(char *directory_path,char *file_suffix);

//链表遍历并打印
void print_allToFileList(struct file_link *listHead);

//销毁链表
void destroy_file_link(struct file_link *listHead);

#endif //__MPLAYER_H
