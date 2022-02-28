#ifndef __UTILS_H__
#define __UTILS_H__

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

extern void info(char* file, char* message);
extern void error(char* file, char* message);
extern void trim_whitespaces(char* data);
extern void mark(int n);
#endif
