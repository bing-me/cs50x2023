// Practice writing a function to find a max value

#include <cs50.h>
#include <stdio.h>
#include <math.h>

int max(int array[], int n);

int main(void)
{
    int n;
    do
    {
        n = get_int("Number of elements: ");
    }
    while (n < 1);

    int arr[n];

    for (int i = 0; i < n; i++)
    {
        arr[i] = get_int("Element %i: ", i);
    }

    printf("%i\n", max(arr, n));
}

// TODO: return the max value
int max(int array[], int n)
{
    int max = fmax(array[0], array[1]);

    for (int i = 0; i < n; i++)
    {
        max = fmax(max, array[i]);
    }
    return max;
}
