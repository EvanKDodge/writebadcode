#include <stdio.h>
#include <stdlib.h>
#include "linked-list.h"

void printList(Node*);
void addItem(Node**, int);

int main(void) {
	Node* head;

	head = NULL;

	addItem(&head, 1);
	addItem(&head, 2);

	printList(head);
}

void addItem(Node** head, int num) {
	Node* tmp;

	if((tmp = (Node *)malloc(sizeof(Node))) == NULL) {
		printf("Memory allocation error\n");
		exit(1);
	}

	tmp->data = num;
	tmp->next = *head;

	*head = tmp;
}

void printList(Node* head) {
	Node *nextNode;
	nextNode = head;

	do {
		printf("%d\n", nextNode->data);
		nextNode = nextNode->next;
	} while(nextNode != NULL);
}
