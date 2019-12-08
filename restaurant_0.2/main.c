#include "Restaurant.h"
void mainmenu()
{
    while (1)
    {
        puts("*****************************");
        puts("    欢迎进入餐厅服务系统");
        puts("*****************************");
        puts("");
        puts("-------------------");
        puts("1.点菜");
        puts("2.客户结账");
        puts("3.餐馆统计");
        puts("4.其他功能");
        puts("5.退出");
        puts("-------------------");
        printf("\n%s", "请选择（无需按回车）：");
        switch (inputselect1(5))//inputselect进行输入合法判断，返回的是输入的值，若非法输入，则返回-1，传入的一个int类型的变量是菜单的最大选项数
        {
        case 1:
            order();//点菜，具体在order.c中
            system("cls");//cls这个cmd命令实现清屏效果
            break;
        case 2:
            calculate();//结账，具体在calculate.c中
            system("cls");
            break;
        case 3:
            seedishrank();//查看统计信息，具体在seedishrank.c中
            system("cls");
            break;
        case 4:
            otherfunc();//其他功能
            break;
        case 5://退出程序
            puts("\n谢谢您的惠顾，欢迎下次光临！");
            system("pause");
            exit(0);
        case -1://非法输入后进行提示
            system("cls");
            puts("输入错误，请重新输入!");
            system("pause");
            system("cls");
            break;
        default:
            break;
        }
    }
}

void otherfunc()
{
    system("cls");
    char pwd[PWDMAX];//PWDMAX在pwd.h中声明
    int seed = getseed(pwd);//getseed读取密码文件，将密文直接存到pwd中，返回解密后的seed值，返回-1表示出错
    if (seed == -1)
    {
        puts("打开密码文件失败或密码文件出错！请联系管理员解决问题！");
        system("pause");
        return;
    }
    else
    {
        decypto(pwd, seed);//利用seed值和密文解密密文，得到密码明文
        //puts(pwd);//正式发行时要去掉吧……
        printf("%s", "请输入密码，按回车停止输入：");
        if (inandcmppwd(pwd) == 1)//inandcmppwd负责密码的输入和比对，比对成功则返回1
        {
            system("cls");//这个清屏是把输入密码的部分清除
            while (1)
            {
                puts("");
                puts("-------------------");
                puts("1.账目管理");
                puts("2.菜单管理");
                puts("3.餐桌管理");
                puts("4.修改密码");
                puts("5.返回上一级");
                puts("6.退出");
                puts("-------------------");
                printf("\n%s", "请选择（无需按回车）：");
                switch (inputselect1(6))
                {
                case 1:
                    system("cls");
                    accountmgr();//账目管理，函数具体内容在accountmgr.c中
                    system("cls");//函数结束之后再清个屏
                    break;
                case 2:
                    system("cls");
                    menumgr();//菜单管理，具体在menumgr.c中
                    system("cls");
                    break;
                case 3:
                    system("cls");
                    tablemgr();//餐桌管理，具体在tablemgr.c中
                    system("cls");
                    break;
                case 4:
                    system("cls");
                    changepwd(pwd);//修改密码，传入的是当前的密码明文
                    break;
                case 5:
                    system("cls");
                    mainmenu();//回到主菜单
                    break;
                case 6://直接退出
                    puts("\n欢迎下次使用！");
                    system("pause");
                    exit(0);
                case -1:
                    system("cls");
                    puts("输入错误，请重新输入!");
                    system("pause");
                    system("cls");
                    break;
                default:
                    break;
                }
            }
        }
        else//密码认证不通过的情况下，回到主菜单
        {
            system("cls");
            puts("密码输入错误！已返回至主菜单\n");
            mainmenu();
        }
    }
}



int inputselect1(int maxn)
//inputselect进行输入合法判断，返回的是输入的值，若非法输入，则返回-1，
//传入的int类型的变量是菜单中的最大选项数
{
    int select;
    char s;
    s = getch();//getch只读入一个字符并且不回显的特性，比较适合进行单个数字的输入，这样就不用按回车来完成输入了
    putchar(s);//手动回显一下
    fflush(stdin);//习惯性地清除输入流
    select = (int)(s - 48);//把输入的字符转成int类型的，1的ASCII码是49，所以如果输入的是数字，字符的ASCII-48就刚好是这个数字
    if (select < 1 || select > maxn)//如果输入的比1小或者比传入的最大值大，就是非法输入了
    {
        return -1;
    }
    else
        return select;
}


int main()//这个main确实有点单薄
{
    mainmenu();
    return 0;
}
