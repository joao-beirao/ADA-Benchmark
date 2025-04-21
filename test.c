#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

// Constants
#define MAX_NAME_SIZE 256
#define VERSIONS_DIR "./versions/"
#define INPUT_DIR "/input/"
#define OUTPUT_DIR "/output/"
#define EXPECTED_OUTPUT_DIR "/expected_outputs/"


double run_java_program(const char* dir, const char* input_file, const char* output_file);
void delete(char* dir_name);
void unzip(char* dir_name);


int main() {

    int TEST_NORMALIZATION_ITERATIONS = 10;

    printf("--------------- JAVA PROJECT TESTER ---------------\n");

    // XXXXXXXXXXX   GET THE LIST OF ZIP FILES TO UNZIP AND RUN THE PROGRAM INTO   XXXXXXXXXXX
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



    // XXXXXXXXXXX   GET THE LIST OF TEST FILES TO RUN ON THE PROGRAM   XXXXXXXXXXX
    struct dirent *entry2;
    char input_dir_name[MAX_NAME_SIZE];
    snprintf(input_dir_name, sizeof(input_dir_name), ".%s", INPUT_DIR);
    DIR *dp2 = opendir(input_dir_name);
    if (dp2 == NULL) {
        perror("opendir");
        return 1;
    }

    char test_files_names[100][MAX_NAME_SIZE]; // Array to store up to 100 zip file names, each up to 255 characters

    int num_of_tests = 0;
    for(int i = 0; (entry2 = readdir(dp2)) != NULL; i++) {
        if (strstr(entry2->d_name, ".txt") != NULL) {
            snprintf(test_files_names[num_of_tests], sizeof(test_files_names[num_of_tests]), "%s", entry2->d_name);
            num_of_tests++;
        }
    }




    double execution_times[num_of_folders][num_of_tests];
    for (int i = 0; i < num_of_folders; i++) {
        // Remove the ".zip" extension to create a directory name
        char dir_name[MAX_NAME_SIZE];
        snprintf(dir_name, sizeof(dir_name), "%.*s", (int)(strlen(zip_files_names[i]) - 4), zip_files_names[i]);

        printf("Found zip file: %s\n", zip_files_names[i]);
        unzip(dir_name);


        // Testing
        for(int j = 0; j < num_of_tests; j++) {
 
            printf("Found test file: %s\n", test_files_names[j]);

            char input_file[MAX_NAME_SIZE];
            snprintf(input_file, sizeof(input_file), "%s", test_files_names[j]);
        
            char output_file[MAX_NAME_SIZE+2];
            snprintf(output_file, sizeof(output_file), "o_%s",  test_files_names[j]);
        

            // --- Normalize the execution time --- (To make hardware fluctuations less impactful)
            double execution_time = 0.0;
            for (int k = 0; k < TEST_NORMALIZATION_ITERATIONS; k++)
            {
                printf("%d:%d\n", k, TEST_NORMALIZATION_ITERATIONS);
                execution_time = run_java_program(dir_name, input_file, output_file);
            }
            execution_time = execution_time / TEST_NORMALIZATION_ITERATIONS;
            // ---

            execution_times[i][j] = execution_time;


        }
        delete(dir_name);

        
    }

    for(int i = 0; i < num_of_tests; i++) {
        printf("Execution times for Test %s:\n", test_files_names[i]);
        for (int j = 0; j < num_of_folders; j++)
        {
            if(execution_times[j][i] != -1 ){
                printf("%s: %.3f seconds\n", zip_files_names[j], execution_times[j][i]);

            }else{
                printf("%s: Not Working\n");

            }

        }
        
    }

    closedir(dp);


    return 0;
}


void unzip(char* dir_name) {
    
    printf("Making directory: %s\n", dir_name);
    char mk_dir_command[MAX_NAME_SIZE];
    snprintf(mk_dir_command, sizeof(mk_dir_command), "mkdir -p %s%s", VERSIONS_DIR, dir_name);
    system(mk_dir_command);
    printf("OK\n");

    printf("Unzipping: %s.zip\n", dir_name);
    char command[MAX_NAME_SIZE];
    snprintf(command, sizeof(command), "unzip -qo %s%s.zip -d %s%s/",VERSIONS_DIR, dir_name,VERSIONS_DIR, dir_name);
    system(command);
    printf("OK\n");
}

void delete(char* dir_name) {

    printf("Deleting directory: %s\n", dir_name);
    char rm_dir_command[MAX_NAME_SIZE];
    snprintf(rm_dir_command, sizeof(rm_dir_command), "rm -r %s%s",VERSIONS_DIR, dir_name);
    system(rm_dir_command);
    printf("OK\n");
}

/** 
 * DIR -> ZipFileName
 * INPUT -> test1.txt
 * OUTPUT -> test1_o.txt
 */
double run_java_program(const char* dir, const char* input_file, const char* output_file) {

    char command[1024];
    struct timeval start, end;
    double elapsed;
    //printf("Running Java program with dir_name: %s, input_file: %s, output_file: %s\n", dir, input_file, output_file);

    // Check Change to target directory
    snprintf(command, sizeof(command), "cd \"%s%s\"",VERSIONS_DIR, dir);
    if (system(command) != 0) {
        fprintf(stderr, "Failed to change to directory: %s%s\n",VERSIONS_DIR, dir);
        return -1.0;
    }

    // Compile Java program
    snprintf(command, sizeof(command), "cd \"%s%s\" && javac Main.java",VERSIONS_DIR, dir);
    if (system(command) != 0) {
        fprintf(stderr, "Failed to compile Main.java in %s%s\n",VERSIONS_DIR, dir);
        return -1.0;
    }

    // Get start time
    gettimeofday(&start, NULL);

    // Run Java program with input redirection
    snprintf(command, sizeof(command), 
             "cd \"%s%s\" && java Main < \"../..%s%s\" > \"../..%s%s_%s\"", 
             VERSIONS_DIR, dir,INPUT_DIR, input_file,OUTPUT_DIR, dir, output_file);
    if (system(command) != 0) {
        fprintf(stderr, "Failed to run Main.java\n");
        return -1.0;
    }

    // Get end time
    gettimeofday(&end, NULL);

    // Compare results
    snprintf(command, sizeof(command), "diff -q \".%s%s\" \".%s%s_%s\"", EXPECTED_OUTPUT_DIR, output_file, OUTPUT_DIR, dir, output_file);
    if (system(command) != 0) {
        fprintf(stderr, "Output does not match expected results for %s\n", output_file);
        return -1.0;
    }


    // Calculate elapsed time in seconds
    elapsed = (end.tv_sec - start.tv_sec) + 
              (end.tv_usec - start.tv_usec) / 1000000.0;
    //printf("Elapsed time: %.6f seconds\n", elapsed);

    return elapsed;
}



