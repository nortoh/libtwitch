#ifndef __CONFIG_H__
#define __CONFIG_H__
struct configuration_t {
    char key[50];
    char value[1024];
    struct configuration_t* next;
};

int has_key(char* key);
void add_config_node(struct configuration_t* node);
char* get_config_value(char* key);
extern void get_config_loaded();
void handle_config_line(char* line);
extern void destroy_configuration();
extern int read_configuration();
#endif