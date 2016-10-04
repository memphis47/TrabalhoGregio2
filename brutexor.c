#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <regex.h>
#include <openssl/md5.h>

#define MD5 "8d7b356eae43adcd6ad3ee124c3dcf1e"
#define CADEIA "0b3430202f27052205292128222619342322272d"
#define REG_HEX "^[a-f]$|^[A-F]$"

//http://stackoverflow.com/a/8389763
//gcc brutexor.c -o brutextor -lcrypto -lssl
void str2md5(const char *str, char *out, int length) {
    int n;
    MD5_CTX c;
    unsigned char digest[16];

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
}

char retriveChar(int value){
    char character;
    
    // valores de 0-9
    if(value >= 0 && value < 10){
        character = 48 + value;
    }
    // valores de A-Z
    else if(value >= 10 && value < 36){
        character = 65 + (value - 10);
    }
    // valores de a-z
    else if(value >= 36 && value < 62){
        character = 97 + (value - 36);
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
        return retriveChar(value - 1);
    }
    return '\0';
}

void append_char_function(char append_char, char *word, int *i){
    if(append_char != '\0'){
        word[(*i)] = append_char;
        (*i)++;
    }
}

char * findWord(){
    int size = 0;
    int first, second, third, fourth, fifth, sixth, seventh, eighth;
    int second_flag, third_flag, fourth_flag, fifth_flag, sixth_flag, seventh_flag, eighth_flag;
    char first_char, second_char, third_char, fourth_char, fifth_char, sixth_char, seventh_char, eighth_char;
    int word_index = 0;
    char *word;
	char buffer[33];
    
	for(first = 0; first <= 62; first ++){
        first_char = return_char(first, &size);
        
        for(second = return_flag(first); second <= 62; second ++){
            second_char = return_char(second, &size);
            
            for(third = return_flag(second); third <= 62; third ++){
                third_char = return_char(third, &size);
                
                for(fourth = return_flag(third); fourth <= 62; fourth ++){
                    fourth_char = return_char(fourth, &size);
                    
                    for(fifth = return_flag(fourth); fifth <= 62; fifth ++){
                        fifth_char = return_char(fifth, &size);
                        
                        for(sixth = return_flag(fifth); sixth <= 62; sixth ++){
                            sixth_char = return_char(sixth , &size);
                            
                            for(seventh = return_flag(sixth); seventh <= 62; seventh ++){
                                seventh_char = return_char(seventh, &size);
                                
                                for(eighth = 0; eighth < 62; eighth ++){
                                    eighth_char = retriveChar(eighth);
                                    word = malloc (size + 2);
                                    
                                    int i = 0;
                                    
                                    append_char_function(first_char, word, &i);
                                    append_char_function(second_char, word, &i);
                                    append_char_function(third_char, word, &i);
                                    append_char_function(fourth_char, word, &i);
                                    append_char_function(fifth_char, word, &i);
                                    append_char_function(sixth_char, word, &i);
                                    append_char_function(seventh_char, word, &i);
                                    word[i] = eighth_char;
                                    i++;
                                    word[i] = '\0';
 									str2md5(word,buffer, strlen(word));
                                    if(strcmp(buffer, MD5) == 0)
                                        return word;
                                    size = 0;
                                    free(word);
                                }
                            }
                        }
                    }
                }
            }
        }   
    }
	return NULL;
}

/**
*   Função que faz a validação da entrada strValidate de acordo com o regex definido no pattern.
**/
int regexValidation(char *strValidate, char * pattern){
    regex_t reg;

    /* compila a ER passada em argv[1]
     * em caso de erro, a função retorna diferente de zero */
    if (regcomp(&reg , pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        fprintf(stderr,"erro regcomp\n");
        exit(1);
    }
    /* tenta casar a ER compilada (&reg) com a entrada (argv[2])
     * se a função regexec retornar 0 casou, caso contrário não */
    if ((regexec(&reg, strValidate, 0, (regmatch_t *)NULL, 0)) == 0)
        return 1;
    else
        return 0;
}


int hex_to_int(char c){
	char str[2];
	str[0] = c;
	str[1] = '\0';
	if(regexValidation(str,REG_HEX)){
		if(c == 'a' || c == 'A')
			return 10;
		else if(c == 'b' || c == 'B')
			return 11;
		else if(c == 'c' || c == 'C')
			return 12;
		else if(c == 'd' || c == 'D')
			return 13;
		else if(c == 'e' || c == 'E')
			return 14;
		else if(c == 'f' || c == 'F')
			return 15;
		else
			return 0;
	}
	else
		 return (int) strtol(str, NULL, 10);
}

//http://stackoverflow.com/a/5403170
int hex_to_ascii(char c, char d){
        int high = hex_to_int(c) * 16;
        int low = hex_to_int(d);
        return high+low;
}

void returnASCIIString(char *string){	
	int index = 0;
	int i;
	char buf = 0;
	for(i = 0; i < 41; i++){
        if(i % 2 != 0){
                string[index] = hex_to_ascii(buf, CADEIA[i]);
				index++;
        }else{
                buf = CADEIA[i];
        }
	}		
}

char * makeXOR(char *key){
	char *string = malloc(20);
	returnASCIIString(string);
	static char xor[42];
	int i; 
	int j = 0;
	for(i = 0; i < 20; i++){
		xor[i] = (char)(string[i] ^ key[j]);
		j++;		
		if(j == strlen(key))
			j = 0;			
	}
	return xor;
}

int main(int argc, char **argv){
	char *result = findWord();    
	if(result != NULL)
        printf("A Dica é:\n %s\n", makeXOR(result));
    else
        printf("\nEPA, nao achei!\n");
}
