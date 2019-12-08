#include "Restaurant.h"
#define PWDMAX 16//密码最大长度的宏定义
char seedstable[11] = "aicxoqpdst";//seed的密码表，

void tostring(int src, char *dest, int len)//把一个int型数字转为char型字符串的函数
{
    char c[len];
    int i = 0;
    for (i = 0; i < len; i++)
    {
        c[i] = (char)((src - (src / 10) * 10) + 48);
        src = src / 10;
    }
    int j = 0;
    for (i = len - 1; i >= 0; i--)
    {
        dest[j] = c[i];//把c[i]反着存到dest里面
        j++;
    }
    dest[len]='\0';//别忘了补0
}
void encode(char* src)//利用seed密码表把src加密
{
	int i;
	for (i = 0; i < 9; i++)
	{
		src[i] = seedstable[(int)(src[i] - 48)];
	}
}
void encypto(char* src, int seed)//真正的加密
{
	unsigned int i;
	char pwdtable[96];
	memset(pwdtable, 0, 96);
	for (i = 0; i < 95; i++)
	{
		pwdtable[i] = (char)(32 + i);//初始化密码表，里面存着所有的能从键盘输入的字符
	}
	srand(seed);//用传入的seed作为srand传入的参数

	for (i = 0; i < strlen(pwdtable); i++)//打乱密码表，由于srand的参数是固定的，这里打乱的结果也是固定的
	{
		int a = rand() % strlen(pwdtable);
		int b = rand() % strlen(pwdtable);
		char c = pwdtable[b];
		pwdtable[b] = pwdtable[a];
		pwdtable[a] = c;
	}

	unsigned int len = strlen(src);
	i = 0;
	while (i < len)
	{
		src[i] = pwdtable[(int)src[i] - 32];//对照打乱的密码表，得到密文
		i++;
	}
}

int randnum()//这个是真的获取一个随机数，长度为9位数
{
	int num;
	int totalNum;
	srand((unsigned int)(time(NULL)));
	totalNum = 0;
	int j;
	for (j = 0; j < 9; j++)
	{
		num = rand() % 9 + 1;
		totalNum += num * (int)pow(10, j);
	}
	return totalNum;
}


void decypto(char* src, int seed)//真正的解密
{
	char pwdtable[96];
	memset(pwdtable, 0, 96);
	unsigned int i;
	for (i = 0;i < 95;i++) {
		pwdtable[i] = (char)(32 + i);//依旧是先初始化好密码表
	}
	srand(seed);//用传入的seed作为srand的参数

	for (i = 0; i < strlen(pwdtable); i++)//打乱密码表，seed是确定的，这张表也就是确定的
	{
		int a = rand() % strlen(pwdtable);
		int b = rand() % strlen(pwdtable);
		char c = pwdtable[b];
		pwdtable[b] = pwdtable[a];
		pwdtable[a] = c;
	}
	unsigned int len = strlen(src);
	int j;
	i = 0;
	while (i < len)//按照密码表解密密文
	{
		for (j = 0; j < 96; j++)
		{
			if (pwdtable[j] == src[i])
			{
				src[i] = (char)(j + 32);
				i++;
				break;
			}
		}
	}
}

int decode(char* src)//解密seed的函数，成功返回0，出错返回-1
{
	int i, j, ok;

	for (i = 0; i < 9; i++)
	{
		ok = 0;
		for (j = 0; j < 10; j++)
		{
			if (src[i] == seedstable[j])
			{
				src[i] = (char)(j + 48);
				ok = 1;
				break;
			}
		}
		if (ok == 0)
			return -1;
	}
	return 0;
}





