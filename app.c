#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "channel.h"
#include "user.h"
#include "bot.h"
#include "t_socket.h"
#include "config.h"
#include "utils.h"

/**
 * @brief Handle signals
 *
 * @param signum
 */
void signal_handler(int signum) {
    switch (signum) {
        default:
            printf("\n");
            destroy_channels();
            destroy_configuration();
            exit(0);
            break;
    }
}

/**
 * @brief Start bot thread
 *
 * @return void*
 */
void* start_bot_thread() {
    read_configuration();
    printf("Loaded configuration\n");

    printf("Connecting as (%s)\n", get_config_value("username"));
    connect_to_twitch();
}

/**
 * @brief Main method
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv) {
    signal(SIGINT, signal_handler);
    read_configuration();

    pthread_t bot_thread;
    printf("Creating bot thread\n");

    if(pthread_create(&bot_thread, 0, start_bot_thread, 0) != 0) {
        printf("Could not start bot thread");
        return 0;
    }

    pthread_join(bot_thread, 0);
    return 0;
}
