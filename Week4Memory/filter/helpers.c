#include <stdio.h>
#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // printf("%i, %i, %i\n", image[0][0].rgbtBlue, image[0][0].rgbtGreen, image[0][0].rgbtRed);
    //need to convert to float to get decimal when dividing
    float ave = (float) (image[0][0].rgbtBlue + image[0][0].rgbtGreen + image[0][0].rgbtRed) / 3;
    //printf("ave: %.3f\n", ave);
    int nc = round(ave);
    // int new_color = average - floor(average) > 0.5 ? ceil(average) : floor(average);
    //printf("new_color: %i\n", nc);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //need to convert one int to float to get decimal when dividing
            float average = (float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3;
            int new_color = round(average);
            image[i][j].rgbtBlue = new_color;
            image[i][j].rgbtGreen = new_color;
            image[i][j].rgbtRed = new_color;
        }
    }
    //printf("%i, %i, %i\n", image[0][0].rgbtBlue, image[0][0].rgbtGreen, image[0][0].rgbtRed);
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{   
    /* printf("%i, %i, %i\n", image[0][0].rgbtBlue, image[0][0].rgbtGreen, image[0][0].rgbtRed);
    int b = image[0][0].rgbtBlue;
    int g = image[0][0].rgbtGreen;
    int r = image[0][0].rgbtRed;
    int newr = round((.393 * r) + (.769 * g) + (.189 * b));
    printf("newr: %i\n", newr); */
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int blue = image[i][j].rgbtBlue;
            int green = image[i][j].rgbtGreen;
            int red = image[i][j].rgbtRed;
            int new_blue = round((.272 * red) + (.534 * green) + (.131 * blue));
            int new_green = round((.349 * red) + (.686 * green) + (.168 * blue));
            int new_red = round((.393 * red) + (.769 * green) + (.189 * blue));
            image[i][j].rgbtBlue = new_blue < 255 ? new_blue : 255;
            image[i][j].rgbtGreen = new_green < 255 ? new_green : 255;
            image[i][j].rgbtRed = new_red < 255 ? new_red : 255;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //printf("w: %i, h: %i\n", width, height);
    for (int i = 0; i < height; i++)
    {
        int half_width = width / 2;
        for (int j = 0; j < half_width; j++)
        {
            int tmp_blue = image[i][j].rgbtBlue;
            int tmp_green = image[i][j].rgbtGreen;
            int tmp_red = image[i][j].rgbtRed;
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][width - 1 - j].rgbtBlue = tmp_blue;
            image[i][width - 1 - j].rgbtGreen = tmp_green;
            image[i][width - 1 - j].rgbtRed = tmp_red;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
