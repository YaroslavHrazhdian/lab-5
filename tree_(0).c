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

static int maxInt(int a, int b) {
    return (a > b) ? a : b;
}

static void copyData(Node* dest, const Node* src) {
    strcpy(dest->title, src->title);
    strcpy(dest->description, src->description);
    dest->is_completed = src->is_completed;
    strcpy(dest->deadline, src->deadline);
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

Node* findMin(Node* root) {
    if (root == NULL) {
        return NULL;
    }
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

Node* findMax(Node* root) {
    if (root == NULL) {
        return NULL;
    }
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}

int treeHeight(Node* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + maxInt(treeHeight(root->left), treeHeight(root->right));
}

int countNodes(Node* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

/* Удаление: лист / один потомок / два потомка (замена — минимум справа) */
Node* deleteNode(Node* root, const char* key) {
    if (root == NULL) {
        return NULL;
    }

    int cmp = strcmp(key, root->title);
    if (cmp < 0) {
        root->left = deleteNode(root->left, key);
        return root;
    }
    if (cmp > 0) {
        root->right = deleteNode(root->right, key);
        return root;
    }

    /* Случай 1: лист */
    if (root->left == NULL && root->right == NULL) {
        free(root);
        return NULL;
    }

    /* Случай 2: только один потомок */
    if (root->left == NULL) {
        Node* child = root->right;
        free(root);
        return child;
    }
    if (root->right == NULL) {
        Node* child = root->left;
        free(root);
        return child;
    }

    /* Случай 3: два потомка — берём минимальный из правого поддерева */
    Node* successor = findMin(root->right);
    copyData(root, successor);
    root->right = deleteNode(root->right, successor->title);
    return root;
}

void preorder(Node* root) {
    if (root == NULL) {
        return;
    }
    printNode(root);
    preorder(root->left);
    preorder(root->right);
}

void inorder(Node* root) {
    if (root == NULL) {
        return;
    }
    inorder(root->left);
    printNode(root);
    inorder(root->right);
}

void postorder(Node* root) {
    if (root == NULL) {
        return;
    }
    postorder(root->left);
    postorder(root->right);
    printNode(root);
}

void dfs(Node* root) {
    inorder(root);
}

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

static void writeNode(FILE* file, const Node* node) {
    fwrite(node->title, sizeof(char), STR_LEN, file);
    fwrite(node->description, sizeof(char), STR_LEN, file);
    fwrite(&node->is_completed, sizeof(int), 1, file);
    fwrite(node->deadline, sizeof(char), STR_LEN, file);
}

static void savePreorder(FILE* file, Node* root) {
    if (root == NULL) {
        return;
    }
    writeNode(file, root);
    savePreorder(file, root->left);
    savePreorder(file, root->right);
}

int saveTree(Node* root, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Не удалось открыть файл \"%s\" для записи.\n", filename);
        return 0;
    }

    int n = countNodes(root);
    fwrite(&n, sizeof(int), 1, file);
    savePreorder(file, root);
    fclose(file);
    printf("Сохранено узлов: %d в файл \"%s\"\n", n, filename);
    return 1;
}

static Node* readNode(FILE* file) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    if (fread(node->title, sizeof(char), STR_LEN, file) != STR_LEN ||
        fread(node->description, sizeof(char), STR_LEN, file) != STR_LEN ||
        fread(&node->is_completed, sizeof(int), 1, file) != 1 ||
        fread(node->deadline, sizeof(char), STR_LEN, file) != STR_LEN) {
        free(node);
        return NULL;
    }
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* loadTree(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Не удалось открыть файл \"%s\" для чтения.\n", filename);
        return NULL;
    }

    int n = 0;
    if (fread(&n, sizeof(int), 1, file) != 1 || n < 0) {
        printf("Файл повреждён.\n");
        fclose(file);
        return NULL;
    }

    Node* root = NULL;
    for (int i = 0; i < n; i++) {
        Node* node = readNode(file);
        if (node == NULL) {
            printf("Ошибка чтения узла.\n");
            freeTree(root);
            fclose(file);
            return NULL;
        }
        root = insertNode(root, node);
    }

    fclose(file);
    printf("Загружено узлов: %d из файла \"%s\"\n", n, filename);
    return root;
}
