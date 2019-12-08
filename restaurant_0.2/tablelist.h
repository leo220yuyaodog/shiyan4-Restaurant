#ifndef	TABLELIST_H
#define  TABLELIST_H
#include "globel.h"
void inserttable(tableptr *sPtr, int id, int capacity, int state);
int deletetable (tableptr *sPtr, int value);
void writetable(tableptr head);
void delalltable(tableptr *head);
#endif
