#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include "helpers.h"

int * get_color(int width, int height, int i, int j, RGBTRIPLE image[height][width], int colors[]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //need to convert to float to get decimal when dividing
    float ave = (float) (image[0][0].rgbtBlue + image[0][0].rgbtGreen + image[0][0].rgbtRed) / 3;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float average = (float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3;
            int new_color = round(average);
            image[i][j].rgbtBlue = new_color;
            image[i][j].rgbtGreen = new_color;
            image[i][j].rgbtRed = new_color;
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
    //store blurred colors in blurred
    RGBTRIPLE blurred[height][width];
    //populate blurred
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //colors array to pass and return to get_color
            int colors[3] = {0, 0, 0};
            int *new_color = get_color(width, height, i, j, image, colors);
            blurred[i][j].rgbtBlue = colors[0];
            blurred[i][j].rgbtGreen = colors[1];
            blurred[i][j].rgbtRed = colors[2];
        }
    }
    //change image to blurred
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = blurred[i][j].rgbtBlue;
            image[i][j].rgbtGreen = blurred[i][j].rgbtGreen;
            image[i][j].rgbtRed = blurred[i][j].rgbtRed;
        }
    }
    return;
}
int * get_color(int width, int height, int i, int j, RGBTRIPLE image[height][width], int colors[])
{
    //make sure not going off picture grid
    int imin = i - 1 > 0 ? i - 1 : 0;
    int imax = i + 1 < height - 1 ? i + 1 : height - 1;
    int jmin = j - 1 > 0 ? j - 1 : 0;
    int jmax = j + 1 < width - 1 ? j + 1 : width - 1; 
    //count adjacent pixels
    int counter = 0;
    //iterate over adjacent pixels
    for (int x = imin; x <= imax; x++)  
    {
        for (int y = jmin; y <= jmax; y++)
        {
            colors[0] += image[x][y].rgbtBlue;
            colors[1] += image[x][y].rgbtGreen;
            colors[2] += image[x][y].rgbtRed;
            counter++;
        } 
    }
    //compute average of adjacent pixel colors
    for (int k = 0; k < 3; k++)
    {
        colors[k] = round((float) colors[k] / counter);
    }
    
    return colors;
}