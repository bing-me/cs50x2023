#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    //prompt for input checking card length

    long C;

    do
    {
        C = get_long("Credit card number: ");
    }
    while (C < (long) round(pow(10, 9)) || C > (long) round(pow(10, 17)));

    //calculate checksum

    //get even digits from last
    int second = C % (int) round(pow(10, 2)) / (int) round(pow(10, 1));

    int secondtwo;

    if (2 * second < 10)
    {
        secondtwo = 2 * second;
    }
    else
    {
        secondtwo = 2 * second % 10 + 2 * second % (int) round(pow(10, 2)) / (int) round(pow(10, 1));
    }

    int fourth = C % (int) round(pow(10, 4)) / (int) round(pow(10, 3));

    int fourthtwo;

    if (2 * fourth < 10)
    {
        fourthtwo = 2 * fourth;
    }
    else
    {
        fourthtwo = 2 * fourth % 10 + 2 * fourth % (int) round(pow(10, 2)) / (int) round(pow(10, 1));
    }

    int sixth = C % (int) round(pow(10, 6)) / (int) round(pow(10, 5));

    int sixthtwo;

    if (2 * sixth < 10)
    {
        sixthtwo = 2 * sixth;
    }
    else
    {
        sixthtwo = 2 * sixth % 10 + 2 * sixth % (int) round(pow(10, 2)) / (int) round(pow(10, 1));
    }

    int eighth = C % (int) round(pow(10, 8)) / (int) round(pow(10, 7));

    int eighthtwo;

    if (2 * eighth < 10)
    {
        eighthtwo = 2 * eighth;
    }
    else
    {
        eighthtwo = 2 * eighth % 10 + 2 * eighth % (int) round(pow(10, 2)) / (int) round(pow(10, 1));
    }

    int tenth = C % (long) round(pow(10, 10)) / (long) round(pow(10, 9));

    int tenthtwo;

    if (2 * tenth < 10)
    {
        tenthtwo = 2 * tenth;
    }
    else
    {
        tenthtwo = 2 * tenth % 10 + 2 * tenth % (int) round(pow(10, 2)) / (int) round(pow(10, 1));
    }

    int twelfth = C % (long) round(pow(10, 12)) / (long) round(pow(10, 11));


    int twelfthtwo;

    if (2 * twelfth < 10)
    {
        twelfthtwo = 2 * twelfth;
    }
    else
    {
        twelfthtwo = 2 * twelfth % 10 + 2 * twelfth % (int) round(pow(10, 2)) / (int) round(pow(10, 1));
    }


    int fourteenth = C % (long) round(pow(10, 14)) / (long) round(pow(10, 13));

    int fourteenthtwo;

    if (2 * fourteenth < 10)
    {
        fourteenthtwo = 2 * fourteenth;
    }
    else
    {
        fourteenthtwo = 2 * fourteenth % 10 + 2 * fourteenth % (int) round(pow(10, 2)) / (int) round(pow(10, 1));
    }

    int sixteenth = C % (long) round(pow(10, 16)) / (long) round(pow(10, 15));

    int sixteenthtwo;

    if (2 * sixteenth < 10)
    {
        sixteenthtwo = 2 * sixteenth;
    }
    else
    {
        sixteenthtwo = 2 * sixteenth % 10 + 2 * sixteenth % (int) round(pow(10, 2)) / (int) round(pow(10, 1));
    }

    //sum of even *2 digits

    int even =  secondtwo + fourthtwo + sixthtwo + eighthtwo + tenthtwo + twelfthtwo + fourteenthtwo + sixteenthtwo;

    //sum of odd digits

    int first = C % 10;

    int third = C % (int) round(pow(10, 3)) / (int) round(pow(10, 2));

    int fifth = C % (int) round(pow(10, 5)) / (int) round(pow(10, 4));

    int seventh = C % (int) round(pow(10, 7)) / (int) round(pow(10, 6));

    int ninth = C % (long) round(pow(10, 9)) / (long) round(pow(10, 8));

    int eleventh = C % (long) round(pow(10, 11)) / (long) round(pow(10, 10));

    int thirteenth = C % (long) round(pow(10, 13)) / (long) round(pow(10, 12));

    int fifthteenth = C % (long) round(pow(10, 15)) / (long) round(pow(10, 14));

    int odd = first + third + fifth + seventh + ninth + eleventh + thirteenth + fifthteenth;
    //sum of even + sum of odd

    int luhn = even + odd;

    //check that the last digit of the sum is 0

    int Cluhn = 0;

    if (luhn % 10 == 0)
    {
        Cluhn++;
    }

    //establish number of digits and 1st and 2nd digit for verification

    int digits = 0;

    while (C > 0)
    {
        C = C / 10;
        digits++;
    }

    int digitone = 0;
    int digittwo = 0;

    if (sixteenth > 0)
    {
        digitone = sixteenth;
        digittwo = fifthteenth;
    }
    else if (fifthteenth > 0)
    {
        digitone = fifthteenth;
        digittwo = fourteenth;
    }
    else if (fourteenth > 0)
    {
        digitone = fourteenth;
        digittwo = thirteenth;
    }
    else if (thirteenth > 0)
    {
        digitone = thirteenth;
        digittwo = twelfth;
    }

    //Print AMEX, MASTERCARD, VISA,
    //visa 13/16 digits, start with 4
    if (Cluhn > 0 && (digits == 13 || digits == 16) && digitone == 4)
    {
        printf("VISA\n");
    }
    //AMEX 15 digits, starts with 34 or 37
    else if (Cluhn > 0 && digits == 15 && digitone == 3 && (digittwo == 4 || digittwo == 7))
    {
        printf("AMEX\n");
    }
    //MASTERCARD  16 digits, starts with 51, 52, 53, 54, 55
    else if (Cluhn > 0 && digits == 16 && digitone == 5 && (0 < digittwo && digittwo < 6))
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
}