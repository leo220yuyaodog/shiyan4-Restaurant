#include "Restaurant.h"

void addaccount(double money)//向account文件中追写历史记录，传入总账的值
{
    //这部分是获取时间，并将时间转成字符串
    time_t rawtime;
    struct tm *timeinfo;
    time( &rawtime );
    timeinfo = localtime( &rawtime );
    char *timec = asctime(timeinfo);

    //开文件
    FILE *fp;
    fp = fopen("account", "a");//追写就可
    if(fp == NULL)
    {
        puts("文件打开失败");
        system("pause");
        return;
    }
    else
    {
        fprintf(fp, "%lf ", money);//总账写入文件
        fprintf(fp, "%s\n", timec);//时间写入文件
    }
    fclose(fp);
    puts("\n已写入账目历史");
}

void readfromfileaccount()//从account文件里面读取历史记录并打印
{
    FILE *fp;
    fp = fopen("account", "r");
    if(fp == NULL)
    {
        puts("文件打开失败");
        system("pause");
        return;
    }
    else
    {
        double money = 0;
        char timec[30];
        memset(timec, 0, sizeof(timec));
        while(fscanf(fp, "%lf", &money) != EOF)//边读边打印
        {
            fscanf(fp, "%[^\n]", timec);
            printf("%.2lf ", money);
            printf("%s\n\n", timec);
        }
        if(money == 0)
        {
            puts("账目还是空的……");
        }

    }
    fclose(fp);
}

void accountmgr()
{
    puts("");
    puts("账目如下：");
    puts("-------------------");
    readfromfileaccount();
    puts("-------------------");
    puts("");
    system("pause");
}



