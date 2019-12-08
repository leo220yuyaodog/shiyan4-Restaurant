#ifndef	ORDERLIST_H
#define  ORDERLIST_H

void insertorder(orderptr *sPtr,int temptid,int tempid,char *tempname,float tempmoney,int temptimes);
void deleteorder (orderptr *sPtr, int value);
void writeorder(orderptr head);//write to file;
void delallorder(orderptr *head);

#endif
