#include <stdio.h>
#include <string.h>

void info(char* file, char* message) {
    printf("[%s] %s", file, message);
}

void erorr(char* file, char* message) {
    printf("!!! [%s] %s", file, message);
}

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

void mark(int n) {
    for(int i = 0; i < n; i++) {
        printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    }
}