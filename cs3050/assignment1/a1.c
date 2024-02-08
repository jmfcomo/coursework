#include "a1.h"

int obtainRandomSample (int *sequence)
{
    int rand;

    for (int i = 1; sequence[i-1] != INT_MAX; i++) // for every value in the array
    {
        if (drand48() < (1.0/i)) // 1/i chance of being true
        {
            rand = sequence[i-1]; // swap out chosen rand
        }
    }
    return rand;


  //Sample biased random function
    // int rand;
    // for (int i = 1; sequence[i-1] != INT_MAX; i++)
    // {
    //     if (drand48() < 0.5)
    //     {
    //         rand = sequence[i-1];
    //     }
    // }
    // return rand;
}


int testRandomSample()
{
    srand48(time(NULL)); //seed drand48()

    // file opening
    FILE * fp = fopen("test.txt", "r");
    if (fp == NULL) // if there are file errors
    {
        fprintf(stderr, "Test file cannot be opened\n");
        return 2; // error for files
    }

    char fileLine[1000]; // will hold each test line
    int testNums[500]; // holds test ints converted from file
    int numCount; // keeps track of number of ints (for test only)
    int trials = 1000000; // how many times to run the test, can be changed
    char *token; // breaks up the test line into individual numbers
    long long total; // holds the sum of all random selections
    float actualMean; // holds calculated mean
    int predTotal; // all test values added together
    float predMean; // average between test files

    while (fgets(fileLine, 999, fp) != NULL) // will loop for each test line
    {
        // setting everything back to 0
        total = 0;
        numCount = 0;
        actualMean = 0.0;
        predTotal = 0;
        predMean = 0.0;

        // breaking up tests where spaces are
        token = strtok(fileLine, " ");
        while (token != NULL) //for each test value
        {
            //convert char string to int, save in testNums
            testNums[numCount] = atoi(token);
            numCount++;
            token = strtok(NULL, " ");
        }
        numCount--; //to account for int_max value

        // run the random function, as many times as needed
        for (int i = 0; i < trials; i++)
        {
            total += obtainRandomSample(testNums); // add all results to total
        }

        // calculate actualMean
        actualMean = (total * 1.0) / trials; 

        // calculate predicted mean
        for (int i = 0; i < numCount; i++)
        {
            predTotal += testNums[i];
            testNums[i] = 0;
        }
        predMean = (predTotal * 1.0) / numCount;

        // compare actualMean to predicted mean, to within 2%
        if (abs(actualMean) > abs(predMean * 1.02) || 
            abs(actualMean) < abs(predMean * 0.98))
        {
            // test failed
            fclose(fp);
            printf("test failed\n");
            return 1;
        }
    }

    // we only get here if all tests pass
    fclose(fp);
    printf("test passed\n");
    return 0;
}


int main()
{
    testRandomSample();
}