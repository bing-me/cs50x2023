#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h ;
    //for selecting height

    do
    {
        h = get_int("Height: ");
    }
    while (h < 1 || h > 8);

    int i ;
    //for rows

    int j ;
    //for columns

    //for (initialization; condition; update)
    //makes rows i times up to h
    for (i = 0 ; i < h ; i++)
    {
        //makes columns or multiples for spacing
        //prints [space] each time i<h, decreases downwards as j increases and gets closer to h
        for (j = i ; j < h - 1 ; j++)
        {
            printf(" ");
        }
        //makes columns or multiples for #
        //prints # each time h-i<h+1, increases downwards as h-1 decreases
        for (j = h - i ; j < h + 1 ; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}
