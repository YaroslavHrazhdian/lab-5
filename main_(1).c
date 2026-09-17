#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "tree.h"

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

static void printMenu(int count) {
    printf("\r\n--- Бинарное дерево задач (BST) ---");
    printf("\r\nУзлов в дереве: %d", count);
    printf("\r\n1. Добавить задачу");
    printf("\r\n2. Найти задачу по названию");
    printf("\r\n3. Прямой обход (preorder, NLR)");
    printf("\r\n4. Центрированный обход (inorder, LNR)");
    printf("\r\n5. Обратный обход (postorder, LRN)");
    printf("\r\n6. Обход в глубину (DFS)");
    printf("\r\n7. Обход в ширину (BFS)");
    printf("\r\n0. Выход");
    printf("\r\nВыбор: ");
}

static void setupConsole(void) {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF-8");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &mode)) {
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#else
    setlocale(LC_ALL, "ru_RU.UTF-8");
#endif
}

static void printHeader(const char* title) {
    printf("\r\n%s\r\n", title);
    printf("Название | Описание | Статус | Срок\r\n");
    printf("----------------------------------------\r\n");
}

int main(void) {
    setupConsole();

    Node* root = NULL;
    int choice;

    do {
        printMenu(countNodes(root));
        choice = readInt();

        switch (choice) {
            case 1:
                root = insertNode(root, createNode());
                break;

            case 2: {
                if (root == NULL) {
                    printf("Дерево пусто.\n");
                    break;
                }
                char key[STR_LEN];
                printf("Введите название для поиска: ");
                fgets(key, STR_LEN, stdin);
                key[strcspn(key, "\n")] = 0;

                Node* found = searchNode(root, key);
                if (found == NULL) {
                    printf("Задача не найдена.\n");
                } else {
                    printf("Найдена задача:\n");
                    printNode(found);
                }
                break;
            }

            case 3:
                if (root == NULL) {
                    printf("Дерево пусто.\n");
                    break;
                }
                printHeader("Прямой обход (preorder, NLR)");
                preorder(root);
                break;

            case 4:
                if (root == NULL) {
                    printf("Дерево пусто.\n");
                    break;
                }
                printHeader("Центрированный обход (inorder, LNR)");
                inorder(root);
                break;

            case 5:
                if (root == NULL) {
                    printf("Дерево пусто.\n");
                    break;
                }
                printHeader("Обратный обход (postorder, LRN)");
                postorder(root);
                break;

            case 6:
                if (root == NULL) {
                    printf("Дерево пусто.\n");
                    break;
                }
                printHeader("Обход в глубину (DFS), используется LNR");
                dfs(root);
                break;

            case 7:
                if (root == NULL) {
                    printf("Дерево пусто.\n");
                    break;
                }
                printHeader("Обход в ширину (BFS, очередь FIFO)");
                bfs(root);
                break;

            case 0:
                freeTree(root);
                root = NULL;
                printf("Дерево удалено из памяти. Выход.\n");
                break;

            default:
                printf("Неверный ввод.\n");
        }
    } while (choice != 0);

    return 0;
}
