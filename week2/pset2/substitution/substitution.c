#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
//get key
//key has to be 26 letters only, no repeats
//1st check if key is less than 26 letters, "Key must contain 26 characters."
//if key is repeated, "Key must not contain repeated characters."
//if key has non-alphabets, "Key must only contain aphabetic characters"

    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    int len = strlen(argv[1]);
    int i = 0;

    for (i = 0; i < len; i++)
    {
        if (len != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        else if (!isalpha(argv[1][i]))
        {
            printf("Key must only contain aphabetic characters.\n");
            return 1;
        }
        for (int j = i + 1; j < len; j++)
            if (argv[1][i] == argv[1][j])
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
    }

//conver key to uppercase
    for (int k = 0; k < len; k++)
    {
        if (islower(argv[1][k]))
        {
            argv[1][k] = argv[1][k] - 32;
        }
    }

//get plaintext

    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");

//encipher text
//match lower case to array
//print ciphered text with case condition

    for (int l = 0; l < strlen(plaintext); l++)
    {
        if (isupper(plaintext[l]))
        {
//since key is upper case, reference array argv[1] directly from uppercase text
            int ctext = plaintext[l] - 65;
            printf("%c", argv[1][ctext]);
        }
        else if (islower(plaintext[l]))
        {
//since key is lower, add back the ascii value after refernecing array
            int ctext = plaintext[l] - 97;
            printf("%c", argv[1][ctext] + 32);
        }
        else
        {
            printf("%c", plaintext[l]);
        }
    }
//print and exit
    printf("\n");
}
