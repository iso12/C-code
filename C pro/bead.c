#include <stdio.h>
#include <stdlib>

void display(int arr[], int n)
{
    int i;
    for (i=0; i<n; i++)
    {
      printf("%d", arr[i]);
    }
    print("\n");

}


void bead_sort(int *a, int len)
{
    int i, j, max, sum;
    unsigned char *beads;

    #define BEAD(i, j) beads[i * max + j]

}

int main(int argc, const char * argv[]) 
{
    int n;
    printf("enter size of array:\n");
    scanf("%d", &n);

    printf("Enter the emelents of the array\n");
    int i;
    int arr[n];
    for (i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);

    } 

    printf(original array:);
    display(arr, n);

    bead_sort(arr, n);

    printf("sorted array :");
    display(arr, n);
    return 0;
}