#include "picture.h"

//----------------------------------------------------------------
//图库处理程序
//----------------------------------------------------------------
//进入图库程序后自动扫描文件夹下所有文件
//先不考虑下载，先实现本地图库预览

//图库管理结构体全局变量
struct pic_link *gallery;

//任意尺寸重建图片，黑底
//可将小图片等比例变大或大图片等比例缩小,注意及时free返回指针
//入口参数：图片管理结构体，大小
bmp_t *pic_rebuild(bmp_t *pic, unsigned int width, unsigned int height)
{
    int wh_cmd = 0; //0：宽贴边，1：高贴边
    float w_h = (float)width/(float)height;
    printf("w/h:%f\n",w_h);
}


//文件名排序前比较算法
//入口参数：对比对象，与被对比对象
//返回值：大于diff_name返回0，小于diff_name返回1，同名返回-1
//排序是大的放后面，小的放前面，有序插入时直接遍历找到第一个比他大的，并插在他前面
int find_name_diff(char *object_name, char *diff_name)
{
    int i = 0;
    //先跳过前缀相同的部分
    while(object_name[i] == diff_name[i] && object_name[i] != '\0' && diff_name[i] != '\0' && i < 255)
        i++;
    if(object_name[i] > diff_name[i])
        return 0;
    else if(object_name[i] < diff_name[i])
        return 1;
    else
        return -1; //名称全等
}

//读取某文件的文件名
//入口参数：文件路径
char *get_find_name(char *find_path)
{
    static char path_buf[512] = {0};
    strcpy(path_buf, find_path);
    char *ptr = NULL;
    char *p = NULL;
    p = strtok(path_buf,"/");
    if (p)
        ptr = p;
    while (p)
    {
        p = strtok(NULL,"/");
        if(p)
            ptr = p;
    }
    return ptr;
}

/*
//目录扫描并更新图片链表
struct pic_link * pic_link_refresh(struct pic_link *listHead)
{
    return NULL;
}
*/

//识别图片文件类型
int pic_find_identify(char *path)
{
    //读取前两字节，判断为bmp图片还是jpeg图片
    //bmp = 0x4d42  jpeg = 0xd8ff
    FILE *fp = fopen(path, "r+");
    if(fp == NULL)
    {
        perror("fopen fail");
        return -1;
    }
    unsigned short buf = 0;
    fread(&buf, sizeof(buf), 1, fp);
    fclose(fp);

    if(buf == BMP_TYPE) //识别到bmp图片
        return BMP_TYPE;
    else if(buf == JPEG_TYPE) //识别到jpeg图片
        return JPEG_TYPE;
    else
    {
        printf("I don't know!\n");
        return -1;
    }
}

//新建节点
pic_t *new_pic(bmp_t *pic, char *find_name)
{
//1.新建一个结点（申请一个结构体的内存空间）
    pic_t *newPic = malloc(sizeof(pic_t));
    if(newPic == NULL)
    {
        printf("malloc newPic error\n");
        return NULL;
    }
//2、初始化新建结点
    newPic->pic = pic;
    newPic->prev = NULL;
    newPic->next = NULL;
    memset(newPic->find_name,'\0',256);
    strcpy(newPic->find_name,find_name);

    return newPic;
}

//有序插入
void insert_list_sort(struct pic_link *listHead, pic_t *newPic)
{
    pic_t *p = listHead->head;
    int i = 0;
    //将节点有序插入到链表中
    if(listHead->head==NULL)    //从无到有
    {
        listHead->head = listHead->tail = newPic;
        newPic->prev = newPic;    //循环链表
        newPic->next = newPic;
    }else //从少到多
    {
        //找到第一个比他大的成员并插到他前面
        while(i < listHead->nodeNumber)
        {
            if(find_name_diff(p->find_name, newPic->find_name) == 0) //p比newPic大
                break;
            p = p->next;
            i++;
        }
        if(i >= listHead->nodeNumber) //没找到就尾插
        {
            newPic->next = listHead->head;
            newPic->prev = listHead->tail;
            listHead->head->prev = newPic;
            listHead->tail->next = newPic;
            listHead->tail = newPic; //更新尾节点
        }else //插在p前面
        {
            newPic->next = p;
            newPic->prev = p->prev;
            p->prev->next = newPic;
            p->prev = newPic;
            if(p == listHead->head)
            {
                listHead->head = newPic;    //如果是头插就更新头节点
            }
        }

    }
    //链表的结点数加1
    listHead->nodeNumber++;
}

//创建一条图片双向链表并初始化
struct pic_link *pic_link_init(void)
{
    //创建管理结构体
    struct pic_link *listHead = malloc(sizeof(struct pic_link));
    if(listHead==NULL)
    {
        printf("malloc listHead error\n");
        return NULL;
    }

    listHead->head = listHead->tail = NULL;
    listHead->nodeNumber = 0;

    //打开目录扫描目录下的文件
    //打开目录
    DIR *dp = opendir("./pic");
    if(dp == NULL)
    {
        perror("opendir fail");
        free(listHead);
        return NULL;
    }

    //切换到目录里面去
    chdir("./pic");

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

        //新建节点
        pic_t *newPic = NULL;
        switch(pic_find_identify(ep->d_name)) //读取前两字节，判断为bmp图片还是jpeg图片
        {
            case BMP_TYPE:
                //创建节点
                newPic = new_pic(open_bmp(ep->d_name), ep->d_name);
                //有序插入
                insert_list_sort(listHead, newPic);
                break;
            case JPEG_TYPE:
                //创建节点
                newPic = new_pic(open_jpeg(ep->d_name), ep->d_name);
                //有序插入
                insert_list_sort(listHead, newPic);
                break;
        }
    }
    chdir("..");
    closedir(dp);

    return listHead;
}

//链表遍历并打印
void print_allToList(struct pic_link *listHead)
{
//遍历链表，将链表中的每个结点的数据打印出来
    pic_t *p = listHead->head;
    int i = 0;
    while(i < listHead->nodeNumber)
    {
        printf("%s\n",p->find_name);
        p = p->next;
        i++;
    }
}
