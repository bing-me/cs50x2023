#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    //loop throught each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //convert to float
            float R = image[i][j].rgbtRed;
            float G = image[i][j].rgbtGreen;
            float B = image[i][j].rgbtBlue;
            //average rgb, float, rint
            int average = rint((R + G + B) / 3);
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
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
            float R = image[i][j].rgbtRed;
            float G = image[i][j].rgbtGreen;
            float B = image[i][j].rgbtBlue;
            //apply sepia formula
            int sepiaRed = rint(.393 * R + .769 * G + .189 * B);
            int sepiaGreen = rint(.349 * R + .686 * G + .168 * B);
            int sepiaBlue = rint(.272 * R + .534 * G + .131 * B);
            //limit to 255
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
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
}
// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //swap horizontally
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            //set image to temp, using formula, set left to right, then right to left
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
    //average pixel with surrounding 8 pixels + itself
    //for edges and corners, remove invalid pixels
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    float valid = 0;
    float totalRed = 0;
    float totalGreen = 0;
    float totalBlue = 0;
    //count valid pixels/ignore invalid pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int x = i - 1; x <= i + 1; x++)
            {
                for (int y = j - 1; y <= j + 1; y++)
                {
                    if (y < width && y >= 0 && x < height && x >= 0)
                    {
                        //calculates total of surrounding value
                        totalRed += temp[x][y].rgbtRed;
                        totalGreen += temp[x][y].rgbtGreen;
                        totalBlue += temp[x][y].rgbtBlue;
                        //count valid occurances for the average
                        valid++;
                    }
                }
            }
            //calculate blur values, averaging from totals earlier
            image[i][j].rgbtRed = round(totalRed / valid);
            image[i][j].rgbtGreen = round(totalGreen / valid);
            image[i][j].rgbtBlue = round(totalBlue / valid);
            valid = 0;
            totalRed = 0;
            totalGreen = 0;
            totalBlue = 0;
        }
    }
    return;
}