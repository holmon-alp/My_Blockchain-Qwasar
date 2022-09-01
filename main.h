#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define ALL "*"
#define OR  ||
#define AND &&
#define ENDL  putchar('\n')
#define MAX_READ_SIZE 200

#define error_node_exist(n)         (printf("node doesn't exist (%d)\n", n))
#define error_node_alrdy_exist(n)   (printf("this node already exists (%d)\n", n))
#define error_block_alrdy_exist(n)  (printf("this block already exists (%s)\n", n))
#define error_block_exist(n)        (printf("block doesn't exist (%s)\n", n))
#define error_commd                 printf("command not found\n")


typedef struct s_bocklist {
    char* data;
    char* time;
    struct s_bocklist* next;
} blocklist;

typedef struct s_nodelist {
    int id;
    blocklist* blocklist;
    struct s_nodelist* next;
} nodelist;

#ifndef STRUCT_STRING_ARRAY
#define STRUCT_STRING_ARRAY
typedef struct s_string_array {
    int size;
    char** array;
} string_array;
#endif
typedef struct s_you_do {
    int add_node;
    int rm_node;
    int rm_all_nodes;
    int rm_block;
    int add_block;
    int add_block_all;
    string_array* array;
} you_do;
