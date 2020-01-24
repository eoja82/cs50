#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// use Coleman-Liau formula to test grade level of input text

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: \n");
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    float l = ((float) letters / (float) words) * 100;
    float s = ((float) sentences / (float) words) * 100;
    float index = (0.0588 * l) - (0.296 * s) - 15.8;
    int grade = (index - floor(index) > 0.5 ? ceil(index) : floor(index));
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string text)
{
    int letters = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if ((text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122))
        {
            letters++;
        }
        else
        {
            continue;
        }
    }
    return letters;
}

int count_words(string text)
{
    int words = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == 32 || text[i] == 0)
        {
            words++;
        }
        //count last word at end of string
        else if (i == n - 1)
        {
            words++;
        }
        else
        {
            continue;
        }
    }
    return words;
}
int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == 46 || text[i] == 33 || text[i] == 63)
        {
            sentences++;
        }
        else
        {
            continue;
        }
    }
    return sentences;
}