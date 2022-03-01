#ifndef __CONFIG_H__
#define __CONFIG_H__

/**
 * @brief Configuration structure
 *
 */
struct configuration_t {
    char key[50];
    char value[1024];
    struct configuration_t* next;
};

/**
 * @brief Print configuration node
 *
 * @param node
 */
void print_config_node(struct configuration_t* node);

/**
 * @brief Does key exist in configuration memory
 *
 * @param key
 * @return int
 */
int has_key(char* key);

/**
 * @brief Add new configuration node
 *
 * @param node
 */
void add_config_node(struct configuration_t* node);

/**
 * @brief Get the config value object
 *
 * @param key
 * @return char*
 */
char* get_config_value(char* key);

/**
 * @brief Print configuration
 *
 */
extern void get_config_loaded();

/**
 * @brief Handle line from configuration file
 *
 * @param line
 */
void handle_config_line(char* line);

/**
 * @brief Destory configuration from memory
 *
 */
extern void destroy_configuration();

/**
 * @brief Read configuration into memory
 *
 * @return int
 */
extern int read_configuration();
#endif
