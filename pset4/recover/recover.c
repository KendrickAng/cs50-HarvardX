#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cs50.h>

char fileName[10];

int main(int argc, char *argv[]) 
{
    //ensure proper usage
    if(argc != 2) 
    {
        fprintf(stderr, "Usage: ./recover infile\n.");
        return 1;
    }
    
    //remember filenames
    char *infile = argv[1];
    
    //open memory card file
    FILE *file = fopen(infile, "r");
    if(file == NULL)
    {
        fprintf(stderr,"Could not open %s.\n", infile);
        return 2;
    }
    
    //initialise variables
    int jpegNumber = 0;
    int blockSize = 512;
    bool foundJpeg = false;
    
    FILE* img = NULL;
    
    //give memory to buffer
    //int *buffer = malloc(sizeof(char) * 1000);
    //uint8_t buffer[512];
    unsigned char buffer[blockSize];
    
    
    //Repeat until end of card
    while((fread(buffer, blockSize, 1, file)) == 1) 
    {
        
        //find beginning of JPEG
        if (buffer[0] == 0xff && 
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0) 
            {
                
                if(foundJpeg == true)
                {
                    //new JPEG found, can close current one to start another (later)
                    fclose(img);
                }
                else
                {
                    //first JPEG discovered, begin writing to file
                    foundJpeg = true;
                }
            
                //make a new JPEG and write to it
                sprintf(fileName, "%03i.jpg", jpegNumber);
                img = fopen(fileName, "w");
                
                fwrite(buffer, blockSize, 1, img);
                
                //count the number of jpegs made
                jpegNumber++;
                
                //check for errors
                if(img == NULL)
                {
                    fclose(img);
                    fprintf(stderr, "Could not open file\n.");
                    return 1;
                }
                
                
            } else if (foundJpeg == true)
                {
                    //Jpeg already found, continue to write data into JPEG 
                    fwrite(buffer, blockSize, 1, img);
                }
        
        //not start of JPEG, and no JPEG found yet
        continue;
            
    }
    
    //close file 
    fclose(file);
    fclose(img);
    
    return 0;
    
}