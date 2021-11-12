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
    add_channel("#nortoh");
    add_user("#nortoh", "nortoh");
    add_user("#nortoh", "cora");
    add_user("#main", "nortoh");
    add_user("#main", "oozebrood");
    add_user("#main", "matthew");
    add_user("#main", "kyle");
    list_users("#main");
    list_users("#nortoh");

    struct user_t* nortoh_user = get_user("#nortoh", "nortoh");
    printf("Got %s\n", nortoh_user->display_name);

    // This works
    connect_to_twitch();

    printf("Continuing after socket creation\n");
    return 0;
}