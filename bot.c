#include <stdlib.h>
#include "bot.h"
#include "channel.h"

static char* name = 0;

struct bot_t* create_bot(char* name) {
    struct bot_t* bot = malloc(sizeof(struct bot_t));
    bot->name = name;
    return bot;
}

