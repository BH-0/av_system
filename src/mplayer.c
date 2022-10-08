//
// Created by 10031 on 2022/10/6.
//
#include "mplayer.h"
//----------------------------------------------------------------
//此文件封装mplayer播放器功能
//----------------------------------------------------------------

//媒体列表全局变量
struct file_link *mp3_file = NULL;
struct file_link *mp4_file = NULL;

//文件后缀判断函数
int file_suffix_judgment_pro(char *filename,char *file_suffix)
{
    int filename_len = strlen(filename);
    int file_suffix_len = strlen(file_suffix);
    int i = 0;
    if(filename_len<file_suffix_len)
    {
        return -1;
    }
    while(i<=file_suffix_len)
    {
        if(filename[filename_len-i] != file_suffix[file_suffix_len-i])
        {
            return 0;
        }
        i++;
    }
    return 1;
}

//创建节点,创建前先判断文件是否为指定的文件类型
//入口参数：文件路径
file_t *new_file(char *find_name)
{
    file_t *newfile = malloc(sizeof(file_t));
    if(newfile == NULL)
    {
        printf("malloc newfile error\n");
        return NULL;
    }
    newfile->next = NULL;
    newfile->prev = NULL;
    strcpy(newfile->find_name,find_name);

    return newfile;
}

//创建一条文件双向链表并初始化
//入口参数：目录路径，文件后缀
struct file_link *file_link_init(char *directory_path,char *file_suffix)
{
    //创建管理结构体
    struct file_link *listHead = malloc(sizeof(struct file_link));
    if(listHead==NULL)
    {
        printf("malloc listHead error\n");
        return NULL;
    }

    listHead->head = listHead->tail = NULL;
    listHead->nodeNumber = 0;

    //打开目录扫描目录下的文件
    //打开目录
    DIR *dp = opendir(directory_path);
    if(dp == NULL)
    {
        perror("opendir fail");
        free(listHead);
        return NULL;
    }

    //切换到目录里面去
    chdir(directory_path);

    struct dirent *ep = NULL;
    while(1)
    {
        ep = readdir(dp);
        if(ep == NULL)  //说明已经读取到最后一项了
        {
            break;
        }
        if(ep->d_name[0] == '.' || ep->d_type == DT_DIR)  //如果文件名的第一个字符是. 或者遇到了目录
        {
            continue;
        }
        if(file_suffix_judgment_pro(ep->d_name,file_suffix) == 0)    //判读文件类型再加入链表
        {
            continue;
        }

        //新建节点
        file_t *newPic = NULL;
                //创建节点
                newPic = new_file(ep->d_name);
                //有序插入
                insert_list_sort((struct pic_link *)listHead, (pic_t *)newPic);

    }
    chdir("..");
    closedir(dp);

    return listHead;
}
//链表遍历并打印
void print_allToFileList(struct file_link *listHead)
{
//遍历链表，将链表中的每个结点的数据打印出来
    file_t *p = listHead->head;
    int i = 0;
    while(i < listHead->nodeNumber)
    {
        printf("print:%s\n",p->find_name);
        p = p->next;
        i++;
    }
}

//销毁链表
void destroy_file_link(struct file_link *listHead)
{
    if(listHead == NULL)
        return;
    if(listHead->nodeNumber == 0)
    {

        free(listHead);
        return;
    }
    if(listHead->head == NULL)
    {
        free(listHead);
        return;
    }

    file_t *dp = NULL;
    file_t *p = listHead->head;
    int i = 0;

    while(i < listHead->nodeNumber)
    {
        dp = p;
        //printf("--%s\n",p->find_name);
        free(dp);
        p = p->next;
        i++;
    }
    free(listHead);

}

#if 0
/*
 * struct input_event {
	struct timeval time;
	__u16 type;
	__u16 code;
	__s32 value;
};

#define EV_KEY                  0x01
*/


//触发单个按键动作
int reportkey(int fd, uint16_t type, uint16_t keycode, int32_t value)
{
    struct input_event event;

    event.type = type;
    event.code = keycode;
    event.value = value;

    gettimeofday(&event.time, 0);

    if (write(fd, &event, sizeof(struct input_event)) < 0) {
        printf("report key error!\n");
        return -1;
    }

    return 0;
}

int analog_key()
{
    uint16_t keycode;

    int k_fd;

    k_fd = open(DEVNAME, O_RDWR);
    printf("%s\n",DEVNAME);
    if (k_fd < 0) {
        printf("open error!\n");
        return k_fd;
    }

    keycode = KEY_Q;
    reportkey(k_fd, EV_KEY, keycode, KEYDOWN);
    reportkey(k_fd, EV_KEY, keycode, KEYUP);

    close(k_fd);

    return 0;
}
#endif
