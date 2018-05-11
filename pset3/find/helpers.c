/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    int lowerLimit = 0;
    int upperLimit = n - 1;
    int count = 0;
    int midPoint = 0;
    
    while(n > 0 && lowerLimit <= upperLimit) {
        
        count++;
        
        //find the midpoint
        midPoint = lowerLimit + (upperLimit - lowerLimit)/2;
        
        //check if the answer is found
        if(values[midPoint] == value) {
            
            return true;
            break;
            
        } else if(values[midPoint] < value) {
            
            //if number being found is larger
                
                //lowerLimit is to the right
                lowerLimit = midPoint + 1;
                
                
            } else {
                //if number being found is smaller
                
                    //upperLimit is to the left
                    upperLimit = midPoint - 1;
                }
            }
    
    //return false if n is negative or value is not found
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    int temp = 0;
    bool isSwapped = false;
    
    //iterate through all numbers in values[]
    for(int i = 0; i < n - 1; i++) {
        
        isSwapped = false;
        
        //iterate through all unsorted numbers
        for(int j = 0; j < n - 1 - i; j++) {
            
            /*Check if the next number is less than than the current number.
            If true, swap positions of both numbers. */
            if(values[j] > values[j + 1]) {
                
                temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
                
                isSwapped = true;
                
            }
            
        }
        //if no number is swapped, the array is already sorted. Break loop
        if(!isSwapped) {
            break;
        }
   
    }
    
    return;
}
