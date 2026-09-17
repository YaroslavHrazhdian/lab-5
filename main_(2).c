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

static void readLine(const char* prompt, char* buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

static void printMenu(int count) {
    printf("\r\n--- BST: расширенные операции ---");
    printf("\r\nУзлов в дереве: %d", count);
    printf("\r\n1. Добавить задачу");
    printf("\r\n2. Найти задачу");
    printf("\r\n3. Удалить задачу");
    printf("\r\n4. Минимальный элемент");
    printf("\r\n5. Максимальный элемент");
    printf("\r\n6. Высота дерева");
    printf("\r\n7. Прямой обход (NLR)");
    printf("\r\n8. Центрированный обход (LNR)");
    printf("\r\n9. Обратный обход (LRN)");
    printf("\r\n10. Сохранить дерево в файл");
    printf("\r\n11. Загрузить дерево из файла");
    printf("\r\n0. Выход (дерево удаляется из памяти)");
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

static int requireTree(Node* root) {
    if (root == NULL) {
        printf("Дерево пусто.\n");
        return 0;
    }
    return 1;
}

int main(void) {
    setupConsole();

    Node* root = NULL;
    int choice;
    char key[STR_LEN];

    do {
        printMenu(countNodes(root));
        choice = readInt();

        switch (choice) {
            case 1:
                root = insertNode(root, createNode());
                break;

            case 2:
                if (!requireTree(root)) {
                    break;
                }
                readLine("Введите название для поиска: ", key, STR_LEN);
                {
                    Node* found = searchNode(root, key);
                    if (found == NULL) {
                        printf("Задача не найдена.\n");
                    } else {
                        printf("Найдена задача:\n");
                        printNode(found);
                    }
                }
                break;

            case 3:
                if (!requireTree(root)) {
                    break;
                }
                readLine("Введите название для удаления: ", key, STR_LEN);
                if (searchNode(root, key) == NULL) {
                    printf("Задача не найдена, удалять нечего.\n");
                } else {
                    root = deleteNode(root, key);
                    printf("Узел \"%s\" удалён.\n", key);
                }
                break;

            case 4:
                if (!requireTree(root)) {
                    break;
                }
                printf("Минимальный элемент:\n");
                printNode(findMin(root));
                break;

            case 5:
                if (!requireTree(root)) {
                    break;
                }
                printf("Максимальный элемент:\n");
                printNode(findMax(root));
                break;

            case 6:
                printf("Высота дерева: %d\n", treeHeight(root));
                break;

            case 7:
                if (!requireTree(root)) {
                    break;
                }
                printHeader("Прямой обход (NLR)");
                preorder(root);
                break;

            case 8:
                if (!requireTree(root)) {
                    break;
                }
                printHeader("Центрированный обход (LNR)");
                inorder(root);
                break;

            case 9:
                if (!requireTree(root)) {
                    break;
                }
                printHeader("Обратный обход (LRN)");
                postorder(root);
                break;

            case 10:
                saveTree(root, DEFAULT_FILENAME);
                break;

            case 11:
                freeTree(root);
                root = loadTree(DEFAULT_FILENAME);
                if (root != NULL) {
                    printHeader("Дерево после загрузки (LNR)");
                    inorder(root);
                }
                break;

            case 0:
                freeTree(root);
                root = NULL;
                printf("Дерево полностью удалено из памяти. Выход.\n");
                break;

            default:
                printf("Неверный ввод.\n");
        }
    } while (choice != 0);

    return 0;
}
