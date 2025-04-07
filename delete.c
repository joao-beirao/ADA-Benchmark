#include "tester.h"

/*
 * delete - Deletes a given directory.
 * 
 * @param dir_name The name of the directory to delete.
 * 
 * This function constructs a command to remove the specified directory and
 * executes it using the system command.
 * 
 * Ex.:
 * -> delete("example_dir");
 * This will delete the directory "./versions/example_dir".
 * 
*/
void delete(char* dir_name) {

    printf("Deleting directory: %s\n", dir_name);
    char rm_dir_command[MAX_NAME_SIZE];
    snprintf(rm_dir_command, sizeof(rm_dir_command), "rm -r %s%s",VERSIONS_DIR, dir_name);
    system(rm_dir_command);
}