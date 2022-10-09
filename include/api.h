//
// Created by 10031 on 2022/10/8.
//

#ifndef __API_H
#define __API_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//随机获取一张图片的网址
char *get_jpg_api();
//将图片下载到某处，入口参数：下载网址，下载路径（包括下载图片文件全名）
//如：/tpm/1.jpg
void download_jpg(char * url, char *path);

#endif //__API_H
