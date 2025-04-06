#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

int MAX_NAME_SIZE = 256;

double run_java_program(const char* dir, const char* input_file, const char* output_file) {
    char command[1024];
    struct timeval start, end;
    double elapsed;

    // Change to target directory
    snprintf(command, sizeof(command), "cd \"%s\"", dir);
    if (system(command) != 0) {
        fprintf(stderr, "Failed to change to directory: %s\n", dir);
        return -1.0;
    }

    // Compile Java program
    snprintf(command, sizeof(command), "cd \"%s\" && javac Main.java", dir);
    if (system(command) != 0) {
        fprintf(stderr, "Failed to compile Main.java in %s\n", dir);
        return -1.0;
    }

    // Get start time
    gettimeofday(&start, NULL);

    // Run Java program with input redirection
    snprintf(command, sizeof(command), 
             "cd \"%s\" && java Main < \"../%s\" > \"%s\"", 
             dir, input_file, output_file);
    if (system(command) != 0) {
        fprintf(stderr, "Failed to run Main.java\n");
        return -1.0;
    }

    // Get end time
    gettimeofday(&end, NULL);

    // Calculate elapsed time in seconds
    elapsed = (end.tv_sec - start.tv_sec) + 
              (end.tv_usec - start.tv_usec) / 1000000.0;

    return elapsed;
}

void unzip(char* dir_name) {
    

    printf("Making directory: %s\n", dir_name);
    char mk_dir_command[MAX_NAME_SIZE];
    snprintf(mk_dir_command, sizeof(mk_dir_command), "mkdir -p ./%s", dir_name);
    system(mk_dir_command);

    printf("Unzipping: %s.zip\n", dir_name);
    char command[MAX_NAME_SIZE];
    snprintf(command, sizeof(command), "unzip -qo %s.zip -d ./%s/", dir_name, dir_name);
    printf("Command: %s\n", command);
    system(command);
}

void delete(char* dir_name) {

    printf("Deleting directory: %s\n", dir_name);
    char rm_dir_command[MAX_NAME_SIZE];
    snprintf(rm_dir_command, sizeof(rm_dir_command), "rm -r %s", dir_name);
    system(rm_dir_command);
}

double execute_test(char* dir_name) {
    

    char cd_command[MAX_NAME_SIZE];
    snprintf(cd_command, sizeof(cd_command), " cd ./%s && javac Main.java", dir_name);
    system(cd_command);
    printf("CD command:%s", cd_command);

    /*
    char exec_command[MAX_NAME_SIZE];
    snprintf(exec_command, sizeof(exec_command), "javac Main.java");
    system(exec_command);
    */ 

    // Simulate a return value for testing purposes
    return 0.0;
}

int main() {

    printf("--------------- JAVA PROJECT TESTER ---------------\n");

    // This directory (dp)
    struct dirent *entry;
    DIR *dp = opendir(".");
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

        execution_times[i] = run_java_program(dir_name, "test1.txt", "test1_o.txt");
        delete(dir_name);
    }

    printf("Execution times for each folder:\n");
    for(int i = 0; i < num_of_folders; i++) {
        printf("Execution time for %s: %.2f seconds\n", zip_files_names[i], execution_times[i]);
    }


    closedir(dp);


    return 0;
}




