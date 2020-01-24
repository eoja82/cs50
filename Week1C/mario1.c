#include <cs50.h>
#include <stdio.h>

void printSpace(int n);
void printHash(int n);
void printNewLine(void);

//print a hash pyramid with height and width between 1 and 8 based on user input, 1st pyramid angle to right, then two spaces, then pyramid angled left, like end of Mario Bros 
int main(void)
{   
    int height = 0;
    //only accept integers 1-8
    while (height < 1 || height > 8) 
    {
        int answer = get_int("height: \n");
        height = answer;
    }
    for (int i = height - 1; i >= 0; i--) 
    {
        printSpace(i);
        for (int j = height - i; ;)
        {
            printHash(j);
            printSpace(2);
            printHash(j);
            break;
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
void printHash(int n) 
{
    for (int i = 0; i < n; i++) 
    {
        printf("#");
    }
}
void printNewLine(void) 
{
    printf("\n");
}