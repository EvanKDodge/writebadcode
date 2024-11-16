#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int value;
	struct Node *next;
} Node;

Node *create_node(int value) {
	Node *new_node = malloc(sizeof(Node));
	new_node->value = value;
	new_node->next = NULL;
	return new_node;
}

void printList(Node *head) {
	Node *tmp = head;
	while(tmp != NULL) {
		printf("%d - ", tmp->value);
		tmp = tmp->next;
	}
	printf("\n");
}

void freeList(Node *head) {
	Node *tmp;

	tmp = head;
	while(tmp != NULL) {
		head = tmp->next;
		printList(head);
		free(tmp);
		tmp = head;
	}
}

int main(void) {
	Node *head = NULL;
	Node *tmp;

	for(int i = 25;i > 0;i--) {
		tmp = create_node(i);
		tmp->next = head;
		head = tmp;
	}

	printList(head);
	printf("\n\n");
	freeList(head);

	return 0;
}
