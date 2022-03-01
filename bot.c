#include <stdlib.h>
#include "bot.h"
#include "channel.h"

/**
 * @brief Create a bot object
 *
 * @param name
 * @return struct bot_t*
 */
struct bot_t* create_bot(char* name) {
    struct bot_t* bot = malloc(sizeof(struct bot_t));
    bot->name = name;
    return bot;
}
