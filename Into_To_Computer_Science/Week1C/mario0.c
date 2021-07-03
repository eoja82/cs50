#include <cs50.h>
#include <stdio.h>

void printSpace(int n);
void printHash(void);
void printNewLine(void);

//print a hash pyramid like the end of Mario Bros with height and width between 1 and 8 based on user input    
int main(void)
{   
    int height = 0;
    while (height < 1 || height > 8) 
    {
        int answer = get_int("height: \n");
        height = answer;
    }
    for (int i = 1; i <= height; i++) 
    {
        printSpace(height - i);
        for (int j = height - i; j < height; j++)
        { 
            printHash();
        }
        printNewLine();
    }
}

void printSpace(int n) 
{
    for (int i = 0; i < n; i++) 
    {
        printf(" ");
    }
}
void printHash(void) 
{
    printf("#");
}
void printNewLine(void) 
{
    printf("\n");
}