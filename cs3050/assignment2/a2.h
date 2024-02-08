#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node Node;
struct Node {
    int data;
    int size;
    Node * left;
    Node * right;
};

typedef struct {
    Node * root;
    int size;
} Info;

typedef struct{
    Info * tree;
} RBST;


RBST initRBST();
int insertRBST(RBST, int key);
int freeRBST(RBST);

int freeNode(Node *);
Node * insertNode (Node *, Node *, int *);
int rearrangeTree(Node *, Node *);
int * store(Node *, int *);
Node * buildTree(int *, int, int);
void printTree(Node * node);

// test functions
int testInsertRBST(int n, int*);
int testFreeRBST(int n, int*);
int scalingTests(int n);

void randomInts(int n, int*);