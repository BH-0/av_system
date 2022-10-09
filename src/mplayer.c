#include "mplayer.h"
//----------------------------------------------------------------
//此文件封装mplayer播放器功能
//----------------------------------------------------------------

//媒体列表全局变量
struct file_link *mp3_file = NULL;
struct file_link *mp4_file = NULL;



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
