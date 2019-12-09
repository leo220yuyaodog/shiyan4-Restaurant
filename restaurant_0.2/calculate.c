#include "Restaurant.h"
#include "globel.h"

void calculate()//正片开始
{
    system("cls");
    printf("%s", "请输入桌号：");

    int seetableid = inputselect2();//输入检测
    if(seetableid == -1||seetableid==0)
    {
        puts("输入出错");
        system("pause");
        return;
    }

    int find = 0;//这个表示order文件里面是否找到了tableid为seetableid的菜，若找到，会被改为1
    int printheadornot = 0;//订单的头只需要打印一次就够了，用这个状态变量控制head只打印一次
    int dishnum = 0;
    double totalmoney = 0;//算总账用的

    orderptr orderhead = NULL;//声明好链表头指针

    {
        //开文件，读order文件到内存
        FILE *fp;
        fp = fopen("order", "rb");
        if(fp == NULL)
        {
            puts("打开文件失败");
            system("pause");
            return;
        }
        fseek(fp, 0, SEEK_END);
        long end = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        int temptid;
        int tempid;
        char tempname[22];
        float tempmoney;
        int temptimes;

        while (ftell(fp) < end)//这次真的要全都读入！而且依然是边读入边打印
        {

            fread(&temptid, sizeof(int), 1, fp);//读一下tableid
            if(seetableid == temptid)//如果就是要找的
            {
                dishnum++;//菜的种类+1
                find = 1;//表示确实找到了
            }
            if(printheadornot == 0 && find == 1)//找到了，而且还没打印head，那就打印
            {
                puts("");
                printf("%d号桌的账单如下：\n", temptid);
                puts("--------------------------------");
                puts("编号 菜名                 单价");
                puts("");
                printheadornot = 1;//打印完了，状态变量改为1
            }
            fread(&tempid, sizeof(int), 1, fp);
            fread(tempname, sizeof(tempname), 1, fp);
            fread(&tempmoney, sizeof(float), 1, fp);
            fread(&temptimes, sizeof(int), 1, fp);

            if(seetableid == temptid) //如果是要找的
            {
                printf("%-4d %-20s %.2f ×%d\n", tempid, tempname, tempmoney, temptimes);//就打印出来详细信息
                totalmoney = totalmoney + tempmoney * temptimes;//总账=原总账+单价×数量
            }

            insertorder(&orderhead, temptid, tempid, tempname, tempmoney, temptimes);//插入到链表里面
        }
        fclose(fp);
    }

    if(find == 0)//while跑完了，但find为0，那就是没有找到
    {
        puts("订单里没有这个桌子的信息哦");
        system("pause");
        return;
    }
    else
    {
        printf("\n共消费了 %.2lf 元\n", totalmoney);//输出总账
        puts("--------------------------------");
    }


    printf("%s", "\n请输入您的付款（只支持整数哦）：");
    //测试的时候，double型的数值一旦变大，就会有精度问题出现，差个一分钱也算是相等……所以就干脆不弄小数输入了
    int pay = inputselect2();//用户输入付款金额
    if(pay == -1)
    {
        puts("输入出错！");
        system("pause");
        return;
    }

    if((double)pay - totalmoney > 0)//这里还是要强制转换一下的
    {
        printf("找您%.2lf元", (double)pay - totalmoney);
    }
    else if(totalmoney > (double)pay)//没带够钱可是不行的哦
    {
        puts("输入的数字小于订单总价");
        system("pause");
        return;
    }

    puts("\n谢谢惠顾，欢迎下次光临！");
    addaccount(totalmoney);//写入历史记录

    int i;
    for(i = 0; i < dishnum; i++)//删除订单
    {
        deleteorder(&orderhead, seetableid);
    }


    {
        //改桌子的状态
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
        int state = 0;
        while(ftell(fp) < end)
        {
            fread(&id, sizeof(int), 1, fp);
            if(id == seetableid)
            {
                fseek(fp, 4, SEEK_CUR);
                fwrite(&state, sizeof(int), 1, fp);
                break;
            }
            else
            {
                fseek(fp, 8, SEEK_CUR);
            }
        }
        fclose(fp);
    }

    writeorder(orderhead);//将新的链表写入文件
    delallorder(&orderhead);//摧毁链表，释放空间

    system("pause");
}
