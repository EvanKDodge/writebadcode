typedef struct Node {
	struct Node *prev;
	char str[25];
	struct Node *next;
} Node;

typedef struct List {
	struct Node *head;
	struct Node *tail;
} List;

void insertAtHead(Node**, Node**, char*);
void printForward(Node*);
void printReverse(Node*);
void deleteAllFromHead(Node**);
/*
void delete(Node**, char*);
void find(Node*, char*);
*/
