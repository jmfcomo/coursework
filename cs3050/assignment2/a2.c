#include "a2.h"

RBST initRBST()
{
    srand48(time(NULL)); //seed drand48()

    // make blank tree
    Info * tree;
    tree = malloc(sizeof(Info));
    if (tree == NULL) // if malloc failed
    {
        RBST nullTree;
        nullTree.tree = NULL;        
        return nullTree;
    }
    tree->root = NULL;
    tree->size = 0;

    // wrap in RBST wrapper
    RBST new;
    new.tree = tree;
    return new;
}


int insertRBST(RBST tree, int key)
{
    // create new node
    Node * new;
    new = malloc(sizeof(Node));
    if (new == NULL) // if malloc failed
        return 0;
    new->data = key;
    new->left = NULL;
    new->right = NULL;
    new->size = 1;

    if (tree.tree->root == NULL) // if tree is empty
    {   
        // printf("tree was empty, inserting node at root \n");
        tree.tree->root = new;
        tree.tree->size = 1;
        return 1;
    }

    // otherwise, add node to tree
    tree.tree->size++;
    int nodesVisited = 0;
    tree.tree->root = insertNode(tree.tree->root, new, &nodesVisited);
    printTree(tree.tree->root);
    return nodesVisited;
}


Node * insertNode (Node * node, Node * new, int * count)
{
    if (drand48() < (1.0/(node->size + 1))) // random reconstruction
    {
        // printf("rearranging tree \n from root %d to %d\n", node->data, new->data);
        *count += rearrangeTree(node, new);
        return new;
    }
    if (new->data < node->data) // insert to left
    {
        node->size++;
        if (node->left == NULL) // place directly to left
        {
            // printf("inserting to the left from node %d with key %d\n", node->data, new->data);
            node->left = new;
            *count += 2;
            return node;
        }
        // printf("going left from node %d to %d with key %d\n", node->data, node->left->data, new->data);
        *count += 1;
        node->left = insertNode(node->left, new, count); // move down to the left
        return node;
    }
    if (new->data > node->data) // insert to right
    {
        node->size++;
        if (node->right == NULL) // place directly to right
        {
            // printf("inserting to the right from node %d with key %d\n", node->data, new->data);
            node->right = new;
            *count += 2;
            return node;
        }
        // printf("going right from node %d to %d with key %d\n", node->data, node->right->data, new->data);
        *count += 1;
        node->right = insertNode(node->right, new, count); // move down to the right
        return node;
    }
}


int freeRBST(RBST tree)
{
    int total = 0; // tracks nodes freed
    if (tree.tree->root)
        total = freeNode(tree.tree->root); // free each node
    free(tree.tree); // free RBST itself
    tree.tree = NULL;
    return total;
}


int rearrangeTree(Node * node, Node * new)
{
    int size = node->size;

    int * treeArray = malloc(sizeof(int) * size);
    if (treeArray == NULL) // if malloc failed
        return 0;

    // hold all data values in an array
    store(node, treeArray);
    // free old nodes
    freeNode(node);

    // find index value where new root is located
    int k = 0;
    for (int i = 0; i < size; i++)
        if (treeArray[i] < new->data)
            k = i+1;

    // reconstruct new tree
    if (k > 0)
        new->left = buildTree(treeArray, 0, k); // add nodes to left
    if (k < size)
        new->right = buildTree(treeArray, k, size);  // add nodes to right

    free(treeArray);
    treeArray = NULL;
    new->size = size + 1;
    return size + 1;
}


int * store(Node * node, int * array)
{   
    if (node == NULL)
        return array;

    if (node->left) // iterate down left tree
        array = store(node->left, array);

    // store current node
    *array = node->data;
    array++;

    if (node->right) // iterate down right tree
        array = store(node->right, array);
    return array;
}


int freeNode(Node * n)
{
    int total = 1;
    if (n->left) // free left tree
        total += freeNode(n->left);
    if (n->right) // free right tree
        total += freeNode(n->right);
    free(n); // free current node
    n = NULL;
    return total;
}


Node * buildTree(int * array, int first, int last)
{
    // printf("building tree from %d to %d\n", first, last);
    if (last < first)  // cancelling tree build
    {
        return NULL;
    } 

    // pick random index k to be next node inserted
    int k = (drand48() * (last - first)) + first;

    // create new node from index k
    Node * new;
    new = malloc(sizeof(Node));
    if (new == NULL) // if malloc failed
        return NULL;
    new->data = array[k];
    new->left = NULL;
    new->right = NULL;
    new->size = last-first;

    if (k > first)  // build to the left
    {
        new->left = buildTree(array, first, k);
    }

    if (k+1 < last) // build to the right
    {
        new->right = buildTree(array, k + 1, last);
    }

    return new;
}


void printTree(Node * node)
{
    int left = 0;
    int right = 0;
    if (node->left)
        left = node->left->data;
    if (node->right)
        right = node->right->data;
    // printf("Node %d with children %d and %d, size %d\n", node->data, left, right, node->size);
    if (node->left)
        printTree(node->left);
    if (node->right)
        printTree(node->right);
}


// test functions
int testInsertRBST(int n, int* keys)
{
    RBST tree = initRBST(); // init tree
    int nodesVisited = 0;

    // run all insertions
    for (int i = 0; i < n; i++)
    {
        nodesVisited += insertRBST(tree, keys[i]);
    }

    // free tree
    freeRBST(tree);
    return nodesVisited;
}


int testFreeRBST(int n, int* keys)
{
    //init tree
    RBST tree = initRBST();
    int nodesVisited = 0;
    
    // run all insertions
    for (int i = 0; i < n; i++)
    {
        insertRBST(tree, keys[i]);
    }
    // free tree
    nodesVisited = freeRBST(tree);
    return nodesVisited;
}


void randomInts(int n, int * keys)
{   
    for (int i = 0; i < n; i++)
    {   // random ints - can be duplicates
        // keys[i] = ((drand48() * 100000));

        // sequential ints - no duplicates
        keys[i] = i;
    }
}


int scalingTests(int n)
{
    int keys[n];
    randomInts(n, keys); // get a dataset
    printf("%d Nodes: Inserting %d, Freeing %d\n", n, testInsertRBST(n, keys), testFreeRBST(n, keys));
    return 1;
}


int main()
{
    scalingTests(1000);
    scalingTests(2000);
    scalingTests(4000);  
    scalingTests(8000);
    scalingTests(16000);
    scalingTests(32000);
    scalingTests(64000);
    scalingTests(128000); 
    
}