#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

// Function to create a directory
void create_directory(const char *dirname) {
    if (mkdir(dirname, 0777) == -1) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    }
    printf("Directory '%s' created successfully.\n", dirname);
}

// Function to list directory contents
void list_directory(const char *dirname) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    printf("Contents of directory '%s':\n", dirname);
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

// Function to delete a file
void delete_file(const char *filename) {
    if (unlink(filename) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }
    printf("File '%s' deleted successfully.\n", filename);
}

int main() {
    const char *dirname = "test_dir";
    const char *filename = "test_file.txt";

    // Create a directory
    create_directory(dirname);

    // List directory contents
    list_directory(dirname);

    // Create a file
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "This is a test file.\n");
    fclose(file);
    printf("File '%s' created successfully.\n", filename);

    // List directory contents again
    list_directory(dirname);

    // Delete the file
    delete_file(filename);

    // List directory contents after deletion
    list_directory(dirname);

    // Delete the directory
    if (rmdir(dirname) == -1) {
        perror("rmdir");
        exit(EXIT_FAILURE);
    }
    printf("Directory '%s' deleted successfully.\n", dirname);

    return 0;
}

