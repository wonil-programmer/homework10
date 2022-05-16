#include <stdio.h>
#include <stdlib.h>


/* 노드 구조체 선언 */
typedef struct node {
	int key;
	struct node *left;
	struct node *right;
} Node;

/* 노드 구조체 포인터형 스택 배열 */
#define MAX_STACK_SIZE	20
Node* stack[MAX_STACK_SIZE]; // 스택 선언
int top = -1;

Node* pop(); // 스택의 원소 추출
void push(Node* aNode); // 스택에 원소 삽입

/* 노드 구조체 포인터형 큐 배열 */
#define MAX_QUEUE_SIZE 20
Node* queue[MAX_QUEUE_SIZE]; // 큐 선언
int front = -1;
int rear = -1;

Node* deQueue(); // 큐의 원소 추출
void enQueue(Node* aNode); // 큐에 원소 삽입

/* 함수 원형 선언 */
int initializeBST(Node** h);
void recursiveInorder(Node* ptr);	  
void iterativeInorder(Node* ptr);
void levelOrder(Node* ptr);	     
int insert(Node* head, int key); 
int deleteNode(Node* head, int key);  
int freeBST(Node* head); 

void printStack();

int main()
{
	printf("------ [2019036068] [허원일] ------\n");
	char command;
	int key;
	Node* head = NULL;

	do{
		printf("\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			/* BST를 초기화 */
			initializeBST(&head); 
			break;

		case 'q': case 'Q':
			/* BST에 할당된 메모리 반납 */
			freeBST(head);
			break;

		case 'i': case 'I':
			/* BST에 원소 삽입 */
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);
			break;

		case 'd': case 'D':
			/* BST의 원소 삭제 */
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
			/* 재귀 전위 순회 */
			recursiveInorder(head->left);
			break;

		case 't': case 'T':
			/* 반복 전위 순회 */
			iterativeInorder(head->left);
			break;

		case 'l': case 'L':
			/* 레벨 순회 */
			levelOrder(head->left);
			break;

		case 'p': case 'P':
			/* 스택 출력 */
			printStack();
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

/* BST를 초기화하는 함수 */
int initializeBST(Node** h) {

	/* head가 NULL이 아닌 경우 즉, 트리가 비어있지 않으면 freeBST로 트리에 할당된 메모리 해제 */
	if(*h != NULL)
		freeBST(*h);
	
	/* head에 메모리 할당 후 각 연결 노드 및 key 초기화 */
	*h = (Node*)malloc(sizeof(Node));
	(*h)->left = NULL;	
	(*h)->right = *h;
	(*h)->key = -9999;

	top = -1; // 스택의 top 지정

	front = rear = -1; // 큐의 front와 rear 지정

	return 1;
}

/* BST를 재귀로 중위순회(왼쪽 자식노드 -> 부모노드 -> 오른쪽 자식노드 순)하는 함수 */
void recursiveInorder(Node* ptr)
{
	/* 노드가 존재하면 수행 */
	if(ptr) {
		recursiveInorder(ptr->left); // 노드의 왼쪽 노드를 재귀적으로 중위순회
		printf(" [%d] ", ptr->key); // 해당 노드의 key 출력
		recursiveInorder(ptr->right); // 노드의 오른쪽 노드를 재귀적으로 중위순회
	}
}

/* BST를 반복으로 중위순회(왼쪽 자식노드 -> 부모노드 -> 오른쪽 자식노드 순)하는 함수 */
void iterativeInorder(Node* node)
{
	for(;;)
	{
		for(; node; node = node->left)
			push(node); // 스택에 node 삽입
		node = pop(); // 스택에서 삭제

		/* node가 NULL, 즉 스택이 공백이면 반복문 종료 */
		if(!node) 
			break; 

		printf(" [%d] ", node->key); // node의 key 출력

		node = node->right; // node의 오른쪽 자식 노드를 node로 지정
	}
}

/* BST를 레벨 순회하는 함수 */
void levelOrder(Node* ptr)
{
	/* BST가 빈 경우 함수 종료 */
	if(!ptr) return; 

	enQueue(ptr); // 루트노드를 큐에 삽입

	for(;;)
	{
		ptr = deQueue(); // 큐에서 원소 추출 후 ptr에 대입

		/* 추출한 원소가 NULL이 아닌 node인 경우 */
		if(ptr) {
			printf(" [%d] ", ptr->key); // ptr의 key 출력

			if(ptr->left) // 왼쪽 자식노드가 있는 경우, 그 자식노드를 큐에 삽입
				enQueue(ptr->left);
			if(ptr->right) // 오른쪽 자식노드가 있는 경우, 그 자식노드를 큐에 삽입
				enQueue(ptr->right);
		}

		/* 추출한 원소가 NULL인 경우 반복문 탈출 */
		else
			break;
	}
}

/* BST에 특정 key의 node를 삽입하는 함수 */
int insert(Node* head, int key)
{
	/* 새로운 노드 생성 */
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

	/* head의 왼쪽 자식노드가 NULL인 경우 */
	if (head->left == NULL) {
		head->left = newNode; // head의 왼쪽에 새로운 노드 삽입
		
		return 1;
	}

	/* head의 왼쪽 자식노드가 NULL이 아닌 경우 */
	Node* ptr = head->left; // ptr이 head의 왼쪽노드를 가리키게 함 (루트노드)

	Node* parentNode = NULL; // 노드 구조체 포인터 parentNode

	// ptr이 리프노드를 가리킬 때까지 반복
	while(ptr != NULL) {

		/* 해당 key의 노드가 존재할 경우 함수 종료 */
		if(ptr->key == key) return 1;

		parentNode = ptr; // parentNode는 ptr을 따라감

		/* 현재 노드의 key가 삽입될 노드의 key보다 작은 경우 */
		if(ptr->key < key)
			ptr = ptr->right; // ptr을 ptr의 오른쪽 자식 노드를 가리키게 함
		/* 현재 노드의 key가 삽입될 노드의 key보다 큰 경우 */
		else
			ptr = ptr->left; // ptr을 ptr의 왼쪽 자식 노드를 가리키게 함
	}

	/* 새로운 노드를 부모노드의 자식노드로 삽입 */
	if(parentNode->key > key)
		// 부모노드의 key가 삽입할 노드의 key보다 큰 경우, 왼쪽 자식노드에 삽입
		parentNode->left = newNode; 
	else
		// 부모노드의 key가 삽입할 노드의 key보다 작은 경우, 오른쪽 자식노드에 삽입
		parentNode->right = newNode;

	return 1;
}

/* BST의 특정 key의 node를 삭제하는 함수 */
int deleteNode(Node* head, int key)
{
	/* head가 NULL인 경우, 즉 head에 메모리가 없으면 경고문 출력 후 함수 종료 */
	if (head == NULL) {
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	/* head의 left가 NULL인 경우, 즉 트리가 비었으면 경고문 출력 후 함수 종료 */
	if (head->left == NULL) {
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	Node* root = head->left; // ptr이 head의 왼쪽 자식노드를 가리키게 함

	Node* parent = NULL; // parent를 NULL로 지정
	Node* ptr = root; // 임시 노드 구조체 포인터 ptr (root를 가리키게 함)

	/* 노드가 존재하고 노드의 key가 찾는 key가 아니면 반복 */
	while((ptr != NULL) && (ptr->key != key)) {
		/* 현재노드의 key가 찾는 key가 아닌 경우 */
		if(ptr->key != key) {
			parent = ptr; // ptr을 따라가는 parent

			/* 찾는 key보다 현재노드의 key가 더 큰 경우 */
			if(ptr->key > key)
				ptr = ptr->left; // 현재노드의 왼쪽 자식노드로 이동
			/* 찾는 key보다 현재노드의 key가 더 작은 경우 */
			else
				ptr = ptr->right; // 현재노드의 오른쪽 자식노드로 이동
		}
	}

	/* 리프노드까지 도달했지만 key 값의 노드가 없을때 경고문 출력 후 함수종료 */
	if(ptr == NULL)
	{
		printf("No node for key [%d]\n ", key);

		return -1;
	}

	/* 찾는 key의 노드가 있는 경우 */
	/* Case 1 : 해당노드가 리프노드인 경우 */
	if(ptr->left == NULL && ptr->right == NULL)
	{		
		/* parent가 NULL이 아닌 경우 */
		if(parent != NULL) { 
			/* 리프노드가 부모의 왼쪽 자식노드인 경우 */
			if(parent->left == ptr)
				parent->left = NULL;
			/* 리프노드가 부모의 오른쪽 자식노드인 경우 */
			else	
				parent->right = NULL;
		} 
		/* 루트노드가 리프노드일 경우 */
		else {
			head->left = NULL;
		}

		free(ptr); // 삭제한 현재노드(리프노드)의 메모리 해제

		return 1;
	}

	/* Case 2 : 해당노드의 자식노드가 하나인 경우 */
	if ((ptr->left == NULL || ptr->right == NULL))
	{

		Node* child; // 임시 노드구조체 포인터 child

		/* 왼쪽 자식노드가 있는 경우 */
		if (ptr->left != NULL)
			child = ptr->left; // 자식노드를 현재노드의 왼쪽 자식노드로 지정
		/* 오른쪽 자식노드가 있는 경우 */
		else
			child = ptr->right; // 자식노드를 현재노드의 오른쪽 자식노드로 지정

		/* 부모노드가 있는 경우 */
		if(parent != NULL)
		{
			/* 현재노드가 부모노드의 왼쪽 자식노드인 경우 */
			if(parent->left == ptr)
				parent->left = child; // 부모노드의 왼쪽 자식노드를 현재노드의 자식노드로 지정
			/* 현재노드가 부모노드의 오른쪽 자식노드인 경우 */
			else
				parent->right = child; // 부모노드의 오른쪽 자식노드를 현재노드의 자식노드로 지정
		} 
		else {
			/* 트리에 루트노드와 그 자식노드 두개만이 존재하는 경우 */
			root = child; // 자식노드가 루트노드로 지정
		}

		free(ptr); // 삭제할 노드(현재노드) 메모리 해제
		
		return 1;
	}

	/* Case 3 : 해당노드의 자식노드가 둘인 경우 */
	Node* candidate;
	parent = ptr; // ptr을 따라가는 parent(부모노드)

	candidate = ptr->right; // 후계자 노드 후보를 삭제할 노드의 오른쪽 자식노드로 지정

	/* 후계자 노드 후보는 삭제할 노드의 오른쪽 서브트리 중 가장 작은 값 */
	while(candidate->left != NULL)
	{
		parent = candidate; // 후계자 노드 후보를 따라가는 parent
		candidate = candidate->left; //후계자 노드를 후계자 노드 후보의 왼쪽 자식노드로 지정
	}

	/* 삭제할 노드의 오른쪽 자식노드가 후계자 노드인 경우 */
	if (parent->right == candidate)
		parent->right = candidate->right; // 후계자 노드의 오른쪽 서브트리를 삭제할 노드의 오른쪽에 붙임
	/* 삭제할 노드의 오른쪽 자식노드가 후계자 노드가 아닌 경우 */
	else
		parent->left = candidate->right; // 후계자 노드의 오른쪽 서브트리를 부모노드의 왼쪽에 붙임

	ptr->key = candidate->key; // 후계자 노드의 key와 삭제할 노드의 key를 맞교환
	free(candidate); // 후계자 노드의 메모리 해제

	return 1;
}

/* node들에 할당된 메모리를 재귀적으로 해제하는 함수 */
void freeNode(Node* ptr)
{
	if(ptr) {
		freeNode(ptr->left); // 해당 노드의 왼쪽 자식노드의 메모리를 해제
		freeNode(ptr->right); // 해당 노드의 오른쪽 자식노드의 메모리를 해제
		free(ptr); // 해당 노드의 메모리 해제
	}
}

/* BST에 할당된 메모리를 해제하는 함수 */
int freeBST(Node* head)
{
	/* head만 존재하는 경우 */
	if(head->left == head)
	{
		free(head);

		return 1;
	}

	Node* p = head->left; // 임시 노드구조체 포인터 p (루트노드를 가리킴)

	freeNode(p); // 루트노드로부터 하위노드들을 모두 메모리 해제

	free(head); // head의 메모리 해제

	return 1;
}


/* 스택에서 원소를 추출하는 함수 */
Node* pop()
{
	if (top < 0)
	 return NULL;

	return stack[top--];
}

/* 스택에 원소를 삽입하는 함수 */
void push(Node* aNode)
{
	stack[++top] = aNode;
}

/* 스택을 출력하는 함수 */
void printStack()
{
	int i = 0;
	printf("--- stack ---\n");

	while(i <= top)
	{
		printf("stack[%d] = %d\n", i, stack[i]->key);
	}
}

/* 레벨순회 구현시, 큐에서 원소를 추출하는 함수 */
Node* deQueue()
{
	/* 큐가 공백상태인 경우 */
	if (front == rear) {
		printf("공백상태\n");
		return NULL; // NULL 반환
	}

	front = (front + 1) % MAX_QUEUE_SIZE; // front를 다음칸으로 이동

	return queue[front]; // front의 원소 반환
}

/* 레벨순회 구현시, 큐에 원소를 삽입하는 함수 */
void enQueue(Node* aNode)
{
	rear = (rear + 1) % MAX_QUEUE_SIZE; // rear를 다음칸으로 이동

	/* 큐가 포화상태인 경우 */
	if (front == rear) {
		printf("포화상태\n");
		return; // 함수종료
	}

	queue[rear] = aNode; // 큐에 원소 삽입
}





