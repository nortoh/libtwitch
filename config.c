#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct configuration_t {
    char* key;
    char* value;
    struct configuration_t* next;
};

static struct configuration_t* head = 0;

void print_config_node(struct configuration_t* node) {
    printf("[%p] Key: %s, Value: %s, Next: %p\n", node, node->key, node->value, node->next);
}

int has_key(char* key) {
    struct configuration_t* curr = head;

    while(curr) {
        if(curr->key == key) return 1;
        curr = curr->next;
    }
    return 0;
}

void add_config_node(struct configuration_t* node) {
    if(has_key(node->key)) return;
    
    node->next = head;
    head = node;
}

char* get_config_value(char* key) {
    struct configuration_t* curr = head;

    while(curr) {
        if(!strcmp(key, curr->key)) {
            return curr->value;
        }
        curr = curr->next;
    }
    
    return "DNF";
}

void get_config_loaded() {
    struct configuration_t* curr = head;

    while(curr) {
        print_config_node(curr);
        curr = curr->next;
    }
}

void handle_config_line(char* line) {
    char* token;
    char* result;
    char* key;
    char* value;
    int got_key = 0;

    line[strcspn(line, "\n")] = 0;
    line[strcspn(line, " ")] = 0;

    for(token = strtok_r(line, "=", &result); token != 0; token = strtok_r(0, "=", &result)) {
        if(!got_key) {
            key = token;
            got_key = 1;
        } else {
            value = token;
        }
    }

    struct configuration_t* node = malloc(sizeof(struct configuration_t));
    asprintf(&node->key, "%s", key);
    asprintf(&node->value, "%s", value);
    node->next = 0;

    add_config_node(node);
}

int read_configuration() {
    FILE* fp;
    char* line;
    size_t len = 0;
    ssize_t read;

    fp = fopen("bot.config", "r");
    if(!fp) {
        printf("Failed to open configuration");
        return 0;
    }

    while((read = getline(&line, &len, fp)) != -1) {
        handle_config_line(line);
    }

    fclose(fp);
    if(line) free(line);
}