#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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

enum bool {true = 1, false = 0};
typedef int  bool;

void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds) ;
}

bool startsWith(char* src, char*sub) {
    for(int i=0; sub[i]!='\0'; i++) {
        if(src[i] != sub[i])
            return false;
    }
    return true;
}
bool isNodeExist(nodelist**nodes, int id) {
    nodelist* n = *nodes;
    while(n != NULL) {
        if(n->id == id)
            return true;
        n = n->next;
    }
    printf("\nid:%d doesn't exist\n", id);
    return false;
}

bool addNode(nodelist** node, int id) {
    nodelist* new = malloc(sizeof(nodelist*));
    new->id = id;
    new->next = NULL;
    nodelist *last = *node;
    while(last->next != NULL) {
        if(isNodeExist(&last, id)) {
            printf("[ %d ] this node already exists\n", id);
            return false;
        }
        last = last->next;
    }
    last->next = new;
    printf("Succesful add node with id : %d\n", id);
    return true;
}

void deleteFirstByKey(nodelist** list, int id) {
    if(!isNodeExist(list, id)) {
        return ;
    }
    nodelist* now = malloc(sizeof(nodelist*));
    now = *list;
    nodelist* next = malloc(sizeof(nodelist*));
    next = now->next;
    
    while(now != NULL) {
        if(now->id == id && next != NULL) {
            *list = next;
            printf("delete node with id: %d\n", id);
            break;
        }
        if (next->id == id && next != NULL) {
            now->next = now->next->next;
            printf("delete node by id: %d\n", id);
            break;
        }
        now = now->next;
        // if(next->next != NULL) {
        //     next = next->next;
        // }
    }
}
int countCode(nodelist**node) {
    int count = 0;
    nodelist*c = *node;
    while(c != NULL) {
        c = c->next;
        ++count;
    }
    return count;
}
void printNodes(nodelist**list) {
    nodelist*n = *list;
    while(n != NULL) {
        printf("%d\t", n->id);
        n = n->next;
    }
}

int blockchain() {
    nodelist *blockchain = malloc(sizeof *blockchain);
    blockchain->id = 1;
    blockchain->blocklist = malloc(sizeof(blocklist*));
    blockchain->next = NULL;
    blockchain->blocklist->data = "Block !!";
    blockchain->blocklist->hash = "bwefjkb2jk3huhbvhi5u4njhgj#$#@WFe";
    blockchain->blocklist->next = NULL;
    // printf("blockchain.\nid: %d\n", blockchain->id);
    // printf("block data: %s\n", blockchain->blocklist->data);
    // printf("block hash: %s\n", blockchain->blocklist->hash);
    addNode(&blockchain, 2);
    addNode(&blockchain, 3);
    addNode(&blockchain, 4);
    
    printNodes(&blockchain);

    deleteFirstByKey(&blockchain, 1);
    deleteFirstByKey(&blockchain, 4);
    deleteFirstByKey(&blockchain, 1);
    deleteFirstByKey(&blockchain, 3);
    deleteFirstByKey(&blockchain, 2);
    printf("size: %d\n", countCode(&blockchain));
    printNodes(&blockchain);
    return 1;
}

int main() {
    
    blockchain();
    return 0;
}
