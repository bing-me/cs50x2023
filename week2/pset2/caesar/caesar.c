#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
//Get key
//accept single cmd line, non-negative int
//if argument is not 2, prompt usage.

    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

//if check arg for non-digits, prompt usage.

    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

//convert arg from ascii to int

    int k = atoi(argv[1]);

//get plaintext
//output "plaintext:  " and then prompt for get_string

    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");

//Encipher
//output "ciphertext: " each character rotated by k
//k should wrap around cap or non cap letters
//ci = (pi + k) % 26 and preserve caps
//print cipher

    for (int j = 0; j < strlen(plaintext); j++)
    {
        if (isupper(plaintext[j]))
        {
            printf("%c", (plaintext[j] - 'A' + k) % 26 + 'A');
        }
        else if (islower(plaintext[j]))
        {
            printf("%c", (plaintext[j] - 'a' + k) % 26 + 'a');
        }
        else if (!isupper(plaintext[j]) || !islower(plaintext[j]))
        {
            printf("%c", plaintext[j]);
        }
    }

//print new line, then exit

    printf("\n");

}