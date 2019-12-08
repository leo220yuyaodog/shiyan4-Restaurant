#include "Restaurant.h"
#include "globel.h"


typedef struct dish2
{
    int id;
    char name[22];
    int times;
    float money;

} DISH;
//之后还要排序什么的，最好把菜品读到数组里面。所以定义了一个临时的结构体类型dish2，这个结构体里面没有自引用哦

int cmp(const void *p1, const void *p2)
//用stdlib.h里面的qsort函数必须要定义这个用来比较的函数
//如果返回值小于 0（< 0），那么 p1 所指向元素会被排在p2所指向元素的前面
//如果返回值等于 0（= 0），那么 p1 所指向元素与 p2 所指向元素的顺序不确定
//如果返回值大于 0（> 0），那么 p1 所指向元素会被排在 p2 所指向元素的后面
{
    DISH *a = (DISH*)p1;
    DISH *b = (DISH*)p2;
    if(a->times > b->times)
        return -1;
    else if(a->times < b->times)//这里为了降序排序，当p1<p2时，就返回1，p2就能排在p1前面了
        return 1;
    else
        return 0;
}

void seedishrank()//正片开始
{
    system("cls");
    puts("菜的受欢迎程度如下：（\"*\"代表被点了1次，\"#\"代表被点了50次）");
    puts("--------------------------------");
    int menunumber = 0;

    //开文件，读文件到内存
    FILE *fp;
    fp = fopen("menu", "rb");
    if(fp == NULL)
    {
        puts("文件打开失败");
        system("pause");
        return;
    }
    int tempid = 0;
    char tempchar[22];
    memset(tempchar, 0, sizeof(tempchar));
    int temptimes = 0;
    float money = 0;

    fseek(fp, 0, SEEK_END);
    long end = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if(end == 0)
    {
        puts("\n菜单还是空的\n");
        system("pause");
        return;
    }

    int lenofonrdish = 34;
    menunumber = end / lenofonrdish; //我TM就是聪明！！！二进制文件的好处！！！直接就知道menu文件里面存了多少道菜了
    printf("共有%d道菜\n\n", menunumber);

    DISH dish[menunumber];//声明好数组

    int i = 0;
    int j = 0;
    while (ftell(fp) < end)
    {
        fread(&tempid, sizeof(tempid), 1, fp);
        fread(tempchar, sizeof(tempchar), 1, fp);
        fread(&temptimes, sizeof(temptimes), 1, fp);
        fread(&money, sizeof(money), 1, fp);

        //将读入的东西写入数组
        dish[i].id = tempid;
        strcpy(dish[i].name, tempchar);
        dish[i].times = temptimes;
        dish[i].money = money;

        //顺便就打印出来
        printf("%d.%-20s %4d  ", dish[i].id, dish[i].name, dish[i].times);
        if(dish[i].times > 50)
        {
            printf("%s", "  ");
            int t = dish[i].times / 50;//t为#个数
            int remain = dish[i].times - t * 50;//remain为*个数
            for(j = 0; j < t; j++)
            {
                printf("%s", "#");

            }
            for(j = 0; j < remain; j++)
            {
                printf("%s", "*");

            }
            puts("");
            i++;
        }
        else
        {
            printf("%s", "  ");
            for(j = 0; j < dish[i].times; j++)
            {
                printf("%s", "*");
            }
            puts("");
            i++;
        }

    }
    fclose(fp);

    qsort(dish, menunumber, sizeof(DISH), cmp);//排序

    puts("--------------------------------");

    if(menunumber <= 4)//如果菜的数目小于等于4，就不能打印“前4道菜”了，会很奇怪的
    {
        printf("\n比较受欢迎的%d道菜如下：\n", menunumber);
        puts("--------------------------------");
        for(i = 0; i < menunumber; i++)
        {
            printf("%d.%-20s \n", dish[i].id, dish[i].name);
        }
        puts("--------------------------------");
    }
    else
    {
        puts("\n比较受欢迎的前4道菜如下：");
        puts("--------------------------------");
        int i;
        for(i = 0; i < 4; i++)
        {
            printf("%d.%-20s \n", dish[i].id, dish[i].name);
        }
        puts("--------------------------------");
    }

    puts("");

    system("pause");
}
