#ifndef	RESTAURANT_H
#define  RESTAURANT_H
//这里几乎include了所有所学的常用的标准库头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <windows.h>
//以下include了本工程中的自定义头文件
#include "pwd.h"
#include "tablelist.h"
#include "menulist.h"
#include "orderlist.h"


void mainmenu();
void otherfunc();
int inputselect1();
int inputselect2();

void tablemgr();
void menumgr();
void accountmgr();
void addaccount(double money);

void order();
void calculate();
void seedishrank();
#endif

