#include <stdio.h>
#include <cs50.h>

int MinToBot(int n);

int main() {
    
    printf("Minutes: ");
    int minutes = get_int();
    
    printf("Bottles: %i\n", MinToBot(minutes));
}

int MinToBot(int n) {
    
    return n * 12;
    
}