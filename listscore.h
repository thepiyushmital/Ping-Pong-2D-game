#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curses.h>
//link list structure for highscore saving
typedef struct info{
	char name[16];
	int level;
	int score;
}info;
typedef struct node{
        struct info data;
        struct node *next, *prev;
}node;
typedef struct list{
        struct node *head, *tail;
        int i;
}list;
void init(list *);
void insert(list *, struct info );
node *remove1(list *);
void includenew(info);

