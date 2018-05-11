#include <stdio.h>

void swap(int *px, int *py);

int main(void) {
    
    //int *x = malloc(5);
    //int *y = malloc(10);
    
    int x = 5;
    int y = 10;
    
    printf("%i\n", x);
    printf("%i\n", y);
    
    swap(&x,&y);
    
    printf("%i\n", x);
    printf("%i\n", y);
}

void swap(int *px, int *py) {
    
    int temp = *px;
    *px = *py;
    *py = temp;
    
}
