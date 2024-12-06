#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doubly-linked-list.h"

int main(void) {
	List dList;
	List dList2;
	List dList3;

	dList.head = NULL;
	dList.tail = NULL;

	insertAtHead(&dList.head, &dList.tail, "Lego(tm) Batman(tm) Jams:");
	insertAtHead(&dList.head, &dList.tail, "darkness");
	insertAtHead(&dList.head, &dList.tail, "no parents");
	insertAtHead(&dList.head, &dList.tail, "really rich");
	insertAtHead(&dList.head, &dList.tail, "kinda makes it better");

	printReverse(dList.tail);
	printForward(dList.head);
	deleteAllFromHead(&dList.head);

	dList2.head = NULL;
	dList2.tail = NULL;

	insertAtTail(&dList2.head, &dList2.tail, "one");
	insertAtTail(&dList2.head, &dList2.tail, "two");
	insertAtTail(&dList2.head, &dList2.tail, "three");
	insertAtTail(&dList2.head, &dList2.tail, "four");
	insertAtTail(&dList2.head, &dList2.tail, "five");

	printReverse(dList2.tail);
	printForward(dList2.head);
	deleteAllFromHead(&dList2.head);

	dList3.head = NULL;
	dList3.tail = NULL;

	insertAtHead(&dList3.head, &dList3.tail, "one");
	insertAtTail(&dList3.head, &dList3.tail, "two");
	insertAtHead(&dList3.head, &dList3.tail, "three");
	insertAtTail(&dList3.head, &dList3.tail, "four");

	printForward(dList3.head);
	printReverse(dList3.tail);
	deleteAllFromTail(&dList3.tail);
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

void insertAtTail(Node **head, Node **tail, char *s) {
	Node *tmp;

	if((tmp = (Node *)malloc(sizeof(Node))) == NULL) {
		printf("Memory allocation error\n");
		exit(1);
	}

	strcpy(tmp->str, s);
	tmp->prev = *tail;

	if(*tail != NULL) tmp->prev->next = tmp;
	tmp->next = NULL;

	*tail = tmp;
	if(*head == NULL) *head = tmp;
}

void insertAtPosFromHead(Node **head, int pos, char*) {
	// handle inserting directing at head
	// handle pos > list length
	Node *tmp;
	Node *nextNode;
	int currentPos = 0;

	while
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

void deleteAllFromTail(Node **tail) {
	Node *prevNode;

	prevNode = *tail;

	while(prevNode != NULL) {
		*tail = prevNode->prev;
		free(prevNode);
		prevNode = *tail;
	}
}
/*
void delete() {

}

void find() {

}
*/
