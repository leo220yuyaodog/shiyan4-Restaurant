#include "Restaurant.h"
#include "globel.h"

typedef struct tempmenu//后面点菜的时候会线把menu文件里面的东西读到这中tempmenu类型的结构体数组里面，所以先声明好这个结构体类型
{
    int tid;//在点菜之前，肯定已经得到了桌号，存到这里
    int id;//菜品编号
    char name[22];//菜名
    float money;//单价
    int times;//在本桌被点的次数
} TMENU;


void changetablestate_infile(int seeid)//更改桌子状态的函数，传入的是这个桌子的id
{
    //开文件
    FILE *fp;
    fp = fopen("table", "rb+");
    if(fp == NULL)
    {
        puts("文件打开失败！");
        system("pause");
        return;
    }
    fseek(fp, 0, SEEK_END);
    long end = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    int id;
    int state=1;
    while(ftell(fp)<end)
    {
        fread(&id,sizeof(int),1,fp);//读入桌子的id
        if(id==seeid)//如果桌子的id就是要找的
        {
            fseek(fp,4,SEEK_CUR);//跳过capacity
            fwrite(&state,sizeof(int),1,fp);//把状态1写进去
            break;
        }
        else//如果这个不是要找的桌子
        {
            fseek(fp,8,SEEK_CUR);//跳过capacity和state部分
        }
    }
    fclose(fp);//用完文件就关闭


}

void addorder_and_printdishes(TMENU *tmenu,int menunumber)
//将订单写到文件里并打印订单信息，传入的第一个参数是tempmenu类型的数组的地址，第二个是菜的总数
{
    //开order文件
    FILE *fp;
    fp = fopen("order", "ab");//续写就好
    if(fp == NULL)
    {
        puts("打开文件失败！");
        system("pause");
        return;
    }
    //开menu文件
    FILE *fpmenu;
    fpmenu=fopen("menu","rb+");//打开方式为可读可写
    if(fp == NULL)
    {
        puts("打开文件失败！");
        system("pause");
        return;
    }

    puts("\n你的订单如下：");
    puts("--------------------------------");
    puts("编号 菜名                 单价");

    double totalmoney=0;//算总账用的
    int i,j;
    int mid,otimes;//这两个都是读menu时候的临时变量，mid用来存菜的id，otimes(originaltimes)用来存这道菜原来的被点次数
    for(i=0; i<menunumber; i++)//在tmenu数组里面线性查找
    {
        if(tmenu[i].times!=0)//说明这个菜客人点了
        {
            //把各项信息写入文件
            fwrite(&tmenu[i].tid, sizeof(int), 1, fp);
            fwrite(&tmenu[i].id, sizeof(int), 1, fp);
            fwrite(tmenu[i].name, sizeof(tmenu[i].name), 1, fp);
            fwrite(&tmenu[i].money, sizeof(float), 1, fp);
            fwrite(&tmenu[i].times, sizeof(int), 1, fp);
            printf("%-4d %-20s %.2f ×%d\n",tmenu[i].id, tmenu[i].name, tmenu[i].money,tmenu[i].times);//打印出来这道菜的信息
            totalmoney=totalmoney+tmenu[i].money*tmenu[i].times;//总价=原总价+这道菜的单价×这道菜在本桌被点的次数
            //接下来给菜品的times++
            fseek(fpmenu,0,SEEK_SET);//先把读写文件的“磁头”拨回原位
            for(j=0; j<menunumber; j++)//在菜单文件里面线性查找
            {
                fread(&mid,sizeof(int),1,fpmenu);//读入这道菜的id
                if(mid==tmenu[i].id)//如果和这道菜的id相同，那就是我们要找的
                {
                    fseek(fpmenu,22,SEEK_CUR);//后移22字节，跳过name部分
                    fread(&otimes,sizeof(int),1,fpmenu);//先读一下原来的值
                    fseek(fpmenu,-4,SEEK_CUR);//前移4字节，准备写入
                    otimes=otimes+tmenu[i].times;
                    fwrite(&otimes,sizeof(int),1,fpmenu);//写入新的值
                    break;
                }
                else
                {
                    fseek(fpmenu,30,SEEK_CUR);//一道菜在文件中的长度是34，已经走了4位了，在向后走30位就到了下一个
                }
            }
        }
    }
    fclose(fp);
    fclose(fpmenu);
    puts("--------------------------------");
    printf("点餐完成，共计%.2lf元\n", totalmoney);
    //delalldish(&dishhead);
}


