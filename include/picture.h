//
// Created by 10031 on 2022/9/30.
//

#ifndef __PICTURE_H
#define __PICTURE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*open*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
/*close read write*/
#include <unistd.h>
/*mmap*/
#include <sys/mman.h>
#include <stdlib.h>
#include <math.h>   //编译时需要链接数学库 -lm
#include "lcd.h"
#include "bmp.h"

#define BMP_TYPE 0x4d42
#define JPEG_TYPE 0xd8ff

//双向链表成员节点
typedef struct pic_t{
    bmp_t *pic;
    char find_name[256];    //图片文件名
    struct pic_t *prev; //存储是上一个结点的地址
    struct pic_t *next; //存储是下一个结点的地址
}pic_t;

//双向链表管理结构体
struct pic_link{
    struct pic_t *head;//保存链表的数据首结点的位置
    struct pic_t *tail;//保存链表的数据尾结点的位置
    int nodeNumber;//保存链表结点的数量
};

//文件名排序前比较算法
//入口参数：对比对象，与被对比对象
//返回值：大于diff_name返回0，小于diff_name返回1，同名返回-1
//排序是大的放后面，小的放前面，有序插入时直接遍历找到第一个比他大的，并插在他前面
int find_name_diff(char *object_name, char *diff_name);

//读取某文件的文件名
//入口参数：文件路径
char *get_find_name(char *find_path);

//识别图片文件类型
int pic_find_identify(char *path);

//新建节点
pic_t *new_pic(bmp_t *pic, char *find_name);

//有序插入
void insert_list_sort(struct pic_link *listHead, pic_t *newPic);

//创建一条图片双向链表并初始化
struct pic_link *pic_link_init(void);

//链表遍历
void print_allToList(struct pic_link *listHead);

#endif //__PICTURE_H
