#include <cs50.h>
#include <stdio.h>
#include <string.h>

#include "helpers.c"

int main(void) {
    string input = "1/4";
    int number = duration(input);

    printf("%d",number);
}