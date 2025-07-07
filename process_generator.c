#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void generate_random_byte(FILE *text_file, FILE *binary_file) {
    unsigned char byte = rand() % 256;
    
    if (fprintf(text_file, "%02X ", byte) < 0) {
        fprintf(stderr, "Error writing to text file\n");
        exit(1);
    }
    
    if (fwrite(&byte, sizeof(unsigned char), 1, binary_file) != 1) {
        fprintf(stderr, "Error writing to binary file\n");
        exit(1);
    }
}

void generate_process(FILE *text_file, FILE *binary_file, int process_id, int code_size, int data_size) {
    // Process ID (1 byte)
    unsigned char pid = process_id & 0xFF;
    fprintf(text_file, "%02X ", pid);
    fwrite(&pid, sizeof(unsigned char), 1, binary_file);

    // Code segment size (2 bytes, big-endian)
    fprintf(text_file, "%02X %02X ", (code_size >> 8) & 0xFF, code_size & 0xFF);
    unsigned char code_size_bytes[2] = {(code_size >> 8) & 0xFF, code_size & 0xFF};
    fwrite(code_size_bytes, sizeof(unsigned char), 2, binary_file);

    // Code segment data
    for (int i = 0; i < code_size; i++) {
        generate_random_byte(text_file, binary_file);
    }

    // Data segment size (2 bytes, big-endian)
    fprintf(text_file, "%02X %02X ", (data_size >> 8) & 0xFF, data_size & 0xFF);
    unsigned char data_size_bytes[2] = {(data_size >> 8) & 0xFF, data_size & 0xFF};
    fwrite(data_size_bytes, sizeof(unsigned char), 2, binary_file);

    // Data segment data
    for (int i = 0; i < data_size; i++) {
        generate_random_byte(text_file, binary_file);
    }

    // End marker
    fprintf(text_file, "FF\n");
    unsigned char end_marker = 0xFF;
    fwrite(&end_marker, sizeof(unsigned char), 1, binary_file);
}

int main(int argc, char *argv[]) {
    // Default values
    char base_filename[256] = "p3";
    int process_id = 1;
    int code_size = 32;
    int data_size = 128;
    
    // Parse command line arguments
    if (argc >= 2) strncpy(base_filename, argv[1], sizeof(base_filename)-1);
    if (argc >= 3) process_id = atoi(argv[2]);
    if (argc >= 4) code_size = atoi(argv[3]);
    if (argc >= 5) data_size = atoi(argv[4]);
    
    // Validation
    if (code_size < 0 || code_size > 65535 || data_size < 0 || data_size > 65535) {
        fprintf(stderr, "Error: Sizes must be between 0 and 65535\n");
        return 1;
    }
    
    // Generate filenames
    char text_filename[300], binary_filename[300];
    snprintf(text_filename, sizeof(text_filename), "%s.txt", base_filename);
    snprintf(binary_filename, sizeof(binary_filename), "%s.proc", base_filename);
    
    // Seed random number generator
    srand(time(NULL));

    // Open files
    FILE *text_file = fopen(text_filename, "w");
    FILE *binary_file = fopen(binary_filename, "wb");
    
    if (text_file == NULL) {
        perror("Failed to open text file");
        return 1;
    }
    if (binary_file == NULL) {
        perror("Failed to open binary file");
        fclose(text_file);
        return 1;
    }

    printf("Generating process file: %s\n", binary_filename);
    printf("Process ID: %d, Code Size: %d bytes, Data Size: %d bytes\n", 
           process_id, code_size, data_size);

    // Generate the process
    generate_process(text_file, binary_file, process_id, code_size, data_size);

    fclose(text_file);
    fclose(binary_file);
    
    printf("Files generated successfully:\n");
    printf("  Binary: %s\n", binary_filename);
    printf("  Text:   %s\n", text_filename);

    return 0;
}
