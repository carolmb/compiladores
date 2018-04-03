#include <stdio.h>
#include "list.c"


int main(){

	List list = createList();

	printf("Size: %i\n",list.size);

	add(&list, 1);
	
	add(&list, 2);
	
	printf("Size: %i\n",list.size);

	printList(&list);

	deleteList(&list);

	return 0;
}
