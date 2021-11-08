struct user_t {
    char* name;
    char* display_name;
    struct user_t* next;
};

struct user_t* create_user(char* user, char* display_name);