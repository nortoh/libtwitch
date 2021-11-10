#include <stdio.h>
#include <stdlib.h>
#include "channel.h"
#include "user.h"
#include "bot.h"
#include "t_socket.h"
#include "config.h"
#include "utils.h"

int main(int argc, char **argv) {
    read_configuration();
    printf("Loaded configuration\n");

    printf("Driver class\n");
    printf("Connecting as (%s)\n", get_config_value("username"));

    // Testing
    struct bot_t* bot = create_bot("bot");

    add_channel("#main");

    add_channel("#test");

    struct user_t* nortoh_user = create_user("nortoh", "nortoh");
    add_user("#main", nortoh_user);

    printf("BEFORE\n");    
    list_channels();

    printf("AFTER\n");
    remove_channel("#main");

    list_channels();

    // This works
    // connect_to_twitch();

    printf("Continuing after socket creation\n");
    return 0;
}