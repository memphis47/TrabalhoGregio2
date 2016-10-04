#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>

#define path_to_your_common_file "tmp.txt"

int isNewString(char *line){
	char *line_output;	
	size_t len_output = 0;
    ssize_t read_output;
    FILE *output_file_read = fopen("dicionario.lst", "r");
	if (output_file_read == NULL)
	{
		fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));

	}
	if(strcmp(line,"Não, não, não venha pra cá, que eu não quero mais saber de você\n")==0)
			printf("Linha no read: %s\n",line);
	while ((read_output = getline(&line_output, &len_output, output_file_read)) != -1){
		if(strcmp(line,"Não, não, não venha pra cá, que eu não quero mais saber de você\n")==0)
			printf("Linha: %s\nLinha lida do arquivo: %s\n",line,line_output);
		if(strcmp(line,line_output)==0){
			fclose(output_file_read);			
			return 0; 
		}
	}
    fclose(output_file_read);
	return 1;
}

int main(int argc, char **argv){
    DIR* FD;
	struct timespec ts;
    struct dirent* in_file;
    FILE *common_file;
    FILE *entry_file;
    char *line;
    size_t len = 0;
    ssize_t read;
    int index = 0;
    int realloc_time = 1;

	ts.tv_sec = 0;
    ts.tv_nsec = 50000000;
    /* Openiing common file for writing */
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

	FILE *output_file_write = fopen("dicionario.lst", "w+");
    if (output_file_write == NULL)
    {
        fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));

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

            return 1;
        }
        

        /* read file content */
        while ((read = getline(&line, &len, entry_file)) != -1) {
			int hasline = 0;
			if(strcmp(line,"Não, não, não venha pra cá, que eu não quero mais saber de você\n") == 0)
				printf("Linha: %s\n",line);
			if(strcmp(line,"\n") != 0 && isNewString(line)){
	            line[strcspn(line, "\n")] = 0;
				fprintf(output_file_write,"%s\n",line);
				int fd = fileno(output_file_write);   
			    int ret = fsync(fd);
				sleep(1);
			}
            
        }

        /* When you finish with the file, close it */
        fclose(entry_file);
        free(path);
    }
    fclose(output_file_write);
    /* Don't forget to close common file before leaving */
    fclose(common_file);

    return 0;
}
