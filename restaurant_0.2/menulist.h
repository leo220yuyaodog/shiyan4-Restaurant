#ifndef	MENULIST_H
#define  MENULIST_H
void insertmenu(menuptr *sPtr, int id, char name[], int times, float money);
char deletemenu (menuptr *sPtr, int value);
void writemenu(menuptr head);
void delallmenu(menuptr *head);
#endif

