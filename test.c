#include "tester.h"


int main() {

    printf("--------------- JAVA PROJECT TESTER ---------------\n");

    // This directory (dp)
    struct dirent *entry;
    DIR *dp = opendir(VERSIONS_DIR);
    if (dp == NULL) {
        perror("opendir");
        return 1;
    }

    char zip_files_names[100][MAX_NAME_SIZE]; // Array to store up to 100 zip file names, each up to 255 characters

    int num_of_folders = 0;
    for(int i = 0; (entry = readdir(dp)) != NULL; i++) {
        if (strstr(entry->d_name, ".zip") != NULL) {
            snprintf(zip_files_names[num_of_folders], sizeof(zip_files_names[num_of_folders]), "%s", entry->d_name);
            num_of_folders++;
        }
    }

    double execution_times[num_of_folders];
    for (int i = 0; i < num_of_folders; i++) {
        // Remove the ".zip" extension to create a directory name
        char dir_name[MAX_NAME_SIZE];
        snprintf(dir_name, sizeof(dir_name), "%.*s", (int)(strlen(zip_files_names[i]) - 4), zip_files_names[i]);

        printf("Found zip file: %s\n", zip_files_names[i]);
        unzip(dir_name);

        char input_file[MAX_NAME_SIZE];
        snprintf(input_file, sizeof(input_file), "%s", "test1.txt");
        char output_file[MAX_NAME_SIZE];
        snprintf(output_file, sizeof(output_file), "%s%s",dir_name, "test1_o.txt");

        execution_times[i] = run_java_program(dir_name, input_file, output_file);
        delete(dir_name);
    }

    printf("Execution times for each folder:\n");
    for(int i = 0; i < num_of_folders; i++) {
        printf("Execution time for %s: %.6f seconds\n", zip_files_names[i], execution_times[i]);
    }


    closedir(dp);


    return 0;
}




