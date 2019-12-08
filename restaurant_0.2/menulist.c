#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globel.h"

//整个menulist和tablelist非常相似，所以就不多写注释了
void insertmenu(menuptr *sPtr, int id, char name[], int times, float money)
{
    menuptr newPtr = malloc(sizeof(Menu));

    if (newPtr != NULL)
    {
        strcpy(newPtr->name, name);
        newPtr->id = id;
        newPtr->times = times;
        newPtr->money = money;
        newPtr->next = NULL;

        menuptr previousPtr = NULL;
        menuptr currentPtr = *sPtr;

        while (currentPtr != NULL && id > currentPtr->id)
        {
            previousPtr = currentPtr;
            currentPtr = currentPtr->next;
        }

        if (previousPtr == NULL)
        {
            newPtr->next = *sPtr;
            *sPtr = newPtr;
        }
        else
        {
            previousPtr->next = newPtr;
            newPtr->next = currentPtr;
        }
    }
    else
    {
        puts("内存申请失败！");
    }
}


int deletemenu (menuptr *sPtr, int value)//这里改为了，删除成功返回菜品id，没找到则返回0
{
    if (value == (*sPtr)->id)
    {
        menuptr tempPtr = *sPtr;
        *sPtr = (*sPtr)->next;
        free(tempPtr);
        return value;
    }
    else
    {
        menuptr previousPtr = *sPtr;
        menuptr currentPtr = (*sPtr)->next;

        while (currentPtr != NULL && currentPtr->id != value)
        {
            previousPtr = currentPtr;
            currentPtr = currentPtr->next;
        }
        if (currentPtr != NULL)
        {
            menuptr tempPtr = currentPtr;
            previousPtr->next = currentPtr->next;
            free(tempPtr);
            return value;
        }
    }
    return 0;
}



void writemenu(menuptr head)
{
    if (NULL == head)
    {
        //利用w打开方式的性质把文件写空
        FILE *fp;
        fp = fopen("menu", "wb");
        if (fp == NULL)
        {
            puts("文件打开失败");
            system("pause");
            return;
        }
        else
        {
            fclose(fp);
        }
    }
    else
    {
        int num=0;
        FILE *fp;
        fp = fopen("menu", "wb");
        if (fp == NULL)
        {
            puts("文件打开失败");
            system("pause");
            return;
        }
        else
        {
            while (head != NULL)
            {
                fwrite(&head->id, sizeof(head->id), 1, fp);
                fwrite(head->name, sizeof(head->name), 1, fp);
                fwrite(&head->times, sizeof(head->times), 1, fp);
                fwrite(&head->money, sizeof(head->money), 1, fp);
                num++;
                head = head->next;
            }
            fclose(fp);
        }
    }
}


void delallmenu(menuptr *head)          //链表的销毁操作
{
    menuptr q;
    while(*head)
    {
        q = (*head)->next;
        free(*head);
        *head = q;
    }

}



