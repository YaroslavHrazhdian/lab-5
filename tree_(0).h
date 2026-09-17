#ifndef TREE_H
#define TREE_H

#define STR_LEN 100
#define QUEUE_MAX 256
#define DEFAULT_FILENAME "tree.dat"

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
Node* deleteNode(Node* root, const char* key);

Node* findMin(Node* root);
Node* findMax(Node* root);
int treeHeight(Node* root);
int countNodes(Node* root);

void printNode(const Node* node);
void preorder(Node* root);
void inorder(Node* root);
void postorder(Node* root);
void dfs(Node* root);
void bfs(Node* root);

int saveTree(Node* root, const char* filename);
Node* loadTree(const char* filename);

void freeTree(Node* root);

#endif
