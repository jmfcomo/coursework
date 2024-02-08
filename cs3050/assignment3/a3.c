#include "a3.h"

float findClusterExtreme(float array[], int n)
{
    // find min/max
    float min = array[0];
    float max = array[0];
    for (int i = 0; i < n; i++)
    {
        if (array[i] > max) // if new max
            max = array[i];
        if (array[i] < min) // if new min
            min = array[i];
    }
    // printf("data min = %f, data max = %f\n", min, max);

    // create buckets
    Bucket buckets[n-1];
    float bWidth = (max-min) / (n-1);
    float level = min;
    for (int i = 0; i < n-1; i++) // loop through each bucket
    {
        // set range of bucket
        buckets[i].bottom = level;
        level += bWidth;
        buckets[i].top = level;
        buckets[i].data = NULL;
        // printf("made bucket from %f to %f\n", buckets[i].bottom, buckets[i].top);
    }

    // fill buckets
    for (int i = 0; i < n; i++) // loop through values in array
    {
        int index = ((array[i] - min) / (max - min)) * (n-1); // find bucket

        if (index >= (n-1)) // if past last bucket
            index = n-2;

        // printf("inserting in %d\n", index);

        // insert into bucket
        if (buckets[index].data == NULL) // if bucket empty
        {
            buckets[index].data = createNode(array[i]);
        }
        else 
        {
            Node * nodePtr = buckets[index].data;
            while (nodePtr->next != NULL) // add to end of bucket linked list
            {
                nodePtr = nodePtr->next;
            }
            nodePtr->next = createNode(array[i]);
        }
        // printf("added %f to bucket from %f to %f\n", array[i], buckets[index].bottom, buckets[index].top);
    }

    // find smallest & largest keys
    for (int i = 0; i < n-1; i++) // loop through buckets
    {
        if (buckets[i].data != NULL) // if not empty
        {
            // set min/max to first node then adjust with others if needed
            buckets[i].min = buckets[i].data->data;
            buckets[i].max = buckets[i].data->data;
            Node * nodePtr = buckets[i].data;
            while (nodePtr != NULL) // loop through node linked list
            {
                if (nodePtr->data < buckets[i].min)
                    buckets[i].min = nodePtr->data;
                if (nodePtr->data > buckets[i].max)
                    buckets[i].max = nodePtr->data;
                nodePtr = nodePtr->next;
            }
        }
    }

    // compare largest key with smallest in next bucket.  track largest separation
    float widestMin = 0;
    float width = 0;
    for (int i = 0; i < n - 2; i++) // loop through buckets
    {
        int nextIndex = i+1; // bucket we compare to
        if (buckets[i].data != NULL) // if not empty
        {
            while (buckets[nextIndex].data == NULL) // if next bucket is empty, keep going
            {
                nextIndex++;
            }
            if ((buckets[nextIndex].min - buckets[i].max) > width) // check if widest gap
            {
                width = buckets[nextIndex].min - buckets[i].max;
                widestMin = buckets[i].max;
            }
        }
    }

    // clear 
    for (int i = 0; i < n-1; i++) // loop through buckets
    {
        if (buckets[i].data != NULL)
        {
            freeNode(buckets[i].data);
        }
    }
    
    return widestMin;
}


Node * createNode(float data)
{
    Node * new = malloc(sizeof(Node));
    if (new == NULL) // if malloc failed
    {
        return NULL;
    }
    new->data = data;
    new->next = NULL;
    return new;
}


void freeNode(Node * node)
{
    if (node->next != NULL) // loop through linked list
    {
        freeNode(node->next);
    }
    // printf("freeing node %f\n", node->data);
    free(node);
    node = NULL;
}


void randArray(int n, float * keys)
{   
    srand48(time(NULL)); //seed drand48()

    for (int i = 0; i < n; i++)
    { 
        keys[i] = ((drand48() * INT_MAX) + drand48()); // somewhat random?
        // printf("%f\n", keys[i]);
    }
}


int main(void)
{
    clock_t start, end;
    double execTime; 

    for (int i = 4; i < 66000; i *= 2) // for running increasingly large tests
    {
        double timeTotal = 0;
        for (int j = 0; j < 10; j++) // run each test 10 times, average results
        {
            float keys[i];
            randArray(i, keys);

            start = clock(); // time function execution
            findClusterExtreme(keys, i);
            end = clock();
            
            execTime = ((double)(end - start)) / CLOCKS_PER_SEC;
            timeTotal += execTime;
        }
        double timeAverage = timeTotal / 10.0;
        printf("N = %d, time = %.7f\n", i, timeAverage);
    }
}