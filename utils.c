#include <stdio.h>

void info(char* file, char* message) {
    printf("[%s] %s", file, message);
}

void erorr(char* file, char* message) {
    printf("!!! [%s] %s", file, message);
}
