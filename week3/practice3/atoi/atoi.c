#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int convert(string input);

int main(void)
{
    string input = get_string("Enter a positive integer: ");

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Invalid Input!\n");
            return 1;
        }
    }

    // Convert string to int
    printf("%i\n", convert(input));
}

int convert(string input)
{
    // TODO
    //Start by getting the index of the last char in the string (the char before the \0).

    int result = 0;//stores and returns int value
    int i = 0;//position from the back/current character
    int sign = 1;//to determins if '+' or '-'
    if(input[i] == '-')
    {
        sign = -1;
        i++;
    }

    //Convert this char into its numeric value. Can you subtract some char to do this?
    //Remove the last char from the string by moving the null terminator one position to the left.
    //Return this value plus 10 times the integer value of the new shortened string.
    for(;input[i] != '\0'; i++)
    {
    result = result*10 + input[i] -'0';
    }

    //Remember you need a base case when creating a recursive function.

    return result*sign;

}