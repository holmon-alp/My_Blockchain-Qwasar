#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define ALL "*"
#define OR ||
#define AND &&
#define ENDL  putchar('\n')
#define MAX_READ_SIZE 200

#define error_node_exist(n) (printf("node doesn't exist (%d)\n", n))


typedef struct s_bocklist {
    char* data;
    char* hash;
    struct s_bocklist* next;
} blocklist;

typedef struct s_nodelist {
    int id;
    blocklist* blocklist;
    struct s_nodelist* next;
} nodelist;
