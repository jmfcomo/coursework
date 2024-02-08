#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

typedef struct Node Node;
struct Node {
    float data;
    Node * next;
};

typedef struct{
    float bottom;
    float top;
    float min;
    float max;
    Node * data;
} Bucket;

float findClusterExtreme(float array[], int n);
Node * createNode (float);
void freeNode(Node * node);
void randArray(int n, float * keys);