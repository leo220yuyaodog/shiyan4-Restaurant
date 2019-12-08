#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globel.h"

void inserttable(tableptr *sPtr, int id, int capacity, int state)//插入新节点
{
    tableptr newPtr = malloc(sizeof(Table));//申请空间

    if (newPtr != NULL)
    {
        newPtr->capacity = capacity;//赋值
        newPtr->id = id;//赋值
        newPtr->state = state;//赋值
        newPtr->next = NULL;//先把next赋值为NULL

        tableptr previousPtr = NULL;
        tableptr currentPtr = *sPtr;

        while (currentPtr != NULL && capacity > currentPtr->capacity)//找到一个合适的位置，按照capacity排序链表的话，有利于order时候座位
        {
            previousPtr = currentPtr;
            currentPtr = currentPtr->next;
        }

        if (previousPtr == NULL)//当合适的位置是链表的头时，previousPtr确实会等于空，这里相当于是完成了头插
        {
            newPtr->next = *sPtr;
            *sPtr = newPtr;
        }
        else//找到合适位置但不是链表的头
        {
            previousPtr->next = newPtr;
            newPtr->next = currentPtr;
        }
    }
    else
    {
        puts("内存申请失败！");
        system("pause");
    }
}

int deletetable (tableptr *sPtr, int value)//删除节点，若成功，返回0，若桌子仍被占用，返回-1，若没找到，返回-2
{
    if (value == (*sPtr)->id&&(*sPtr)->state==0)//如果第一个节点就是要找的，且桌子空闲
    {
        tableptr tempPtr = *sPtr;
        *sPtr = (*sPtr)->next;
        free(tempPtr);
        return 0;
    }

    else if (value == (*sPtr)->id&&(*sPtr)->state!=0)//如果第一个节点就是要找的，但桌子有人
    {
        return -1;
    }
    else
    {
        tableptr previousPtr = *sPtr;
        tableptr currentPtr = (*sPtr)->next;

        while (currentPtr != NULL && currentPtr->id != value)//向后找，直到找到为止
        {
            previousPtr = currentPtr;
            currentPtr = currentPtr->next;
        }

        if (currentPtr != NULL&&currentPtr->state==0)//找到，且空闲
        {
            tableptr tempPtr = currentPtr;
            previousPtr->next = currentPtr->next;
            free(tempPtr);
            return 0;
        }
        else if(currentPtr != NULL&&currentPtr->state!=0)//找到，但不空闲
        {
            return -1;
        }
    }

    return -2;
}

void writetable(tableptr head)
{
    //如果链表是空的
    if (NULL==head)
    {
        //利用w打开方式的性质把文件写空
        FILE *fp;
        fp = fopen("table", "wb");
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
        FILE *fp;
        fp = fopen("table", "wb");
        if (fp == NULL)
        {
            puts("文件打开失败");
            system("pause");
            return;
        }
        else
        {
            while (head != NULL)//遍历链表，写入文件
            {
                fwrite(&head->id,sizeof(head->id),1,fp);
                fwrite(&head->capacity,sizeof(head->capacity),1,fp);
                fwrite(&head->state,sizeof(head->state),1,fp);
                head = head->next;
            }
            fclose(fp);
        }
    }
}


void delalltable(tableptr *head)          //链表的销毁操作
{
    tableptr q;
    while(*head)//还没删完的话
    {
        q = (*head)->next;//先把当前头节点的下一个赋给临时的q
        free(*head);//删除当前的头节点
        *head = q;//把q赋给*head
    }
}


