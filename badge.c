#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "badge.h"

/**
 * @brief Print all bridges from a given badge linked-list
 *
 * @param badge
 */
void print_badges(struct badge_t* badge) {
    struct badge_t* curr = badge;

    while(curr) {
        printf("[ BADGE %p:%p] %s = %s\n", (void *) curr, (void *) curr->next, curr->key, curr->value);
        curr = curr->next;
    }
}

/**
 * @brief Free all badges from a given badge linked-list
 *
 * @param tag
 */
void free_badges(struct badge_t* tag) {
    struct badge_t* curr = tag;
    while(curr) {
        struct badge_t* tmp = curr;
        curr = curr->next;
        free(tmp);
        printf("Freeing badge %p\n", (void *) tmp);
    }
}

/**
 * @brief Dissamble badge and create badge linked-list
 *
 * @param raw
 * @return struct badge_t*
 */
struct badge_t* disassemble_badge(char* raw) {
    char badge_str[1024];
    strcpy(badge_str, raw);

    char* badge_split;
    char* badge_result;

    char* token_pair;
    char* token_result;

    char key[50];
    char value[50];

    struct badge_t* head = 0;
    struct badge_t* curr = 0;

    for(badge_split = strtok_r(badge_str, ",", &badge_result); badge_split != 0; badge_split = strtok_r(0, ",", &badge_result)) {
        int pair_count = 0;

        for(token_pair = strtok_r(badge_split, "/", &token_result); token_pair != 0; token_pair = strtok_r(0, "/", &token_result)) {
            if(pair_count == 0) {
                strcpy(key, token_pair);
            } else {
                strcpy(value, token_pair);
            }
            pair_count++;
        }

        struct badge_t* next = create_badge_pair(key, value);
        if(!head) {
            head = next;
            curr = next;
        } else {
            curr->next = next;
            curr = curr->next;
        }
    }

    return head;
}

/**
 * @brief Create a badge pair object
 *
 * @param key
 * @param value
 * @return struct badge_t*
 */
struct badge_t* create_badge_pair(char* key, char* value) {
    struct badge_t* badge = malloc(sizeof(struct badge_t));
    strcpy(badge->key, key);
    strcpy(badge->value, value);
    badge->next = 0;
    return badge;
}
