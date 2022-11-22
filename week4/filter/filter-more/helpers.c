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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //temporarary copy of our image
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy [3][4] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int redX = 0;
            int greenX = 0;
            int blueX = 0;
            int redY = 0;
            int greenY = 0;
            int blueY = 0;

            //loop through each pixel to find neighboring pixel
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    if(i - 1 + x < 0 || i - 1 + x > height - 1 || j - 1 + y < 0 || j - 1 + y > width - 1)
                    {
                        continue;
                    }
                    //calculate Gx for each colour
                    redX =  redX + (image[i - 1 + x][j - 1 + y].rgbtRed * Gx[x][y]);
                    greenX =  greenX + (image[i - 1 + x][j - 1 + y].rgbtGreen * Gx[x][y]);
                    blueX =  blueX + (image[i - 1 + x][j - 1 + y].rgbtBlue * Gx[x][y]);


                    //calculate GY for each colour
                    redY =  redY + (image[i - 1 + x][j - 1 + y].rgbtRed * Gy[x][y]);
                    greenY =  greenY + (image[i - 1 + x][j - 1 + y].rgbtGreen * Gy[x][y]);
                    blueY =  blueY + (image[i - 1 + x][j - 1 + y].rgbtBlue * Gy[x][y]);
                }
            }

            // Calculate square root of Gx2 and Gy2
            int red = round(sqrt((redX * redX) + (redY * redY)));
            int green = round(sqrt((greenX * greenX) + (greenY * greenY)));
            int blue = round(sqrt((blueX * blueX) + (blueY * blueY)));

            // Cap value at 255 if that value exceed 255
            if (red > 255)
            {
                red = 255;
            }

            if (green > 255)
            {
                green = 255;
            }

            if (blue > 255)
            {
                blue = 255;
            }

            //Copy value into temp images
            temp[i][j].rgbtRed = red;
            temp[i][j].rgbtGreen = green;
            temp[i][j].rgbtBlue = blue;
        }
    }

    //copy new pixels into original yource image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}
