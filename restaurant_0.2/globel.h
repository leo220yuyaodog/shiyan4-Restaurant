#ifndef	GLOBAL_H
#define  GLOBAL_H
//这个头文件存放了链表所要用到的三种结构体类型声明
typedef struct table//定义桌子结构体
{
    int id;//桌子编号
    int capacity;//桌子容量
    int state;//桌子状态。这个值为0表示空闲，1表示有人，order.c的changetablestate_infile函数和calculate.c的结尾部分会直接在文件里修改这个值
    struct table *next;//链表的标准操作，定义一个指向下一个节点的指针

} Table;
typedef Table *tableptr;//这里参考了课本，把Table* typedef了一下，方便后期使用

typedef struct menu//定义菜单结构体
{
    int id;//菜的编号
    char name[22];//菜的名字，经过尝试，22的长度最多可以存入10个汉字，不会产生什么奇怪的符号
    int times;//菜被点的次数
    float money;//菜的价格，用float应该就够了，存五位整数和两位小数是不会发生问题的，一般不会有五位数价格的菜吧
    struct menu *next;//链表标准操作

} Menu;
typedef Menu *menuptr;//依旧是参考课本，typedef，方便后期使用

typedef struct order//订单结构体，每一个order类型的数据只存一道菜
{
    int tid;//桌号
    int id;//菜品编号
    char name[22];//菜品名
    float money;//菜品单价
    int times;//这道菜在本桌被点的次数
    struct order *next;//链表标准操作
} Order;
typedef Order *orderptr;//仍然是参考课本，typedef，方便后期使用


#endif
