#define _CRT_SECURE_NO_WARNINGS
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void clearBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static int readInt(void) {
    int value;
    if (scanf("%d", &value) != 1) {
        clearBuffer();
        return -1;
    }
    clearBuffer();
    return value;
}

Node* createNode(void) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        printf("Ошибка выделения памяти.\n");
        return NULL;
    }

    printf("Название (ключ дерева): ");
    fgets(node->title, STR_LEN, stdin);
    node->title[strcspn(node->title, "\n")] = 0;

    printf("Описание: ");
    fgets(node->description, STR_LEN, stdin);
    node->description[strcspn(node->description, "\n")] = 0;

    printf("Статус (1 - выполнена, 0 - нет): ");
    node->is_completed = (readInt() == 1);

    printf("Срок: ");
    fgets(node->deadline, STR_LEN, stdin);
    node->deadline[strcspn(node->deadline, "\n")] = 0;

    node->left = NULL;
    node->right = NULL;
    return node;
}

void printNode(const Node* node) {
    if (node == NULL) {
        return;
    }
    printf("%s | %s | %s | %s\r\n",
        node->title,
        node->description,
        node->is_completed ? "выполнена" : "не выполнена",
        node->deadline);
}

/* Левый потомок меньше родителя, правый — больше. Ключ — название. */
Node* insertNode(Node* root, Node* newNode) {
    if (newNode == NULL) {
        return root;
    }
    if (root == NULL) {
        return newNode;
    }

    int cmp = strcmp(newNode->title, root->title);
    if (cmp < 0) {
        root->left = insertNode(root->left, newNode);
    } else if (cmp > 0) {
        root->right = insertNode(root->right, newNode);
    } else {
        printf("Задача с таким названием уже есть в дереве.\n");
        free(newNode);
    }
    return root;
}

Node* searchNode(Node* root, const char* key) {
    if (root == NULL) {
        return NULL;
    }

    int cmp = strcmp(key, root->title);
    if (cmp == 0) {
        return root;
    }
    if (cmp < 0) {
        return searchNode(root->left, key);
    }
    return searchNode(root->right, key);
}

/* Прямой обход: корень — левое — правое (NLR) */
void preorder(Node* root) {
    if (root == NULL) {
        return;
    }
    printNode(root);
    preorder(root->left);
    preorder(root->right);
}

/* Центрированный обход: левое — корень — правое (LNR)
   Для BST названия выходят по алфавиту. */
void inorder(Node* root) {
    if (root == NULL) {
        return;
    }
    inorder(root->left);
    printNode(root);
    inorder(root->right);
}

/* Обратный обход: левое — правое — корень (LRN) */
void postorder(Node* root) {
    if (root == NULL) {
        return;
    }
    postorder(root->left);
    postorder(root->right);
    printNode(root);
}

/* DFS через рекурсию. По заданию используем LNR. */
void dfs(Node* root) {
    inorder(root);
}

/* BFS через очередь (FIFO) */
void bfs(Node* root) {
    if (root == NULL) {
        return;
    }

    Node* queue[QUEUE_MAX];
    int front = 0;
    int rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        Node* current = queue[front++];
        printNode(current);

        if (current->left != NULL && rear < QUEUE_MAX) {
            queue[rear++] = current->left;
        }
        if (current->right != NULL && rear < QUEUE_MAX) {
            queue[rear++] = current->right;
        }
    }
}

void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int countNodes(Node* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}
