#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

char FindInitials(string input);

int main(void) {
    
    string name = get_string();
    
    if(name != NULL) {
    
        FindInitials(name);
        
        }
    
}

char FindInitials(string input) {
    
    //ASCII for space
    int space = 32;
    
    int i = 0;
    int charInString;
    
    //print the first letter
    printf("%c" , toupper(input[0]));
    
    //iterate over each char after the first letter 
    for( int n = strlen(input); i < n; i++) {
        
        //update ASCII number of current char
        charInString = input[i];
        
        //If ASCII of current char = space, print the letter after each spacing
        if(charInString == space) {
            
            printf("%c" , toupper(input[i + 1]));
            
        }
    }
    
    printf("\n");
    
    return 0;
}