int getseed(char* src)//十分重要的getseed函数，返回明文seed，同时会把传入的src赋值为密码密文
{
	int seed;
	char seeds[9];
	char in[9+PWDMAX];
	memset(in, 0, 9+PWDMAX);
	unsigned int i, j;
	FILE* fp;
	fp = fopen("pwd", "r");
	if (fp == NULL)
	{
		return -1;
	}
	else
	{
		i = 0;

		if (fscanf(fp, "%[^\n]", in) == EOF) {
			puts("读取密码文件出现错误！");
			system("pause");
			return -1;
		}

		fclose(fp);

		if (strlen(in) > (9 + PWDMAX))
			//密码长度过长
			return -1;
		else {
			for (i = 0; i < 9; i++)
			{
				seeds[i] = in[i];
			}
			if (decode(seeds) == -1) {
				//密码格式出错
				return -1;
			}
			else {
				seed = atoi(seeds);
				for (i = 9, j = 0; i < strlen(in); i++)
				{
					src[j++] = in[i];
				}
				src[j] = '\0';
				return seed;
			}
		}
	}

}


int inandcmppwd(const char* rightpwd)
{
	char in[PWDMAX];
	int i = 0;
	while (i < PWDMAX)
	{
		in[i] = getch();

		if (in[i] == '\r' || in[i] == '\n')
		{
			in[i] = '\0';
			break;
		}
		else if (i == 15)
		{
			in[i + 1] = '\0';
		}

		if (in[i] == '\b')
		{
			if (i == 0)
			{
				continue;
			}
			else
			{
				printf("%s", "\b \b");
				i--;
			}
		}
		else
		{
			printf("*");
			i++;
		}
	}
	if (strcmp(in, rightpwd) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void changepwd()
{
	char pwd[PWDMAX];
	int seed = getseed(pwd);
	if (seed == -1)
    {
        puts("打开密码文件失败或密码文件出错！请联系管理员解决问题！");
        system("pause");
        return;
    }
	decypto(pwd, seed);
	char in2[PWDMAX];
	int i = 0;
	printf("%s", "\n请输入原密码以确认身份：");
	if (inandcmppwd(pwd) == 1)
	{
		printf("%s", "\n请输入新密码，长度不超过16位：");
		while (i < PWDMAX)
		{
			in2[i] = getch();

			if (in2[i] == '\r' || in2[i] == '\n')
			{
				in2[i] = '\0';
				break;
			}
			else if (i == PWDMAX-1 )
			{
				in2[i + 1] = '\0';
			}

			if (in2[i] == '\b')
			{
				if (i == 0)
				{
					continue;
				}
				else
				{
					printf("%s", "\b \b");
					i--;
				}
			}
			else
			{
				printf("*");
				i++;
			}
		}
		if (strcmp(in2, pwd) == 0)
		{
			puts("\n新密码和原密码相同哦，自动返回中");
			Sleep(1000);
			system("cls");
		}
		else
		{
			printf("%s", "\n请再次输入新密码：");
			while (1)
			{
				int state = inandcmppwd(in2);
				static int times = 0;
				if (state == 0)
				{

					puts("\n两次密码不一致！");

					times++;
					if (times >= 5)
					{
						printf("%s", "是否继续输入，若否，将返回至菜单(Y/N): ");
						char s;
						s = getch();
						printf("%c", s);
						if (s == 'Y' || s == 'y')
						{
							printf("%s", "\n请再次输入新密码：");
							continue;
						}
						else
						{
							puts("\n输入终止，即将自动返回");
							times = 0;
							Sleep(1000);
							system("cls");
							break;
						}
					}
					printf("%s", "请再次输入新密码：");
				}
				else if (state == 1)
				{

					int seed = randnum();

					encypto(in2, seed);

					char seeds[9];
					tostring(seed, seeds, 9);

					char out[9 + PWDMAX];
					encode(seeds);

					strcpy(out, seeds);

					strcat(out, in2);

					FILE* fp;
					fp = fopen("pwd", "w");
					if (fp == NULL) {
						puts("密码文件打开失败！即将自动返回");
						break;
					}
					else {
						fprintf(fp, "%s", out);
						fclose(fp);
						puts("\n密码修改成功！即将自动返回");
					}
					Sleep(1000);
					system("cls");
					break;
				}
			}
		}
	}
	else
	{
		puts("\n密码输入错误，任务终止！即将自动返回");
		Sleep(1000);
		system("cls");
	}
}

