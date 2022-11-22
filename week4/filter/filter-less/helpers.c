#include "helpers.h"
#include<math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //calculating the average of the rgb values and rounding them.
            //using (float) to transform them in float in order to use them with the fun round
            int average_color = round(((float)image[i][j].rgbtBlue + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtRed) / 3);

            image[i][j].rgbtBlue = average_color;
            image[i][j].rgbtGreen = average_color;
            image[i][j].rgbtRed = average_color;
        }
    }
    return;
}



// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float originalBlue = image[i][j].rgbtBlue;
            float originalGreen = image[i][j].rgbtGreen;
            float originalRed = image[i][j].rgbtRed;

            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtRed = sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) //we don't change anything here as only the column will change
    {
        //here we divide by 2 in order to stop at the half of the image.
        // If we don't stop at the half, we'll re inverse the pixels and end up at the same place
        // as the beginning.
        for (int j = 0; j < width / 2; j++)
        {
            // Reflect the pixels

            //why j+ 1 = eg :
            //0 1 2 3 4 -> position
            //4 5 6 7 8 -> numbers for the example

            //we change j (4) at position (0) with j (8) at position (4)
            // the position (4) is:  width (5) - (j (0) - 1)


            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //copy of the image
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int totalRed, totalBlue, totalGreen;
            totalRed = totalBlue = totalGreen = 0; //we set them to 0 to start
            float counter = 0;


            //this determine the neighbours pixels of the targeted pixel
            for (int x = - 1; x < 2; x++)
            {
                for (int y = - 1; y < 2; y++)
                {
                    int current_X = i + x;
                    int current_Y = j + y;

                    //check if surrounding pixels (neighbors) are valid
                    if (current_X < 0 || current_X > (height - 1) || current_Y < 0 || current_Y > (width - 1))
                    {
                        continue; //if it's invalid, this will skip it and continue instead of stopping everything
                    }

                    //get value for image - [current_X] and [current_Y] are the neighboring pixels infos.
                    totalRed += image[current_X][current_Y].rgbtRed;
                    totalGreen += image[current_X][current_Y].rgbtGreen;
                    totalBlue += image[current_X][current_Y].rgbtBlue;

                    counter++; //the counter will give us the amount of neighbors we found.
                }

                //average of the surrounding pixels
                temp[i][j].rgbtRed = round(totalRed / counter);
                temp[i][j].rgbtGreen = round(totalGreen / counter);
                temp[i][j].rgbtBlue = round(totalBlue / counter);

            }
        }
    }

    //copying the new pixels into original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //"replacing" the original pixels with the one we created above.
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}
