#include "tester.h"


double run_java_program(const char* dir, const char* input_file, const char* output_file) {
    char command[1024];
    struct timeval start, end;
    double elapsed;
    printf("Running Java program with dir_name: %s, input_file: %s, output_file: %s\n", dir, input_file, output_file);
    return -1.00;

    // Change to target directory
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
             "cd \"%s%s\" && java Main < \"../..%s%s\" > \"../..%s%s\"", 
             VERSIONS_DIR, dir,INPUT_DIR, input_file,OUTPUT_DIR, output_file);
    if (system(command) != 0) {
        fprintf(stderr, "Failed to run Main.java\n");
        return -1.0;
    }

    // Get end time
    gettimeofday(&end, NULL);

    // Compare results
    snprintf(command, sizeof(command), "diff -q \"%s%s%s\" \"%s%s\"", OUTPUT_DIR, dir, output_file, INPUT_DIR, input_file);
    if (system(command) != 0) {
        fprintf(stderr, "Output does not match expected results for %s%s\n", dir, output_file);
        return -1.0;
    }


    // Calculate elapsed time in seconds
    elapsed = (end.tv_sec - start.tv_sec) + 
              (end.tv_usec - start.tv_usec) / 1000000.0;

    return elapsed;
}