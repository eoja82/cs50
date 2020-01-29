#include <stdio.h>
#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    printf("%i, %i, %i\n", image[0][0].rgbtBlue, image[0][0].rgbtGreen, image[0][0].rgbtRed);
    //need to convert to float to get decimal when dividing
    float ave = (float) (image[0][0].rgbtBlue + image[0][0].rgbtGreen + image[0][0].rgbtRed) / 3;
    printf("ave: %.3f\n", ave);
    int nc = round(ave);
    /* int new_color = average - floor(average) > 0.5 ? ceil(average) : floor(average); */
    printf("new_color: %i\n", nc);
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
    printf("%i, %i, %i\n", image[0][0].rgbtBlue, image[0][0].rgbtGreen, image[0][0].rgbtRed);
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
