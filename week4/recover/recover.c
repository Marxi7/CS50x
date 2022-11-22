#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover filename\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 2;
    }


    //storing blocks of 512 bytes in an array
    //why unsigned and not signed ? because unsigned can only store positive values
    unsigned char buffer[512];

    //this will help us track the number of images generated
    int image_count = 0;

    //file pointer for recovered images
    FILE *output = NULL;

    //char filename - 8 because one char is 8 bits. So we want to multiply the number of char per 8 bits.
    char *filename = malloc(8 * sizeof(char));


    //rweading the blocks of 512 bytes
    while (fread(buffer, sizeof(char), 512, input))
    {
        //checking if bytes are indicating the start of a jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //Writing the JPEG filenames
            sprintf(filename, "%03i.jpg", image_count);

            output = fopen(filename, "w");

            //Counting the number of images found
            image_count++;
        }
        //check if already found and used - if not, we will write the name found images jpegs into it.
        if (output != NULL)
        {
            fwrite(buffer, sizeof(char), 512, output);
        }

    }
    free(filename); //freeing the memory we asked for to the computer for filename with malloc
    fclose(output);
    fclose(input);

    return 0;
}