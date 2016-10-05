#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#define SEPARATORS " ,;.-\":%?!()"

int isStringInBuffer(char *str, char **buffer, int size){
    int i;
    for(i = 0; i < size; i++){
        if(strcmp(str, buffer[i]) == 0)
            return 0;
    }
    return 1;
}

void writeInLibrary(char **buffer, int size){
    int i;

    FILE *output_file = fopen("dicionario.txt", "w+");
    if (output_file == NULL) {
        fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
    }

    /* read file content */
    for(i = 0; i < size; i++){
        if (i+1 == size)
            fprintf(output_file,"%s", buffer[i]);
        else
            fprintf(output_file, "%s\n", buffer[i]);
    }

    /* When you finish with the file, close it */
    fclose(output_file);
}

int main(int argc, char **argv){
    DIR* FD;
    struct dirent* in_file;
    FILE *common_file;
    FILE *entry_file;
    char *line;
    size_t len = 0;
    ssize_t read;
    int index = 0;
    char **buffer = NULL;
    int buffer_size = 0;
    char *word;

    /* Scanning the in directory */
    if (NULL == (FD = opendir (argv[1]))){
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
        return 1;
    }

    while ((in_file = readdir(FD))){
        char *path = malloc(sizeof(argv[1]) + sizeof(in_file->d_name) + 1);
        /* On linux/Unix we don't want current and parent directories
         * On windows machine too, thanks Greg Hewgill
         */
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))
            continue;
        /* Open directory entry file for common operation */

        //get the path of the file
        strcpy(path,argv[1]);
        strcat(path,in_file->d_name);

        entry_file = fopen(path, "rw");
        if (entry_file == NULL){
            fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
            return 1;
        }

        /* read file content */
        while ((read = getline(&line, &len, entry_file)) != -1){
            // if needed, allocate space for BUFFER_CHUNK_SIZE more strings
            if (index >= buffer_size) {
                buffer_size += 64 * 1024;
                buffer = (char**) realloc(buffer, buffer_size * sizeof(char*));
            }

            // strip the line feed character
            line[strcspn(line, "\n")] = 0;

            for (word = strtok(line, SEPARATORS); word != NULL; word = strtok(NULL, SEPARATORS)) {
                // if the string is in the bufer
                if (isStringInBuffer(word, buffer, index) != 0) {
                    buffer[index] = (char*) malloc((strlen(word) + 1) * sizeof(char));
                    strcpy(buffer[index], word);
                    index++;
                }
            }
        }

        /* When you finish with the file, close it */
        fclose(entry_file);
        free(path);
    }

    writeInLibrary(buffer,index);

    return 0;
}
