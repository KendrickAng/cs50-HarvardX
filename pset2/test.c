#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <cs50.h>
#include <stdio.h>

int main(void) {
    
    string s;
    
    s = get_string();
    string c = crypt(s, "50");
    
    printf("%s", c);
}
