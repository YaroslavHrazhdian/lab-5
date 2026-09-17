#ifndef TREE_H
#define TREE_H

#define STR_LEN 100
#define QUEUE_MAX 256

typedef struct Node {
    char title[STR_LEN];
    char description[STR_LEN];
    int is_completed;
    char deadline[STR_LEN];
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(void);
Node* insertNode(Node* root, Node* newNode);
Node* searchNode(Node* root, const char* key);

void printNode(const Node* node);
void preorder(Node* root);
void inorder(Node* root);
void postorder(Node* root);
void dfs(Node* root);
void bfs(Node* root);
void freeTree(Node* root);
int countNodes(Node* root);

#endif
