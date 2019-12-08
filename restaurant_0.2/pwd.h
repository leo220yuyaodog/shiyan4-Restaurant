#ifndef	PWD_H
#define  PWD_H

#define PWDMAX 16
void tostring(int src, char *dest, int len);
void encode(char* src);

void encypto(char* src, int seed);
int randnum();
void decypto(char* src, int seed);
int decode(char* src);
int getseed(char* src);
int inandcmppwd(const char* rightpwd);
void changepwd();
#endif
