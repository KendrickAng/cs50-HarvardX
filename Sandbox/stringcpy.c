#include <stdio.h>

void stringcpy(char *s, char*t);

int main(void) {
    
    char s1[] = "how foolish";
    char s2[] = "you fool!";
    
    stringcpy(s1,s2);
    
    printf("%s\n", s1);
    printf("%s\n", s2);
}

void stringcpy(char *s, char*t) {
    
    int i = 0;
    
    while((s[i] = t[i]) != '\0')
        i++;
}