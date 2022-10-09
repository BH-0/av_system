#include "api.h"
//----------------------------------------------------------------
//此为网络api图片获取程序
//----------------------------------------------------------------

//随机获取一张图片的网址
char *get_jpg_api()
{
    system("rm /tmp/random_img.php");    //删除此文件
    system("wget -O /tmp/random_img.php http://img.xjh.me/random_img.php"); //下载带网址的文件
    FILE *f = fopen("/tmp/random_img.php", "r");
    if (f == NULL)
    {
        printf("Error: Couldn't open /tmp/random_img.php?type=bg\n");
        return NULL;
    }
    //解析文件
    static char buf[512];
    int size = fread(buf, 1, sizeof(buf), f);
    if (size == 0)
    {
        printf("fread random_img.php Error\n");
    }
    int i = 0;
    char *ch_buf = NULL;
    for(i=0; i<size;i++)
    {
        if(buf[i] == 'a' && buf[i+1] == 'l' && buf[i+2] == 't' && buf[i+3] == '=')
        {
            ch_buf = buf+i+7; //'i'
            break;
        }
    }
    if(i == size)
        return NULL;

    //终结字符串
    for(;i<size;i++)
    {
        if(buf[i] == '\"')
        {
            buf[i] = '\0';
        }
    }
    printf("%s\n",ch_buf);
    return ch_buf;
}

//将图片下载到某处，入口参数：下载网址，下载路径（包括下载图片文件全名）
//如：/tpm/1.jpg
void download_jpg(char * url, char *path)
{
    char buf[256] = {0};
    sprintf(buf,"wget -O %s %s",path,url);
    system(buf);
}