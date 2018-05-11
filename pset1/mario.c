#include <stdio.h>
#include <cs50.h>

void PrintRow(int height);

int main(void) {
    
    int x;
    
    do {
        printf("Height: ");
        x = get_int();
    } 
    while (x < 0 || x >23);
    
    PrintRow(x);
    
}

void PrintRow(int height) {
    
        int increasing = height;
        int decreasing = height;
        
    
        for( int i = 0; i < height; i++) {
        
        increasing += 1;
        decreasing -= 1;
        
        //print the spaces (decreasing iteration)
       for ( int z = 1; z <= decreasing; z++) {
           
           printf(" ");
    
        }
        
         //print the hashes (increasing iteration)
        for ( int y = increasing ; y >= height; y--) {
       
         printf("#");
        
       }
      
      //print a new line
        printf("\n");
        
        
    }
   
    
}

