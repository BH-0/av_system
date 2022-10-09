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
#include "picture.h"
//音量
#include <sys/ioctl.h>
#include <sys/soundcard.h>


#define DEVNAME "/dev/input/event3"

#define KEYDOWN	1
#define KEYUP	0

//媒体列表全局变量
extern struct file_link *mp3_file;
extern struct file_link *mp4_file;

// 设置音量
int general_volume(int* data);



#endif //__MPLAYER_H
