#ifndef __BADGE_H__
#define __BADGE_H__

/**
 * @brief Badge structure
 *
 */
struct badge_t {
    char key[50];
    char value[50];
    struct badge_t* next;
};

/**
 * @brief Print all bridges from a given badge linked-list
 *
 * @param badge
 */
extern void print_badges(struct badge_t* badge);

/**
 * @brief Free all badges from a given badge linked-list
 *
 * @param tag
 */
extern void free_badges(struct badge_t* badge);

/**
 * @brief Dissamble badge and create badge linked-list
 *
 * @param raw
 * @return struct badge_t*
 */
extern struct badge_t* disassemble_badge(char* raw);

/**
 * @brief Create a badge pair object
 *
 * @param key
 * @param value
 * @return struct badge_t*
 */
struct badge_t* create_badge_pair(char* key, char* value);
#endif
