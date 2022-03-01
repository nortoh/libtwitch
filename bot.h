#ifndef __BOT_H__
#define __BOT_H__

/**
 * @brief Bot structure
 *
 */
struct bot_t {
    char* name;
};

/**
 * @brief Create a bot object
 *
 * @param name
 * @return struct bot_t*
 */
extern struct bot_t* create_bot(char* name);
#endif
