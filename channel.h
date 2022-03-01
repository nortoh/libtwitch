#ifndef __CHANNEL_H__
#define __CHANNEL_H__

/**
 * @brief Channel structure
 *
 */
struct channel_t {
    char name[30];
    struct channel_t* next;
};

/**
 * @brief Print all channels
 *
 */
extern void list_channels();

/**
 * @brief Does channel exist in channel linked-list
 *
 * @param channel
 * @return int
 */
extern int has_channel(struct channel_t* channel);

/**
 * @brief Create a channel object
 *
 * @param name
 * @return struct channel_t*
 */
struct channel_t* create_channel(char* name);

/**
 * @brief Get the channel object
 *
 * @param name
 * @return struct channel_t*
 */
extern struct channel_t* get_channel(char* name);

/**
 * @brief Add channel to the top of channel linked-list
 *
 * @param name
 * @return struct channel_t*
 */
extern struct channel_t* add_channel(char* name);

/**
 * @brief Remove channel and relink channel linked-list
 *
 * @param channel
 */
extern void remove_channel(struct channel_t* channel);

/**
 * @brief Remove channel from channel linked-list and free channel object
 *
 * @param channel
 * @return int
 */
extern int destroy_channel(struct channel_t* channel);

/**
 * @brief Destroy all channels from memory
 *
 */
extern void destroy_channels();
#endif
