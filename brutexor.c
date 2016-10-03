#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <openssl/md5.h>

#define MD5 "8d7b356eae43adcd6ad3ee124c3dcf1e"

//http://stackoverflow.com/a/8389763
//gcc brutexor.c -o brutextor -lcrypto -lssl
char *str2md5(const char *str, int length) {
    int n;
    MD5_CTX c;
    unsigned char digest[16];
    char *out = (char*)malloc(33);

    MD5_Init(&c);

    while (length > 0) {
        if (length > 512) {
            MD5_Update(&c, str, 512);
        } else {
            MD5_Update(&c, str, length);
        }
        length -= 512;
        str += 512;
    }

    MD5_Final(digest, &c);

    for (n = 0; n < 16; ++n) {
        snprintf(&(out[n*2]), 16*3, "%02x", (unsigned int)digest[n]);
    }

    return out;
}

char retriveChar(int value){
    char character;
    
    // valores de 0-9
    if(value >= 0 && value < 10){
        character = 48 + value;
    }
    // valores de A-Z
    else if(value >= 10 && value < 37){
        character = 65 + (value - 10);
    }
    // valores de a-z
    else if(value >= 37 && value < 63){
        character = 97 + (value - 37);
    }
        
    return character;
}

int return_flag(int value){
    if(value == 0)
        return 0;
    else
        return 1;
}

char return_char(int value,int * size){
    if(value > 0){
        size ++;
        return retriveChar(value);
    }
    return '\0';
}

int findWord(){
    int size = 0;
    int first, second, third, fourth, fifth, sixth, seventh, eighth;
    int second_flag, third_flag, fourth_flag, fifth_flag, sixth_flag, seventh_flag, eighth_flag;
    char first_char, second_char, third_char, fourth_char, fifth_char, sixth_char, seventh_char, eighth_char;
    int word_index = 0;
    char *words;
    
    for(first = 0; first <= 63; first ++){
        first_char = return_char(first, &size);
        
        for(second = return_flag(first); second <= 63; second ++){
            second_char = return_char(second, &size);
            
            for(third = return_flag(second); third <= 63; third ++){
                third_char = return_char(third, &size);
                
                for(fourth = return_flag(third); fourth <= 63; fourth ++){
                    fourth_char = return_char(fourth, &size);
                    
                    for(fifth = return_flag(fourth); fifth <= 63; fifth ++){
                        fifth_char = return_char(fifth, &size);
                        
                        for(sixth = return_flag(fifth); sixth <= 63; sixth ++){
                            sixth_char = return_char(sixth, &size);
                            
                            for(seventh = return_flag(sixth); seventh <= 63; seventh ++){
                                seventh_char = return_char(seventh, &size);
                                
                                for(eighth = 0; eighth < 63; eighth ++){
                                    eighth_char = retriveChar(eighth);
                                    words = malloc (size + 2);
                                    
                                    int i = 0;
                                    if(first_char != '\0'){
                                        words[i] = first_char;
                                        i++;
                                    }
                                    if(second_char != '\0'){
                                        words[i] = second_char;
                                        i++;
                                    }
                                    if(third_char != '\0'){
                                        words[i] = third_char;
                                        i++;
                                    }
                                    if(fourth_char != '\0'){
                                        words[i] = fourth_char;
                                        i++;
                                    }
                                    if(fifth_char != '\0'){
                                        words[i] = fifth_char;
                                        i++;
                                    }
                                    if(sixth_char != '\0'){
                                        words[i] = sixth_char;
                                        i++;
                                    }
                                    if(seventh_char != '\0'){
                                        words[i] = seventh_char;
                                        i++;
                                    }
                                    words[i] = eighth_char;
                                    i++;
                                    words[i] = '\0';
                                    
                                    printf("String: %s\n", words);
                                    if(strcmp(str2md5(words, strlen(words)),MD5) == 0)
                                        return 1;
                                    size = 0;
                                    free(words);
                                }
                            }
                        }
                    }
                }
            }
        }   
    }
}

int main(int argc, char **argv){
    if(findWord())
        printf("Achei !");
    else
        printf("EPA, nao achei!");
}