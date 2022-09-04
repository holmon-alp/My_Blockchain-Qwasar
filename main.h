#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define DB "./BLOCKCHAIN"
#define ALL     "*"
#define OR      ||
#define AND     &&
#define ENDL    putchar('\n')
#define EXIT    1001
#define MAX_READ_SIZE 4050
#define error_node_exist(n)         (printf("node doesn't exist (%d)\n", n))
#define error_node_alrdy_exist(n)   (printf("this node already exists (%d)\n", n))
#define error_block_alrdy_exist(n)  (printf("this block already exists (%s)\n", n))
#define error_block_exist(n)        (printf("block doesn't exist (%s)\n", n))
#define error_commd                 printf("command not found\n""Use: add node nid..\tadd block bid nid..\n""\trm node nid..\trm block bid..\n\tls (-l) or sync or quit\n")
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
int like(char*, char*);
void trim(char* , char*);

void itoa(int , char[]);
void reverse(char[]);

char* my_readline(int );
