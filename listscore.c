#include"listscore.h"
/*creating list functions*/
/*initializing list*/
void init(list *l){
        l->head = NULL;
        l->tail = NULL;
        l->i = 0;
}
/*sorting list elements*/
void sort(list *l){
	node *q;
	struct info temp;
	int i, j;
	for(i = 0; i < l->i; i++){
		q = l->head;
		for(j = 0; j < l->i && q->next; j++){
			if(q->data.score < q->next->data.score){
				temp = q->data;
				q->data = q->next->data;
				q->next->data = temp; 
			}
			q = q->next;
		}
	}
}
/*insertion in list*/
void insert(list *l, struct info store){
        node *temp;
        temp = (struct node *)malloc( 1 * sizeof(struct node));
        temp->data = store;
        temp->next = NULL;
        temp->prev = NULL;
        if(l->head == NULL && l->tail == NULL){
                l->head = l->tail = temp;
                temp->next = temp->prev = NULL;
                l->i = l->i + 1;
                return;
        }
        if(l->head == l->tail){
                l->head->next = temp;
                temp->prev = l->head;
                temp->next = NULL;
                l->tail = temp;
		l->i = l->i + 1;                
		return;
        }
        l->tail->next = temp;
        temp->prev = l->tail;
        temp->next = NULL;
        l->tail = temp;
        l->i = l->i + 1;
        return;
}
/*removal from list*/
node *remove1(list *l){
        node *temp;
        temp = (node *)malloc(sizeof(node));
	temp->next = temp->prev = NULL;
        if(l->head == NULL && l->tail == NULL){
                return NULL;
        }
        if(l->head == l->tail){
                temp->data = l->head->data;
                l->head = l->tail = NULL;
                l->i--;
                return temp;
        }
        temp = l->head;
        l->head = l->head->next;
        l->head->prev = NULL;
        return temp;
}
/*Checking if score can be loaded in highscore.
 *If yes then score uploaded in highscore table
 *Else nothing done*/
void includenew(struct info store1){
	struct info store;
	int n, i = 0;
	list l;
	init(&l);
	node *p;
	struct info temp;
	FILE *fp;
	fp = fopen("highscore.c", "r+");
	if(fp == NULL){
		printf("File Couldnt be opened\n");
		return;
	}
 	fread(&n, 1, sizeof(int), fp);	
	for(i = 0; i < n; i++){
		fread(&store, 1,  sizeof(struct info), fp);
		insert(&l, store);
	}
	insert(&l, store1);	
	sort(&l);	
	rewind(fp);	
	n = n + 1;
	if(n > 10)
		n = 10;	
	fwrite(&n, 1, sizeof(int), fp);
	i = 0;
	while(i < n){
		p = remove1(&l);
		temp = p->data;
		fwrite(&temp, 1, sizeof(struct info), fp);
		i++;
	}	
	free(p);
	fclose(fp);
	return;
}
