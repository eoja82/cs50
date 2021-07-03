#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include "helpers.h"

int *get_color(int width, int height, int i, int j, RGBTRIPLE image[height][width], int colors[]);
int *get_edge(int width, int height, int i, int j, RGBTRIPLE image[height][width], int colors[]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //need to convert to float to get decimal when dividing
    float ave = (float)(image[0][0].rgbtBlue + image[0][0].rgbtGreen + image[0][0].rgbtRed) / 3;
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //store edge colors
    RGBTRIPLE edges[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int colors[3] = {0, 0, 0};
            int *new_color = get_edge(width, height, i, j, image, colors);
            edges[i][j].rgbtBlue = colors[0];
            edges[i][j].rgbtGreen = colors[1];
            edges[i][j].rgbtRed = colors[2];
        }
    }
    //change image to edges
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = edges[i][j].rgbtBlue;
            image[i][j].rgbtGreen = edges[i][j].rgbtGreen;
            image[i][j].rgbtRed = edges[i][j].rgbtRed;
        }
    }
    return;
}

int *get_color(int width, int height, int i, int j, RGBTRIPLE image[height][width], int colors[])
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
int *get_edge(int width, int height, int i, int j, RGBTRIPLE image[height][width], int colors[])
{
    //used to multiply pixel colors
    int gx[9] = {-1, 0, 1,
                 -2, 0, 2,
                 -1, 0, 1
                };
    int gy[9] = {-1, -2, -1,
                 0, 0, 0,
                 1, 2, 1
                };
    // index for access to gx and gy
    int index = 0;
    int blue_gx[9], blue_gy[9];
    int green_gx[9], green_gy[9];
    int red_gx[9], red_gy[9];
    for (int x = i - 1; x < i + 2; x++)
    {
        for (int y = j - 1; y < j + 2; y++)
        {
            //assume black pixel off edge of image, negative i or j values = 0
            //multiply pixel color by coresponding gx and gy number
            if (x >= 0 && y >= 0 && x < height && y < width)
            {
                blue_gx[index] = image[x][y].rgbtBlue * gx[index];
                blue_gy[index] = image[x][y].rgbtBlue * gy[index];
                green_gx[index] = image[x][y].rgbtGreen * gx[index];
                green_gy[index] = image[x][y].rgbtGreen * gy[index];
                red_gx[index] = image[x][y].rgbtRed * gx[index];
                red_gy[index] = image[x][y].rgbtRed * gy[index];
                index++;
            }
            else
            {
                blue_gx[index] = 0;
                blue_gy[index] = 0;
                green_gx[index] = 0;
                green_gy[index] = 0;
                red_gx[index] = 0;
                red_gy[index] = 0;
                index++;
            }
        }
    }

    //sum up rgb arrays
    int blue_gxT = 0, blue_gyT = 0;
    int green_gxT = 0, green_gyT = 0;
    int red_gxT = 0, red_gyT = 0;
    for (int k = 0; k < 9; k++)
    {
        blue_gxT += blue_gx[k];
        blue_gyT += blue_gy[k];
        green_gxT += green_gx[k];
        green_gyT += green_gy[k];
        red_gxT += red_gx[k];
        red_gyT += red_gy[k];
    }
    //get color values
    int blue = round(sqrt(pow((float) blue_gxT, 2) + pow((float) blue_gyT, 2)));
    int green = round(sqrt(pow((float) green_gxT, 2) + pow((float) green_gyT, 2)));
    int red = round(sqrt(pow((float) red_gxT, 2) + pow((float) red_gyT, 2)));
    // add rgb colors to colors array
    colors[0] = blue < 255 ? blue : 255;
    colors[1] = green < 255 ? green : 255;
    colors[2] = red < 255 ? red : 255;

    return colors;
}