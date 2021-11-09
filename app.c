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
    printf("Connecting as (%s)", get_config_value("username"));

    // Testing
    struct bot_t* bot = create_bot("bot");

    struct channel_t *test = create_channel("test");
    add_channel(test);

    struct channel_t *main_channel = create_channel("main");
    add_channel(main_channel);

    struct user_t* nortoh_user = create_user("nortoh", "nortoh");
    add_user(main_channel, nortoh_user);
    
    list_channels();

    read_configuration();
    get_config_loaded();
    printf("%s\n", get_config_value("username"));

    // This works
    connect_to_twitch();

    printf("Continuing after socket creation\n");
    return 0;
}