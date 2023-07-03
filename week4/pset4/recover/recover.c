#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //TODO
    //open memorycard/file/print errors
    //chck arg for file
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE");
        return 1;
    }

    //repeat until end of card
    //read 512 bytes into buffer
    BYTE buffer[512];
    //initialize variables
    int image_count = 0;
    int bytes_read = 0;
    char filename[8];

    //create file
    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file;
    //check file validity
    if (input_file == NULL)
    {
        printf("Could not open file");
        return 1;
    }

    while (1)
    {
        bytes_read = fread(buffer, sizeof(BYTE), 512, input_file);
        //if already found jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (image_count == 0)
            {
                //write file names
                sprintf(filename, "%03i.jpg", image_count);
                //open file and write it
                output_file = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), bytes_read, output_file);
                //add to count
                image_count++;
            }
            else
            {
                //close the previous image and start a new one
                fclose(output_file);
                sprintf(filename, "%03i.jpg", image_count);
                output_file = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), bytes_read, output_file);
                image_count++;
            }
        }
        else if (image_count != 0)
        {
            //keep writing last file
            fwrite(buffer, sizeof(BYTE), bytes_read, output_file);
            if (bytes_read == 0)
            {
                //close last file
                fclose(output_file);
                break;
            }
        }
    }
    //close remaining files
    fclose(input_file);

    return 0;
}