#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#define DB "./BLOCKCHAIN"
#define ALL     "*"
#define OR      ||
#define AND     &&
#define ENDL    putchar('\n')
#define EXIT    1001
#define MAX_READ_SIZE 4050
#define OK                       printf("OK\n")
#define error_node_exist         printf("NOK: node doesn't exist\n")
#define error_node_alrdy_exist   printf("NOK: this node already exists\n")
#define error_block_alrdy_exist  printf("NOK: this block already exists\n")
#define error_block_exist        printf("NOK: block doesn't exist\n")
#define error_commd              printf("NOK: command not found\n")
#define error_resourse           printf("NOK: no more resources available on the computer\n")
typedef struct s_bocklist {
    char* data;
    size_t size;
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
char* my_strtok(char*,const char*);
string_array* my_split(char*, char*);
void putch(char );
void putstr(char*);
bool like(char*, char*);
void trim(char* , char*);

void itoa(int , char[]);
void reverse(char[]);
char* my_readline(int );

bool is_sync(nodelist**);
int count_nodes(nodelist**);
void read_linked_list(nodelist**);
int blockchain(nodelist**, string_array*);
