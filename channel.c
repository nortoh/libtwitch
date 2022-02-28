#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "channel.h"
#include "utils.h"

/** Channel head */
static struct channel_t *head = 0;

/**
 * @brief Print all channels
 *
 */
void list_channels() {
    struct channel_t* curr = head;

    while(curr) {
        printf("[Channel %p] name=%s, next=%p\n", curr, curr->name, curr->next);
        curr = curr->next;
    }
}

/**
 * @brief Does channel exist in channel linked-list
 *
 * @param channel
 * @return int
 */
int has_channel(struct channel_t* channel) {
    struct channel_t* curr = head;

    while(curr) {
        if(strcmp(curr->name, channel->name) == 0) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

/**
 * @brief Create a channel object
 *
 * @param name
 * @return struct channel_t*
 */
struct channel_t* create_channel(char* name) {
    struct channel_t* channel = malloc(sizeof(struct channel_t));
    sprintf(channel->name, "%s", name);
    channel->next = 0;
    return channel;
}

/**
 * @brief Add channel to the top of channel linked-list
 *
 * @param name
 * @return struct channel_t*
 */
struct channel_t* add_channel(char* name) {
    struct channel_t* channel = create_channel(name);
    channel->next = head;
    head = channel;
    return channel;
}

/**
 * @brief Remove channel and relink channel linked-list
 *
 * @param channel
 */
void remove_channel(struct channel_t* channel) {
    if(!strcmp(head->name, channel->name)) {
        struct channel_t* tmp = head;
        head = head->next;
        return;
    }

    struct channel_t* curr = head;
    struct channel_t* prev = head;

    while(curr) {
        if(!strcmp(curr->name, channel->name)) {
            prev->next = curr->next;
            break;
        }

        prev = curr;
        curr = curr->next;
    }
}

/**
 * @brief Get the channel object
 *
 * @param name
 * @return struct channel_t*
 */
struct channel_t* get_channel(char* name) {
    struct channel_t* curr = head;

    while(curr) {
        if(strcmp(curr->name, name) == 0) {
            return curr;
        }
        curr = curr->next;
    }

    return 0;
}

/**
 * @brief Remove channel from channel linked-list and free channel object
 *
 * @param channel
 * @return int
 */
int destroy_channel(struct channel_t* channel) {
    remove_channel(channel);
    free(channel);
    return 1;
}

/**
 * @brief Destroy all channels from memory
 *
 */
void destroy_channels() {
    struct channel_t* curr = head;

    int size = 0;
    while(curr) {
        struct channel_t* tmp = curr;
        curr = curr->next;
        destroy_channel(tmp);
        size++;
    }

    printf("Destroyed %d channels\n", size);
}
