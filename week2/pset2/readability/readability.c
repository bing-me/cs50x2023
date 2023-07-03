#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
//get string
    string text = get_string("Text: ");


    int l = 0;
    int sp = 0;
    int ep = 0;
    int hy = 0;

    for
    (int i = 0; i <= strlen(text); i++)
    {
        //check if text[i] is an alphabet
        if (isalpha(text[i]))
        {
            ++l;
        }
        //check if text[i] is a blank
        if (isblank(text[i]))
        {
            ++sp;
        }
        //check if text[i] is a ', ? !'
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            ++ep;
        }
        //check if text[i] has hyphenated words
        if (text[i] == '-')
        {
            ++hy;
        }
    }

    float w = sp + 1 - hy;

//Coleman-liau index | index = 0.0588 * L - 0.296 * S - 15.8
    float L = l * 100 / w;
    float S = ep * 100 / w;
    float cli = (0.0588 * L) - (0.296 * S) - 15.8;
    int clir = round(cli);

//print grade | if less than 1, state so and if above 16, 16+.

    if (clir < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (1 <= clir && clir <= 16)
    {
        printf("Grade %i\n", clir);
    }
    else if (clir > 16)
    {
        printf("Grade 16+\n");
    }
}
