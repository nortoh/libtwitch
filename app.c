#include <stdio.h>
#include <stdlib.h>
#include<signal.h>
#include "channel.h"
#include "user.h"
#include "bot.h"
#include "t_socket.h"
#include "config.h"
#include "utils.h"

void signal_handler(int signum) {
    switch (signum)
    {
    default:
        destroy_channels();
        exit(0);
        break;
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, signal_handler);
    read_configuration();
    printf("%sLoaded configuration%s\n", GRN, RESET);

    printf("Driver class\n");
    printf("Connecting as (%s)\n", get_config_value("username"));

    // Testing
    struct bot_t* bot = create_bot("bot");

    // This works
    connect_to_twitch();

    printf("Continuing after socket creation\n");
    return 0;
}