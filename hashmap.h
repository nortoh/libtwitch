#ifndef __HASHMAP_H__
#define __HASHMAP_H__
struct bucket_t {
    struct element_t* head;
};

struct element_t {
    struct element_t* next;
};
#endif