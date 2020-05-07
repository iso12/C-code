#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define NARRAY 8
#define NBUCKET 5 /* bucket size */
#define INTERVAL 10 /* bucket range */

struct Node
{
    int data;
    struct Node * next;
};

void BucketSort(int arr[])
struct Node * InsertioSort(struct Node *list);
void print(int arr[]);
void printBuckets(struct Node * list);
int getBucketIndex(int value);

void BucketSort(int arr[])
{
    int i,j;
    struct Node **buckets;

    buckets = (struct Node **) malloc(sizeof(struct Node*) NBUCKET)

    for(i = 0; i<NBUCKET; i++)
    {
        bucket[i] = NULL;

    }

    for 
}