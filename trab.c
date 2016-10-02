#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#define path_to_your_common_file "tmp.txt"

int main(int argc, char **argv){
    DIR* FD;
    struct dirent* in_file;
    FILE    *common_file;
    FILE    *entry_file;
    char   *line;
    char **buffer;
    size_t len = 0;
    ssize_t read;
    int index = 0;
    int realloc_time = 1;
    /* Openiing common file for writing */
    buffer =(char **) malloc(sizeof(char *) * 10);
    common_file = fopen(path_to_your_common_file, "w");
    if (common_file == NULL)
    {
        fprintf(stderr, "Error : Failed to open common_file - %s\n", strerror(errno));

        return 1;
    }

    /* Scanning the in directory */
    if (NULL == (FD = opendir (argv[1]))) 
    {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
        fclose(common_file);

        return 1;
    }
    while ((in_file = readdir(FD))) 
    {
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
        if (entry_file == NULL)
        {
            fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
            fclose(common_file);

            return 1;
        }
        

        /* read file content */
        printf("\nContent of file %s:\n", in_file->d_name);
        while ((read = getline(&line, &len, entry_file)) != -1) {
            if(index == 10 * realloc_time){
                realloc_time ++;
                buffer =(char **) realloc(buffer, 10 * realloc_time);
            }
            buffer[index] = malloc(sizeof(line));
            strcpy(buffer[index],line);
            printf("%s",buffer[index]);
            index ++;
        }

        /* When you finish with the file, close it */
        fclose(entry_file);
        free(path);
    }

    /* Don't forget to close common file before leaving */
    fclose(common_file);

    return 0;
}