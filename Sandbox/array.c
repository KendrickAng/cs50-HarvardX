#include <stdio.h>

int main(void) {
    
    int a[10];
    int *pa;
    int x = 0;
    
    for(int i = 0; i < 10; i++) {
        a[i] = i;
        
        printf("a[%i] is: %i\n", i , i);
    }
    
    pa = &a[5];
    x = *pa;
    
    printf("x is: %i", x);
}