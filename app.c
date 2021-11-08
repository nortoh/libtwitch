#include <stdio.h>
#include <stdlib.h>
#include "channel.h"

int main(int argc, char **argv) {

    printf("Driver class\n");
    
    struct channel_t *test = malloc(sizeof(struct channel_t));
    test->name = "Test";
    test->next = 0;

    add_channel(test);

    return 0;
}