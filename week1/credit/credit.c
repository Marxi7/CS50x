#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //First we ask the user to input the credit card number using the get_long fun as we will use long numbers and need to be precise.
    long card = get_long("Card Number: ");

    //counting the number of the card
    int length = 0;
    long cc = card;
    while (cc > 0)
    {
        cc = cc / 10;
        length++;
    }
    //Check and verifiy the length which should be 13, 15 or 16
    if (length != 13 && length != 15 && length != 16)
    {
        printf("INVALID\n");
        return 0;
    }
    //calculate checksum
    long x = card; //creating a new copy of card that we will use to calculate our mod and our sums
    int total = 0;


    //sums that we will later addition
    int sum1 = 0;
    int sum2 = 0;

    //digit 1, 2, mod 1,2
    int mod1, mod2, d1, d2;


    do
    {
        //Remove last digit and add to sum1
        mod1 = x % 10; //we take the last number
        x = x / 10; //we knoff of the remaining last number now for the next operation as we take 1 number out of 2..
        sum1 = sum1 + mod1;

        //Remove second last digit
        mod2 = x % 10; //we take the last number
        x = x / 10; //we knoff of the remaining last number now for the next operation as we take 1 number out of 2

        //we multiply by 2 the second last digit and add digits to sum2
        mod2 = mod2 * 2;

        d1 = mod2 % 10; 
        d2 = mod2 / 10;
        sum2 = sum2 + d1 + d2;
    }
    while (x > 0);

    total = sum1 + sum2;

    //Checking with the Luhn Algorithm given
    if (total % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // Get the starting digit to then check card type
    long start = card;
    do
    {
        start = start / 10;
    }
    while (start > 100);

    //Checking if Mastercard -> 16 digits and starts with either 51, 52. 53, 54, 55

    if ((start / 10 == 5) && (0 < start % 10 && start % 10 < 6))
    {
        printf("MASTERCARD\n");
    }

    //Checking if american express -> 15 digits and starts with 34 or 37
    else if ((start / 10 == 3) && (start % 10 == 4 || start % 10 == 7))
    {
        printf("AMEX\n");
    }

    //Checking if it is a visa card, which should either have have 13 or 16 digits and starts with the number 4.
    else if (start / 10 == 4)
    {
        printf("VISA\n");
    }

    //If all the above fail, we return invalid
    else
    {
        printf("INVALID\n");
    }

}