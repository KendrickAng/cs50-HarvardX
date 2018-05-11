#include <stdio.h>
#include <cs50.h>
#include <math.h>

int GetCoins(int money);


int main(void) {
    
    float change;
    
    do {
        printf("O hai! How much change is owed?\n");
        change = get_float();
    } 
        while (change < 0);
        
    int roundedChange = round(change * 100);
    
    GetCoins(roundedChange);
}

int GetCoins(int money) {
    
    int n = 0;
    
    while (money >= 25) {
        n++;
        money -= 25;
    }
    
    while (money >= 10) {
        n++;
        money -= 10;
    }
    
    while (money >= 5) {
        n++;
        money -= 5;
    }
    
     while (money >= 1) {
        n++;
        money -= 1;
    }
    
    printf("%i\n" , n);
    return 0;
}