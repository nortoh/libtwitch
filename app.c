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
    
    // add_channel("#nortoh");
    // struct channel_t* nortoh_chan = get_channel("#nortoh");

    // struct user_t* nortoh_user = get_user(nortoh_chan, "nortoh");
    // if(nortoh_user == 0) {
    //     printf("We did not find nortoh in #nortoh\n");
    // }

    // add_user(nortoh_chan, "nortoh");
    // add_user(nortoh_chan, "oozebrood");
    // add_user(nortoh_chan, "cora");

    // nortoh_user = get_user(nortoh_chan, "nortoh");

    // if(nortoh_user != 0) {
    //     printf("We found nortoh in #nortoh\n");
    // } else {
    //     printf("Still not found\n");
    // }
    
    // list_users(nortoh_chan);


    // struct user_t* nortoh_user = get_user("#nortoh", "nortoh");
    // printf("Got %s\n", nortoh_user->display_name);

    // This works
    connect_to_twitch();

    printf("Continuing after socket creation\n");
    return 0;
}