#include <stdio.h>
#include <stdlib.h>
#include "channel.h"
#include "user.h"
#include "bot.h"
#include "t_socket.h"

int main(int argc, char **argv) {

    printf("Driver class\n");
    
    struct bot_t* bot = create_bot("bot");

    struct channel_t *test = create_channel("test");
    add_channel(test);

    struct channel_t *main_channel = create_channel("main");
    add_channel(main_channel);

    struct user_t* nortoh_user = create_user("nortoh", "nortoh");
    add_user(main_channel, nortoh_user);
    
    list_channels();

    twitch_connect("localhost", 8000);

    return 0;
}