int checkmenu()//检查table文件是否为空，如果打开失败，返回-1，如果为空，返回0
{
    FILE *fp;
    fp = fopen("menu", "rb");
    if(fp == NULL)
    {
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long end = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if(end == 0)
    {
        fclose(fp);
        return 0;
    }
    else
    {
        fclose(fp);
        return 1;
    }
}

int inputselect2()//这个负责检测整数的输入，非法输入返回-1，否则返回这个数
{
    char in[11];
    scanf("%9s", in);//int最大为2147483647，是十位数，所以就将输入的无论是多长的字符串截断为长度为9的，否则例如输入10个9，就超出int的最大值了
    fflush(stdin);
    int i;
    for(i = 0; i < strlen(in); i++)//在字符串里面一个个找，如果发现有不是数字的字符，返回-1
    {
        if(in[i] < 48 || in[i] > 57)
        {
            return -1;
        }
    }

    return atoi(in);//过了上面的检查，应该就是个数了，用atoi把字符串转成数字
}

int inputselect4()//inputselect2的翻版，只不过不允许输入0
{
    char in[11];
    scanf("%9s", in);
    fflush(stdin);
    int i;
    for(i = 0; i < strlen(in); i++)
    {
        if(in[i] < 49 || in[i] > 57)
        {
            return -1;
        }
    }

    return atoi(in);
}


int checktable()//检查table文件是否为空，如果打开失败，返回-1，如果为空，返回0
{
    FILE *fp;
    fp = fopen("table", "rb");
    if(fp == NULL)
    {
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long end = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if(end == 0)
    {
        fclose(fp);
        return 0;
    }
    else
    {
        fclose(fp);
        return 1;
    }
}


void order()//正片开始
{

    system("cls");

    int tableid = 0;//之后放桌号用的
    //检查桌子和菜单文件
    int tablestate = checktable();
    int menustate = checkmenu();

    if(tablestate == -1 || menustate == -1)
    {
        puts("打开文件失败！");
        system("pause");
        return;
    }
    else if(tablestate == 0 && menustate == 1)
    {
        puts("餐厅里面还没有桌子");
        system("pause");
        return;
    }
    else if(tablestate == 1 && menustate == 0)
    {
        puts("菜单还是空的");
        system("pause");
        return;
    }
    else if(tablestate == 0 && menustate == 0)
    {
        puts("餐厅里面没有桌子且菜单为空");
        system("pause");
        return;
    }
    else
    {
        printf("%s", "请输入来客人数：");
        int peoplenum = inputselect4();//输入来客人数
        if(peoplenum==-1)
        {
            puts("输入出错！");
            system("pause");
            return;
        }
        puts("");
        int i;
        int find = 0;//find一开始为0，找到桌子之后变成1
        //读取桌子，读到容量合适的桌子就break，一旦点餐成功，之后要对桌子的状态进行修改
        //开文件，读文件到内存
        FILE *fp;
        fp = fopen("table", "rb");
        if(fp == NULL)
        {
            puts("文件打开失败！");
            system("pause");
            return;
        }
        fseek(fp,0,SEEK_END);
        long end=ftell(fp);
        fseek(fp,0,SEEK_SET);
        int temp[3];//因为桌子文件里面都是int型的，就声明个数组当作临时变量吧
        while (ftell(fp)<end)
        {
            fread(&temp[0],sizeof(int),1,fp);
            fread(&temp[1],sizeof(int),1,fp);
            fread(&temp[2],sizeof(int),1,fp);
            if(temp[2]==0&&temp[1]>=peoplenum&&find==0)
            {
                find=1;
                tableid=temp[0];//给tableid赋值
                printf("请到%d号桌就座\n", tableid);
                break;
            }
        }
        fclose(fp);

        if(find == 0)
        {
            puts("现在客满了……下次再来吧！");
            system("pause");
            return;
        }

        puts("\n------------本店菜单------------\n");
        puts("编号 菜名                 单价");

        //开menu文件，读文件到内存
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
        end = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        int menunumber=end/34;//每道菜占用34个字节，所以用文件总长÷34就能得到菜的个数了
        TMENU tmenu[menunumber];//把数组建立好

        i=0;
        while (ftell(fp) < end)//这部分：读文件，写进数组
        {
            fread(&tempid, sizeof(tempid), 1, fp);
            fread(tempchar, sizeof(tempchar), 1, fp);
            fread(&temptimes, sizeof(temptimes), 1, fp);
            fread(&money, sizeof(money), 1, fp);

            tmenu[i].tid=tableid;
            tmenu[i].id=tempid;
            strcpy(tmenu[i].name,tempchar);
            tmenu[i].money=money;
            tmenu[i].times=0;
            printf("%-4d %-20s %.2lf\n", tempid, tempchar, money);//边写进数组，边打印出来，既完成了菜单的打印，又存入了内存
            i++;
        }
        fclose(fp);


        puts("\n--------------------------------\n");
        int orderornot = 0;//orderornot是用户是否点菜的状态
        while(1)
        {
            printf("%s", "请输入菜品的编号，输入0以结束：");
            int input;
            input = inputselect2();//inputselect2检测输入

            if(input == 0 && orderornot == 1)//点菜结束
            {
                addorder_and_printdishes(tmenu,menunumber);//向order文件写入并打印出来订单信息
                changetablestate_infile(tableid);//改桌子的状态

                system("pause");
                return;
            }
            else if(input == 0 && orderornot == 0) //没点菜……
            {
                puts("未点餐，即将返回上一级菜单");
                Sleep(1000);
                return;
            }
            else if(input == -1)
            {
                puts("输入出错！");
            }
            else
            {
                find = 0;
                for(i=0; i<menunumber; i++)//在菜单里面找有没有这道菜
                {
                    if(input == tmenu[i].id)
                    {
                        tmenu[i].times++;//找到了，就给本桌的被点次数+1
                        find = 1;
                        if(orderornot == 0)
                        {
                            orderornot = 1;//表示确实点过菜了
                        }
                        break;
                    }
                }
                if(find == 0)
                {
                    puts("似乎没有这道菜呢！");
                }
            }
        }
    }
}
