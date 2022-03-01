#ifndef __TAG_H__
#define __TAG_H__

/**
 * @brief Tag header structure
 *
 */
struct tag_header_t {
    // char id[40];
    struct tag_t* tags;
    struct badge_t* badges;
};

/**
 * @brief Tag structure
 *
 */
struct tag_t {
    char key[128];
    char value[128];
    struct badge_t* badges;
    struct tag_t* next;
};

/**
 * @brief Create a tag object
 *
 * @param raw
 * @return struct tag_header_t*
 */
extern struct tag_header_t* create_tag(char* raw);

/**
 * @brief Print tag linked-list from header
 *
 * @param header
 */
void print_tag_header(struct tag_header_t* header);

/**
 * @brief Create a tag pair object
 *
 * @param key
 * @param value
 * @return struct tag_t*
 */
struct tag_t* create_tag_pair(char* key, char* value);

/**
 * @brief Free tag from header
 *
 * @param header
 */
extern void free_tag_header(struct tag_header_t* header);

/**
 * @brief Dissamble tag and attach to tag header
 *
 * @param header
 * @param raw
 */
extern void disassemble_tag(struct tag_header_t* header, char* raw);

/**
 * @brief Destory tag header from memory
 *
 * @param header
 */
extern void destroy_tag_header(struct tag_header_t* header);
#endif
