#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int value;
	struct Node *next;
	struct Node *prev;
}Node;

typedef struct {
	int size;
	Node *headNode;
	Node *endNode;
}List;

/*Adds a new value to the list*/
void add(List *list, int value){

	
	Node *newNode = (Node*) malloc(sizeof(Node));
	newNode->value = value;
	newNode->next = NULL;

	newNode->prev = (struct Node*) list->endNode;

	if(list->size==0){
		list->headNode = newNode;
	}else {
		list->endNode->next = (struct Node*) newNode;
	}
	list->endNode = newNode;
	
	list->size++;
}

/*query a value for the position*/
int get(List *list, int index){
	Node *head = list->headNode;

	if(head == NULL) return -1;

	int i;
	for(i = 0; i < list->size; i++){
		if(i==index){
			return head->value;
		}
		head = (Node*) head->next;
	}
}

/*Initializes the list*/
List createList(){
	List list;
	list.size = 0;
	list.headNode = NULL;
	list.endNode = NULL;
	return list;
}

/*Prints the list*/
void printList(List *list){
	Node *head = list->headNode;
	printf("{");
	
	int i;
	for(i = 0; i < list->size; i++){
		
		printf("%d", head->value);
		
		if( (i+1) < list->size){
			printf(", ");
		}
		head = (Node*) head->next;
	}
	printf("}\n");
}

void deleteList(List *list){
	Node *head = list->headNode;
	
	int i;
	for(i = 0; i < list->size; i++){
		Node *aux = (Node*) head->next;
		free(head);
		head = (Node*) aux;	
	}
}
