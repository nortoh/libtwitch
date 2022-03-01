#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * @brief Terminal colors
 *
 */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

/**
 * @brief Print info message
 *
 * @param file
 * @param message
 */
extern void info(char* file, char* message);

/**
 * @brief Print error message
 *
 * @param file
 * @param message
 */
extern void error(char* file, char* message);

/**
 * @brief Trim whitespaces from a string
 *
 * @param data
 */
extern void trim_whitespaces(char* data);

/**
 * @brief Print a mark "@@@@"
 *
 * @param n
 */
extern void mark(int n);
#endif
