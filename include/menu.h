//
// Created by 10031 on 2022/9/30.
//

#ifndef __MENU_H
#define __MENU_H
#include <stdio.h>
#include "lcd.h"
#include "bmp.h"
#include "touch.h"
#include "icon.h"
#include <stdlib.h>
#include "picture.h"


//当点击开始时，此函数被调用
int menu_start();

//桌面
int menu_main();

#endif //__MENU_H
