#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <getopt.h>
#include <stdlib.h>

typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;

void edges(int height, int width, RGBTRIPLE image[height][width]);
int * get_edge(int width, int height, int i, int j, RGBTRIPLE image[height][width], int colors[]);

int main(void)
{
    //poulate test image
    RGBTRIPLE image[3][3];
    image[0][0].rgbtBlue = 0;
    image[0][0].rgbtGreen = 10;
    image[0][0].rgbtRed = 25;
    image[0][1].rgbtBlue = 0;
    image[0][1].rgbtGreen = 10;
    image[0][1].rgbtRed = 30;
    image[0][2].rgbtBlue = 40;
    image[0][2].rgbtGreen = 60;
    image[0][2].rgbtRed = 80;
    image[1][0].rgbtBlue = 20;
    image[1][0].rgbtGreen = 30;
    image[1][0].rgbtRed = 90;
    image[1][1].rgbtBlue = 30;
    image[1][1].rgbtGreen = 40;
    image[1][1].rgbtRed = 100;
    image[1][2].rgbtBlue = 80;
    image[1][2].rgbtGreen = 70;
    image[1][2].rgbtRed = 90;
    image[2][0].rgbtBlue = 20;
    image[2][0].rgbtGreen = 20;
    image[2][0].rgbtRed = 40;
    image[2][1].rgbtBlue = 30;
    image[2][1].rgbtGreen = 10;
    image[2][1].rgbtRed = 30;
    image[2][2].rgbtBlue = 50;
    image[2][2].rgbtGreen = 40;
    image[2][2].rgbtRed = 10;
    
    edges(3, 3, image);
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //printf("image blue: %i\n", image[0][0].rgbtBlue);
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
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%i %i %i\n", image[i][j].rgbtBlue, image[i][j].rgbtGreen, image[i][j].rgbtRed);
        }
        
    }
    
}
int * get_edge(int width, int height, int i, int j, RGBTRIPLE image[height][width], int colors[])
{
    int gx[9] = {-1, 0, 1,
                 -2, 0, 2,
                 -1, 0, 1};
    int gy[9] = {-1, -2, -1,
                  0, 0, 0,
                  1, 2, 1,};
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
            //printf("x: %i, y: %i\n", x, y);
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
                //printf("index: %i\n", index);
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
                //printf("index: %i\n", index);
            }
            
            /* blue_gx[index] = x >= 0 && y >= 0 ? image[x][y].rgbtBlue * gx[index] : 0;
            blue_gy[index] = x >= 0 && y >= 0 ? image[x][y].rgbtBlue * gy[index] : 0;
            green_gx[index] = x >= 0 && y >= 0 ? image[x][y].rgbtGreen * gx[index] : 0;
            green_gy[index] = x >= 0 && y >= 0 ? image[x][y].rgbtGreen * gy[index] : 0;
            red_gx[index] = x >= 0 && y >= 0 ? image[x][y].rgbtRed * gx[index] : 0;
            red_gy[index] = x >= 0 && y >= 0 ? image[x][y].rgbtRed * gy[index] : 0;
            index++; */
        }
    }
    /* for (int i = 0; i < 9; i++)
    {
        printf("blue_gx: %i\n", blue_gx[i]);
    } */
    
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
    //printf("bxT: %i, byT: %i, gxT: %i, gyT: %i, rxT: %i, ryT: %i\n", blue_gxT, blue_gyT, green_gxT, green_gyT, red_gxT, red_gyT);
    //get color values
    int blue = round(sqrt(pow((float) blue_gxT, 2) + pow((float) blue_gyT, 2)));
    int green = round(sqrt(pow((float) green_gxT, 2) + pow((float) green_gyT, 2)));
    int red = round(sqrt(pow((float) red_gxT, 2) + pow((float) red_gyT, 2)));
    //printf("blue: %i, green: %i, red: %i\n", blue, green, red);
    // add rgb colors to colors array
    colors[0] = blue < 255 ? blue : 255;
    colors[1] = green < 255 ? green : 255;
    colors[2] = red < 255 ? red : 255;

    return colors;
}