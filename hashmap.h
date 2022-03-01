#ifndef __HASHMAP_H__
#define __HASHMAP_H__

/**
 * @brief Bucket structure
 *
 */
struct bucket_t {
    struct element_t* head;
};

/**
 * @brief Element structure
 *
 */
struct element_t {
    struct element_t* next;
};
#endif
