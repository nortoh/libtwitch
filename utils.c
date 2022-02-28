#include <stdio.h>
#include <string.h>

/**
 * @brief Print info message
 *
 * @param file
 * @param message
 */
void info(char* file, char* message) {
    printf("[%s] %s", file, message);
}

/**
 * @brief Print error message
 *
 * @param file
 * @param message
 */
void error(char* file, char* message) {
    printf("!!! [%s] %s", file, message);
}

/**
 * @brief Trim whitespaces from a string
 *
 * @param data
 */
void trim_whitespaces(char* data) {
    size_t len = strlen(data);

    int i, j;
    for(i = 0; i < len; i++) {
        if(data[i] == ' ') {
            for(j = 0; j < len; j++) {
                data[j] = data[j + 1];
            }
            len--;
        }
    }
}

/**
 * @brief Print a mark "@@@@"
 *
 * @param n
 */
void mark(int n) {
    for(int i = 0; i < n; i++) {
        printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    }
}
