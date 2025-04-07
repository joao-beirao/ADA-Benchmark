#include "tester.h"

/*
 * unzip - Unzips a given zip file into a specified directory.
 * 
 * @param dir_name The name of the directory to unzip the contents into.
 * 
 * This function creates a directory with the specified name and unzips the
 * contents of a zip file with the same name into that directory. It uses the
 * system command to create the directory and unzip the file.
 * 
 * Ex.:
 * -> unzip("example_dir");
 * This will create a directory named "example_dir" and unzip the contents of
 * "example_dir.zip" into it.
*/
void unzip(char* dir_name) {
    
    printf("Making directory: %s\n", dir_name);
    char mk_dir_command[MAX_NAME_SIZE];
    snprintf(mk_dir_command, sizeof(mk_dir_command), "mkdir -p %s%s", VERSIONS_DIR, dir_name);
    system(mk_dir_command);

    printf("Unzipping: %s.zip\n", dir_name);
    char command[MAX_NAME_SIZE];
    snprintf(command, sizeof(command), "unzip -qo %s%s.zip -d %s%s/",VERSIONS_DIR, dir_name,VERSIONS_DIR, dir_name);
    printf("Command: %s\n", command);
    system(command);
}