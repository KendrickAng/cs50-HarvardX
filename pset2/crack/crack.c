#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <cs50.h>
#include <stdio.h>

void CrackPassword(string input);
int Check(int stringLength, string genString, string inputString);


int main(int argc, string argv[]) {

    if(argc != 2) {
        
        printf("Error: Only accepting one command-line argument!");
        return 1;
        
    } else {
        
        CrackPassword(argv[1]);
        
    }
}

void CrackPassword(string input) {
    
    int numOfChars = 51;
     
    char chars[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
               'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
   
    char genString[6];
    
    //generate 1-character plaintexts
    int stringLength = 1;
    
        for (int i = 0; i < numOfChars; i++) {
        
            genString[0] = chars[i];
            
            //Check the crypted word
            Check(stringLength, genString, input);
            
            
        }
    
    //generate 2-character plaintexts
    stringLength = 2;
    
        for(int i = 0; i < numOfChars; i++) {
            
            genString[0] = chars[i];
            
                for(int o = 0; o < numOfChars; o++) {
                
                    genString[1] = chars[o];
                    
                    //Check the crypted word
                    Check(stringLength, genString, input);
                    
                }
           
        }
       
    //generate 3-character plaintexts
    stringLength = 3;
    
        for(int i = 0; i < numOfChars; i++) {
            
            genString[0] = chars[i];
            
                for(int o = 0; o < numOfChars; o++) {
                
                    genString[1] = chars[o];
                    
                        for(int p = 0; p < numOfChars;p++) {
                            
                            genString[2] = chars[p];
                            
                            //Check the crypted word
                            Check(stringLength, genString, input);
                            
                        }
                }
           
        }
    
    //generate 4-character plaintexts
    stringLength = 4;
    
        for(int i = 0; i < numOfChars; i++) {
            
            genString[0] = chars[i];
            
                for(int o = 0; o < numOfChars; o++) {
                
                    genString[1] = chars[o];
                    
                        for(int p = 0; p < numOfChars; p++) {
                            
                            genString[2] = chars[p];
                            
                                for(int l = 0; l < numOfChars; l++) {
                                    
                                    genString[3] = chars[l];
                                    
                                    //Check the crypted word
                                    Check(stringLength, genString, input);
                                    
                                }
                            
                        }
                        
                }
           
        }
        
    //If no matches are found
    printf("The answer you're looking for either has more than 4 characters, doesn't consist ONLY of alphabets, or doesn't exit!");
    exit(1);
}

int Check(int stringLength, string genString, string inputString) {
    
    if(stringLength == strlen(genString)) {
        
        //hash plaintext to be checked against input
        string encryptedStr = crypt(genString,"50");
        
        //check for a match
        if(strcmp(encryptedStr, inputString) == 0) {
            
            printf("%s\n", genString);
            
            exit(0);
        } 
        
    }
    return 1;
}