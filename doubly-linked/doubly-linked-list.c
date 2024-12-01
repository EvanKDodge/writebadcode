#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doubly-linked-list.h"

int main(void) {
	List dList;

	dList.head = NULL;
	dList.tail = NULL;

	insertAtHead(&dList.head, &dList.tail, "test");
	insertAtHead(&dList.head, &dList.tail, "fart");
	insertAtHead(&dList.head, &dList.tail, "massive turd");
	insertAtHead(&dList.head, &dList.tail, "uber dump");

	printForward(dList.head);
	printReverse(dList.tail);
	deleteAllFromHead(&dList.head);
}

void insertAtHead(Node **head, Node **tail, char *s) {
	Node *tmp;

	if((tmp = (Node *)malloc(sizeof(Node))) == NULL) {
		printf("Memory allocation error\n");
		exit(1);
	}

	strcpy(tmp->str, s);
	tmp->next = *head;

	if(*head != NULL) tmp->next->prev = tmp;
	tmp->prev = NULL;

	*head = tmp;
	if(*tail == NULL) *tail = tmp;
}

void printForward(Node *head) {
	Node *nextNode;

	nextNode = head;

	while(nextNode != NULL) {
		printf("%s\n", nextNode->str);
		nextNode = nextNode->next;
	}
	printf("\n");
}

void printReverse(Node *tail) {
	Node *nextNode;

	nextNode = tail;

	while(nextNode != NULL) {
		printf("%s\n", nextNode->str);
		nextNode = nextNode->prev;
	}
	printf("\n");
}

void deleteAllFromHead(Node **head) {
	Node *nextNode;

	nextNode = *head;

	while(nextNode != NULL) {
		*head = nextNode->next;
		free(nextNode);
		nextNode = *head;
	}
}
/*
void delete() {

}

void find() {

}
*/
