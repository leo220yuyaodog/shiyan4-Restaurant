#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globel.h"

//链表尾插
void insertorder(orderptr *sPtr,int temptid,int tempid,char *tempname,float tempmoney,int temptimes)
{
    orderptr newPtr = malloc(sizeof(Order));

    if (newPtr != NULL)
    {
        newPtr->id = tempid;
        newPtr->tid = temptid;
        newPtr->money = tempmoney;
        strcpy(newPtr->name,tempname);
        newPtr->times=temptimes;
        newPtr->next = NULL;
        if(*sPtr==NULL)
        {
            *sPtr = newPtr;
        }
        else
        {
            orderptr walk=*sPtr;
            while(walk!=NULL)
            {
                if(walk->next==NULL)
                {
                    walk->next=newPtr;
                    break;
                }
                walk=walk->next;
            }
        }
    }
    else
    {
        puts("内存申请失败！");
    }
}

void deleteorder(orderptr *sPtr, int value)
{
    if (value == (*sPtr)->tid)
    {
        orderptr tempPtr = *sPtr;
        *sPtr = (*sPtr)->next;
        free(tempPtr);
        return;
    }
    else
    {
        orderptr previousPtr = *sPtr;
        orderptr currentPtr = (*sPtr)->next;


        while (currentPtr != NULL && currentPtr->tid != value)
        {
            previousPtr = currentPtr;
            currentPtr = currentPtr->next;
        }


        if (currentPtr != NULL)
        {
            orderptr tempPtr = currentPtr;
            previousPtr->next = currentPtr->next;
            free(tempPtr);


            return;
        }
    }

    return;
}


void writeorder(orderptr head)
{

    if (NULL == head)
    {

        FILE *fp;
        fp = fopen("order", "wb");
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
        fp = fopen("order", "wb");
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
                fwrite(&head->tid, sizeof(int), 1, fp);
                fwrite(&head->id, sizeof(int), 1, fp);
                fwrite(head->name, sizeof(head->name), 1, fp);
                fwrite(&head->money, sizeof(float), 1, fp);
                fwrite(&head->times, sizeof(int), 1, fp);
                head = head->next;
            }
            fclose(fp);
        }
    }
}

void delallorder(orderptr *head)          //链表的销毁操作
{
    orderptr q;
    while(*head)
    {
        q = (*head)->next;
        free(*head);
        *head = q;
    }
}


