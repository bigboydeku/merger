/* This program belongs to Denis Nadarevic (104445626, nadarevd@uwindsor.ca). 
Any use of this program by another person for commercial use is subjected to copyright infringement laws. 
This is made for educational purposes.*/
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int headReader(int photo, char mapType[3], char width[10], char height[10]);

int main(int arg_c, char *argv[]) 
{
    //exception handling: if there 4 arguments are not passed, stop the program.
    if(arg_c != 4) return -1;

    //open the images, only read them.
    int imageOne = open(argv[1], O_RDONLY, 0777);
    int imageTwo = open(argv[2], O_RDONLY, 0777);
	
    //Organized the information of each picture by map type and size.
    char mapTypeOne[3], oneWidth[5], oneHeight[5];
    char mapTypeTwo[3], twoWidth[5], twoHeight[5];
	
    //other variables are initialized later.

	//read the headers of each image
    int countOne = headReader(imageOne, mapTypeOne, oneWidth, oneHeight);
    int countTwo = headReader(imageTwo, mapTypeTwo, twoWidth, twoHeight);
	
    //cannot work with char variables if dealing with size, must convert to integer.
    int oneW = atoi(oneWidth);
    int oneH = atoi(oneHeight);
    int twoW = atoi(twoWidth);
    int twoH = atoi(twoHeight);
	
	
    // program will not continue if the first image is smaller than the  second image.
	// program will not continue if the map type of either image does not correspond with P6.
    if (oneW < twoW || oneH < twoH || strcmp(mapTypeOne, "P6") != 0 ||strcmp(mapTypeTwo, "P6") != 0) 
	{
		printf("%s", mapTypeOne);
		printf("%s", mapTypeTwo);
		return -1;
		
	}

    //create a new ppm.
    //uses 4th argument in command line as the name.
    int newImage = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0755);

    //write new information into the new image, such as the header information
    //size will be countOne because image one should be bigger than image two
    char newImageInfo[countOne];
    lseek(imageOne, 0, SEEK_SET);

    read(imageOne, newImageInfo, countOne);
    write(newImage, newImageInfo, countOne);

    //writing the first image into the new image
	//multiply by 3 because theres 3 bits per colour
    for (int i = 0 ; i < oneH ; i++)
    {
        char temp[oneW * 3];
        read(imageOne, temp, oneW * 3);
        write(newImage, temp, oneW * 3);
    }

    lseek(newImage, countOne, SEEK_SET);
    lseek(imageTwo, countTwo, SEEK_SET);

    //writing the second image into the new image
    for (int i = 0 ; i < twoH ; i++)
    {
		lseek(newImage, (oneW * 3) - (twoW * 3), SEEK_CUR);
        
		char temp[oneW * 3];
        read(imageTwo, temp, twoW * 3);
        write(newImage, temp, twoW * 3);
    }

    //close all files
    close(imageOne);
    close(imageTwo);
    close(newImage);

}

/*
headReader reads the ppm image's header to obtain information about the dimensions.
Input:  photo - file id
        maptype[] - type of the image, size = 3 to include \0
        width[] - image width
        height[] - image height

Output: Count - location of beginning of data in integer form.
*/
int headReader(int photo, char mapType[3], char width[5], char height[5])
{
	
    //return variable.
    int count = 7;

    read(photo, mapType, 2);
    //set file pointer to beginning.
    lseek(photo, 1, SEEK_CUR);

    for (int i = 0 ; i < 5 ; i++)
    {
        count++;
        read(photo, width + i, 1);
        if(width[i] == ' ') //breaks loop once ' ' is found
		{
            width[i] = '\0';
            break;
		}
    }

    
    for (int i = 0 ; i < 5 ; i++)
    {
        count++;
        read(photo, height + i, 1);
        if(height[i] == ' ')
		{
            height[i] = '\0';
            break;
		}
    }

    lseek(photo, 4, SEEK_SET);

    //count returns the location of the file where all of the photo's data begins.
    return count;
}
