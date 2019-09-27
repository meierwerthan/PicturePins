//Meier Werthan
#include <stdio.h>
#include "lodepng.h"
#include <string.h>
#include <math.h>

int numToRepeatMax(int* arr1 , int n, int k)
{
    int mx = arr1[0], result = 0;
    for (int i = 0; i< n; i++)
        arr1[arr1[i]%k] += k;

    for (int i = 1; i < n; i++)
    {
        if (arr1[i] > mx)
        {
            mx = arr1[i];
            result = i;
        }
    }
    return result;
}

int main() {
    FILE * fp;
    unsigned int error;
    unsigned char *rawImageBytes;
    unsigned int width, height;
    char filename[100] = "C:\\Users\\MeierWerthan\\ClionProjects\\PicturePins\\Cookie.PNG";
    error = lodepng_decode32_file(&rawImageBytes, &width, &height, filename);
    if (error) {
        printf("error loading: %s\n", lodepng_error_text(error));
        return 1;
    }
    int arr[width][height]; //make 2d array
    int x = 0; //make variable for x on array
    int y = 0; //make variable for y on the array
    int z = 0; //make variable for rgb array
    int b = 0; //make comparison var for rgbDistance
    int d = 0; //make var to find smallest distance
    int red; //make variable to hold red pixel value
    int blue;//make variable to hold blue pixel value
    int green;//make variable to hold green pixel value
    int i = 0; //make variable to hold count
    int redD[12] = {0, 51, 102, 0, 160, 255, 255, 255, 153, 255, 255, 255};
    int greenD[12] = {0, 153, 51, 255, 160, 0, 128, 102, 0, 0, 255, 255};
    int blueD[12] = {0, 255, 0, 0, 160, 255, 0, 255, 153, 0, 255, 0};
    int rgbDistance[12] = {};
    //(0)Black, (1)Blue, (2)Brown, (3)Green, (4)Grey, (5)Magenta, (6)Orange, (7)Pink, (8)Voilet, (9)Red, (10)White, (11)Yellow
    for (y = 0; y <= height - 1; y++) {  //make loop to fill y values
        for (x = 0; x <= width - 1; x++) { //make a loop to fill x values
            red = rawImageBytes[i]; //find red value
            i++;
            green = rawImageBytes[i]; //find green value
            i++;
            blue = rawImageBytes[i];  //find blue value
            for (z = 0; z < 12; z++) {
                rgbDistance[z] = abs(red - redD[z]) + abs(green - greenD[z]) + abs(blue - blueD[z]);
                //printf(".%d, %d, %d,(%d),,(%d, %d, %d).", red, blue, green, rgbDistance[z], redD[z], blueD[z], greenD[z]);
                if(z == 0){
                    d = rgbDistance[z];
                    b = z;
                }else if (rgbDistance[z] < d){
                    b = z;
                    d = rgbDistance[z];
                }
            }
            //printf("%d,", b);
            arr[x][y] = b; //store value in new 2d array
            i = i + 2; //skip the alpha
        }
    }
    int X = 0; //make a new x for smaller 2d array
    int x_scale = 4;
    int y_scale = 5;
    int s[x_scale*y_scale]; //make variable to find new most frequent
    int n = sizeof(s)/ sizeof(s[0]);
    int Y = 0; //make a new y for smaller 2d array
    int arr2[width / x_scale][height / y_scale]; //condense array by a factor of 5
    for (Y = 0; Y < height / y_scale; Y++) { //make a loop to fill y values
        for (X = 0; X < width / x_scale; X++) { //make a loop to fill x values
            for (int j = 0; j <= 4; j++) { //make a loop to keep track of large array y values
                for (i = 0; i <= 4; i++) { //make a loop to keep track of large array x values
                    s[i] = arr[X * x_scale + i][Y * y_scale + j]; //condense array
                }
            }
            arr2[X][Y] = numToRepeatMax(s, n, 12); //condensed array find most frequent and store in smaller array
            memset(s, 0, sizeof(s)); //clear array
        }
    }
    char grayLevels[12] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', '#'}; //initialize gray level values for the stored RGB values //
    //(0)Black, (1)Blue, (2)Brown, (3)Green, (4)Grey, (5)Magenta, (6)Orange, (7)Pink, (8)Voilet, (9)Red, (10)White, (11)Yellow
    fp = fopen ("C:\\Users\\MeierWerthan\\ClionProjects\\PicturePins\\2.txt","w");
    for (Y = 0; Y <= (height / y_scale) - 1; Y++) { //make a loop to print smaller array y values
        for (X = 0; X <= (width / x_scale) - 1; X++) { //make a loop to print smaller array x values
            //arr2[X][Y] = (arr2[X][Y] / 20); //divide the raw RGB values by 30 to get 0-8
            arr2[X][Y] = grayLevels[arr2[X][Y]]; //store gray level values to corresponding smaller array
            printf("%c", arr2[X][Y]); //print the array
            fprintf(fp, " %c", arr2[X][Y]);
        }
        printf("\n"); //enter lines
        fprintf(fp, "\n");
    }
    fclose (fp);
    return 0;
}