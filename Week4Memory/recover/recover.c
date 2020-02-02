//recover jpegs from file
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef uint8_t BYTE;

bool start_jpeg(void);
void write(char *filename);

int counter = -1;
BYTE buffer[sizeof(BYTE) * 512];

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    //open file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Cannot read %s.\n", argv[1]);
        return 1;
    }
    char *filename = malloc(8);
    //get 512 buffer block, at end if 512 not returned
    while (fread(buffer, 1, 512, file) == 512)
    {
        //if at start of a jpeg increment counter update filename
        //new file will be created
        if (start_jpeg())
        {
            counter++;
            sprintf(filename, "%03i.jpg", counter);
            write(filename);
        }
        else
        {
            //if haven't got to start of a jpeg
            if (counter < 0)
            {
                continue;
            }
            else
            {
                write(filename);
            }
        }
    }
    free(filename);
    fclose(file);
}

bool start_jpeg(void)
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && 
        buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    return false;
}
void write(char *filename)
{
    FILE *img = fopen(filename, "a");
    fwrite(buffer, 1, 512, img);
    fclose(img);
}