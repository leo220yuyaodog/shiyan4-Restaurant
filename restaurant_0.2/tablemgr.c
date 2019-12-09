#include "Restaurant.h"
#include "globel.h"


int getnewtableid(tableptr currentPtr, int tablenumber) //“智能”算出一个桌子id
//比如说现在的桌子id为：1 2 3 5 6，本函数将会返回4
{
    if (NULL == currentPtr)
    {
        return 1;//如果还没有桌子的话，当然是1了
    }
    else
    {
        if(tablenumber == 0)//算是二次验证真的没有桌子吧
            return 1;
        int n[tablenumber];//建个数组
        int i = 0;
        int ok;
        while (currentPtr != NULL)//遍历桌子链表，把id先存起来
        {
            n[i] = currentPtr->id;
            i++;
            currentPtr = currentPtr->next;
        }
        int biggest = n[0];//假设第1个的id最大
        for(i = 0; i < tablenumber ; i++)
        {
            if(n[i] > biggest)//更新最大值
            {
                biggest = n[i];
            }
        }
        int find = 1;
        while(biggest != find)
        {
            ok = 0;//状态值归0
            for(i = 0; i < tablenumber ; i++)//从id数组里面找
            {
                if(n[i] == find)//如果有了和当前的find相等的，就算了
                {
                    ok = 1;//改变状态值
                }
            }
            if(ok == 0)//说明这个find在id里不存在
                return find;//那这个就是新桌子合适的id了
            else
                find++;//如果存在，就++，然后继续找
        }
        return biggest + 1;

    }
}


void seetable()//直接从文件里读入并打印
{
    int tablenumber = 0;
    {
        //开文件，读文件并打印
        FILE *fp;
        fp = fopen("table", "rb");
        if(fp == NULL)
        {
            puts("文件打开失败！");
            system("pause");
            return;
        }
        int tempid, tempcap, tempstate;

        fseek(fp, 0, SEEK_END);
        long end = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        if(end == 0)
        {
            puts("\n一张桌子都没有呢\n");
            fclose(fp);
            return;
        }
        int num = end / 12;//每个桌子的字节数为12，那就可以算出来要读多少次
        int i = 0;
        while (i < num)
        {
            fread(&tempid, sizeof(int), 1, fp);
            fread(&tempcap, sizeof(int), 1, fp);
            fread(&tempstate, sizeof(int), 1, fp);
            printf("编号：%5d 容量：%5d   ", tempid, tempcap);
            if(tempstate == 1)
                printf("%s", "状态：有客\n");
            else
                printf("%s", "状态：无客\n");
            tablenumber++;
            i++;
        }
        fclose(fp);


    }
    printf("\n共有%d张桌子\n", tablenumber);
}


int readtabletomem(tableptr *tablehead)//从文件读取桌子信息，并存入链表，返回桌子的个数
{
    int tablenumber = 0;
    {
        //开文件，读文件到内存
        FILE *fp;
        fp = fopen("table", "rb");
        if(fp == NULL)
        {
            puts("文件打开失败！");
            system("pause");
            return 0;
        }
        int temp[3];

        fseek(fp, 0, SEEK_END);
        long end = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        if(end == 0)
        {
            return 0;
        }
        while (ftell(fp) < end)
        {
            fread(&temp[0], sizeof(int), 1, fp);
            fread(&temp[1], sizeof(int), 1, fp);
            fread(&temp[2], sizeof(int), 1, fp);
            inserttable(&*tablehead, temp[0], temp[1], temp[2]);
            //printf("编号：%d 容量：%d   状态：%d\n", temp[0], temp[1], temp[2]);
            tablenumber++;
        }
        //puts("NULL");
        fclose(fp);
    }
    return tablenumber;
}


void addtable()//添加桌子
{
    tableptr tablehead = NULL;
    int tablenumber = readtabletomem(&tablehead);//读入内存
    printf("\n%s", "请输入新桌子的容量：");
    char in[11];
    scanf("%9s", in);
    int i;
    for(i = 0; i < strlen(in); i++)
    {
        if(in[0] < 49 || in[0] > 57)
        {
            system("cls");
            puts("输入有误");
            return;
        }
    }


    int cap = atoi(in);

    inserttable(&tablehead, getnewtableid(tablehead, tablenumber), cap, 0); //向链表中链表插入桌子
    writetable(tablehead);//将内存写入文件
    delalltable(&tablehead);//摧毁链表，释放空间
    system("cls");
}
void deltable()//删除桌子
{
    tableptr tablehead = NULL;
    readtabletomem(&tablehead);//从文件读入内存
    if (tablehead != NULL)
    {
        printf("\n%s", "请输入要删除的桌子的编号：");

        int delid = inputselect2();
        if(delid == -1)
        {
            puts("输入出错！");
            system("pause");
            return;
        }
        int s = deletetable(&tablehead, delid);//删除节点，若成功，返回0，若桌子仍被占用，返回-1，若没找到，返回-2
        if (s == 0)
        {
            system("cls");
            printf("%d号桌子已经删除\n", delid);


            writetable(tablehead);//将内存写入文件
            delalltable(&tablehead);//摧毁链表，释放空间
            return;
        }
        else if(s == -1)
        {
            system("cls");
            puts("桌子仍被占用，无法删除");
            delalltable(&tablehead);//摧毁链表，释放空间
            return;
        }
        else if(s == -2)
        {
            system("cls");
            puts("似乎没有这张桌子");
            delalltable(&tablehead);//摧毁链表，释放空间
            return;
        }


    }
    else
    {
        system("cls");
        puts("一张桌子还没有呢");
        delalltable(&tablehead);//摧毁链表，释放空间
        return;
    }

}


void tablemgr()
{
    while (1)
    {
        puts("");
        puts("-------------------");
        puts("目前的餐桌情况为：");
        seetable();
        puts("-------------------");
        puts("操作：");
        puts("1.添加餐桌");
        puts("2.删除餐桌");
        puts("3.返回上一级");
        puts("-------------------");
        printf("\n%s", "请选择（无需按回车）：");
        switch(inputselect1(3))
        {
        case 1:
            addtable();
            break;
        case 2:
            deltable();
            break;
        case 3:
            return;
        case -1:
            system("cls");
            puts("输入错误，请重新输入!");
            break;
        default:
            break;
        }
    }
}
