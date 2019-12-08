#include "Restaurant.h"
#include "globel.h"

float inputselect3()//检测小数输入，支持5为整数和2位小数
{
    char in[14];
    scanf("%13s", in);
    fflush(stdin);
    if(strlen(in) > 8)//12345.67为8位，所以长度大于8就直接pass
        return -1;
    int i;
    int isfloat = 0;//判断是否位小数的状态变量
    int floatpos = -1;//存的是小数点在字符串里的位置
    for(i = 0; i < strlen(in); i++)
    {
        if(in[i] == '.' && floatpos == -1) //如果找到小数点，且是第一次找到
        {
            isfloat = 1;//是个小数
            floatpos = i;//在第i位是小数点
            continue;
        }
        else if(in[i] == '.' && floatpos != -1) //如果找到小数点，但不是第一次找到
        {
            return -1;//诸如1.123.12这样的输入应是不合法的
        }
        else if(in[i] < 48 || in[i] > 57)//存在不是数字也不是小数点的字符
        {
            return -1;
        }
    }

    //例如12.3456这样的的输入，能过上面的所有检查，这个部分负责把第三位小数及之后的小数去掉
    if(strlen(in) - floatpos > 3 && isfloat == 1) //是小数，且小数点的位置和字符串结尾的位置超过了3，说明小数位数超了
    {
        for(i = strlen(in) - 1; strlen(in) - 1 - floatpos > 2; i--)//把超了的都换成'\0'
        {
            in[i] = '\0';
        }
    }

    if(isfloat == 0)//如果经过一系列判断，但不是小数
    {
        return (float)atoi(in);//强转一下并return
    }
    else
    {
        float out;
        sscanf(in, "%f", &out);//sscanf可以把某个字符串当作输入流再次进行格式化输入
        return out;
    }
}


int getnewmenuid(menuptr currentPtr, int menunumber)//雷同getnewtableid，看注释请移步tablemgr.c
{
    if (NULL == currentPtr)
    {
        return 1;
    }
    else
    {
        if(menunumber == 0)
            return 1;
        int n[menunumber];
        int i = 0;
        int ok;
        while (currentPtr != NULL)
        {
            n[i] = currentPtr->id;
            i++;
            if(i > menunumber)
                break;
            currentPtr = currentPtr->next;
        }
        int biggest = n[0];
        for(i = 0; i < menunumber; i++)
        {

            if(n[i] > biggest)
            {
                biggest = n[i];
            }
        }
        int find = 1;
        while(biggest != find)
        {
            ok = 0;
            for(i = 0; i < menunumber ; i++)
            {
                if(n[i] == find)
                {
                    ok = 1;
                }
            }
            if(ok == 0)
                return find;
            find++;
        }
        return biggest + 1;

    }
}

//剩下的部分和tablemgr非常相似，所以就不多写注释了
void seemenu()
{
    int menunumber = 0;

    //开文件，读文件到内存
    FILE *fp;
    fp = fopen("menu", "rb");
    if(fp == NULL)
    {
        puts("打开文件失败！");
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
        puts("\n一道菜都没有呢\n");
        fclose(fp);
        return;
    }
    puts("");
    while (ftell(fp) < end)
    {
        fread(&tempid, sizeof(tempid), 1, fp);
        fread(tempchar, sizeof(tempchar), 1, fp);
        fread(&temptimes, sizeof(temptimes), 1, fp);
        fread(&money, sizeof(money), 1, fp);

        printf("编号：%3d   菜名：%-22s  单价：%.2f  被点次数: %3d \n", tempid, tempchar, money, temptimes);
        menunumber++;
    }

    printf("\n共有%d道菜\n", menunumber);
    fclose(fp);
}

int readmenutomem(menuptr *menuhead)
{
    int menunumber = 0;

    //开文件，读文件到内存
    FILE *fp;
    fp = fopen("menu", "rb");
    if(fp == NULL)
    {
        puts("文件打开失败！");
        system("pause");
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    long end = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if(end == 0)
    {
        return 0;
    }

    int tempid = 0;
    char tempchar[22];
    memset(tempchar, 0, sizeof(tempchar));
    int temptimes = 0;
    float money = 0;

    while (ftell(fp) < end)
    {
        fread(&tempid, sizeof(tempid), 1, fp);
        fread(tempchar, sizeof(tempchar), 1, fp);
        fread(&temptimes, sizeof(temptimes), 1, fp);
        fread(&money, sizeof(money), 1, fp);

        insertmenu(&*menuhead, tempid, tempchar, temptimes, money);
        menunumber++;

    }

    fclose(fp);

    return menunumber;

}
void addmenu()
{
    menuptr menuhead = NULL;
    int menunumber = readmenutomem(&menuhead);
    printf("\n%s", "请输入新菜品的名字（最多10个汉字）：");
    char in[22];
    scanf("%20s", in);
    fflush(stdin);

    float money;
    printf("\n%s", "请输入新菜品的价格（支持5位整数和2位小数）：");
    money = inputselect3();
    if(money == -1)
    {
        system("cls");
        puts("输入出错");
        return;
    }

    insertmenu(&menuhead, getnewmenuid(menuhead, menunumber), in, 0, money); // insert item in list

    writemenu(menuhead);//将内存写入文件
    system("cls");
    delallmenu(&menuhead);
}
void delmenu()
{
    menuptr menuhead = NULL;
    readmenutomem(&menuhead);
    if (menuhead != NULL)
    {
        printf("\n%s", "请输入要删除的菜品的编号：");
        int delid = inputselect4();
        if(delid == -1)
        {
            puts("输入出错！");
            system("pause");
            return;
        }
        if (deletemenu(&menuhead, delid))
        {

            system("cls");
            printf("%d号菜品已经删除\n", delid);
            writemenu(menuhead);
            delallmenu(&menuhead);
            return;
        }
        else
        {
            system("cls");
            puts("似乎没有这道菜哦");
            delallmenu(&menuhead);
            return;
        }
    }
    else
    {
        system("cls");
        puts("一道菜都没有呢");
        delallmenu(&menuhead);//摧毁链表，释放空间
        return;
    }

}
void menumgr()
{
    while (1)
    {
        puts("");
        puts("-------------------");
        puts("目前的菜单情况为：");
        seemenu();
        puts("-------------------");
        puts("操作：");
        puts("1.添加菜品");
        puts("2.删除菜品");
        puts("3.返回上一级");
        puts("-------------------");
        printf("\n%s", "请选择（无需按回车）：");
        switch(inputselect1(3))
        {
        case 1:
            addmenu();
            break;
        case 2:
            delmenu();
            break;
        case 3:
            return;
        case -1:
            system("cls");
            puts("\n输入错误，请重新输入!");
            break;
        default:
            break;
        }
    }
}
