#include <stdlib.h>
#include <stdio.h>

int main(void) {
    
    int *ip = malloc(4);
    int *iq = malloc(4);
    
    *ip = 10;
    *iq = 5;
    
    //before
    printf("%i\n", *ip);
    printf("%i\n", *iq);
    
    *iq = *ip;
    
    //after
    printf("%i\n", *ip);
    printf("%i\n", *iq);
    
    free(ip);
    free(iq);
}