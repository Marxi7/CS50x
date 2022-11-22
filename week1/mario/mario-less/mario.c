#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height, row, column, spaces;
    do
    {
        height = get_int("What's the height of the pyramid ?\n");
    }
    while (height > 8 || height < 1);

    // This is  nested loop. A loop inside another loop. we first start with the fact of adding one row for each height introcuded.
    for (row = 0; row < height; row++)
    {
        //Then, we will take care of the spaces because they are printed before the #'s as they come in the left first.
        for (spaces = 0; spaces < height - row - 1; spaces++)
        {
            //printing one more space for each new row
            printf(" ");
        }
        //After that, we take care of creating the column. One more column will be added for each hmore height introduced.
        for (column = 0; column <= row; column++)
        {
            //then we print the #'s for each new column.
            printf("#");
        }
        //to make sure that we print these above in a new line, we need to use \n, otheriwse, everything will be on the same line and look like something like : # ## ### #### #####...
        printf("\n");
    }

}