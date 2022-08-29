#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define OR ||
#define AND &&
#define MAX_READ_SIZE 200

enum bool {true = 1, false = 0};
typedef int  bool;
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


char* input() {
    char* res = malloc(MAX_READ_SIZE), c;
    int i = 0;
    while(read(STDIN_FILENO, &c, 1) > -1) {
        if(strlen(res) == MAX_READ_SIZE-1 OR c=='\n')
            break;
        res[i++] = c;
    }
    return res;
}

int putch(char c) {
    return write(1,&c, 1);
}
int putstr(char* s) {
    while(*s) {
        putch(*s++);
    }
    return putch('\n');
}

void delay(int number_of_seconds) {
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
int countCode(nodelist**node) {
    int count = 0;
    nodelist*c = *node;
    while(c != NULL) {
        c = c->next;
        ++count;
    }
    return (count>0) ? count-1 : count;
}


bool isNodeExist(nodelist** nodes, int id) {
    nodelist* n = *nodes;
    while (n != NULL) {
        if(n->id == id)
            return true;
        n = n->next;
    }
    return false;
}
bool isBlockExistInNode(nodelist** node, blocklist** block) {
    nodelist*n = *node;
    blocklist *b = n->blocklist;
    blocklist* in = *block;
    while (b != NULL) {
        if(b->data == in->data)
            return true;
        b = b->next;
    }
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
    // printf("Succesful add node with id : %d\n", id);
    return true;
}

void deleteAllNodes(nodelist** head) {
   nodelist* current = *head;
   nodelist* next;
   while (current != NULL) {
       next = current->next;
       free(current);
       current = next;
   }
   *head = NULL;
}
void deleteNodeByKey(nodelist *head, int key) {
    nodelist *prev, *cur;
    while (head != NULL && head->id == key) {
        prev = head;
        head = head->next;
        free(prev);
        return;
    }

    prev = NULL;
    cur  = head;
    while (cur != NULL) {
        if (cur->id == key) {
            if (prev != NULL) 
                prev->next = cur->next;
            free(cur);
            return;
        } 

        prev = cur;
        cur = cur->next;
    }
}
void removeBlockfromBlocklist(blocklist *block, char* key) {
    blocklist *prev, *cur;
    if (block != NULL && block->data == key) {
        prev = block;
        block = block->next;
        free(prev);
        return;
    }
    prev = NULL;
    cur  = block;
    while (cur != NULL) {
        if (cur->data == key) {
            if (prev != NULL) 
                prev->next = cur->next;
            free(cur);
            return;
        } 
        prev = cur;
        cur = cur->next;
    }
}

void removeBlock(nodelist **head, char* data) {
    nodelist *node = *head;
    blocklist* test = malloc(sizeof(blocklist));
    test->data = data;
    bool is_block_exist = false;
    while(node != NULL) {
        blocklist* now = node->blocklist;
        if(isBlockExistInNode(&node, &test)){
            removeBlockfromBlocklist(now, data);
            is_block_exist = true;
        }
        node = node->next;
    }
    if(!is_block_exist) putstr("block doesn't exist");
}

int pushBlock(nodelist** nodes, blocklist** block) {
    nodelist* n = *nodes;
    blocklist *last = n->blocklist;
    if(last == NULL) {
        n->blocklist = *block;
        printf("Add block to first (%s)\n", n->blocklist->data);
        return 0;
    } else {
        blocklist* b =  malloc(sizeof(blocklist*));
        b = last;
        while(b->next != NULL) {
            b = b->next;
        }
        b->next = *block;
        printf("new added (%s) -- previus block (%s)\n\n", b->next->data, b->data);
        b->next->next = NULL;
        n->blocklist = last;
    }
    return 0;
}
void addBlockById(nodelist** nodes, char* data, int node_id) {
    blocklist* new = malloc(sizeof(blocklist));
    new->data = malloc(strlen(data)+1);
    new->data = data;
    nodelist *node = *nodes;
    while (node != NULL) {
        if (node->id == node_id) {
            if(isBlockExistInNode(&node, &new)) {
                printf("Block already exist\n");
                return ;
            }
            // printf("find node (%d)\n", node->id);
            pushBlock(&node, &new);
            // node->blocklist = new;
            // node->blocklist->next = NULL;
            break;
        }
        node = node->next;
    }
}

void printBlocksById(nodelist* list, int index) {
    nodelist* n = list;
    while (n != NULL) {
        if(n->id == index) {
            // blocklist* block = n->blocklist;
            printf("print node id: %d\n", n->id);
            while(n->blocklist != NULL) {
                printf("block ->%s\n", n->blocklist->data);
                printf("hash: ->%s\n", n->blocklist->hash);
                n->blocklist = n->blocklist->next;
            }
        }
        n = n->next;
    }
}

// int removeBlock(nodelist** nodes, )

void printNodes(nodelist**list) {
    nodelist*n = *list;
    while(n != NULL) {
        printf("%d\t", n->id);
        n = n->next;
    }
    putchar('\n');
}

int blockchain() {
    nodelist *blockchain = malloc(sizeof *blockchain);
    blockchain->id = 0;
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
    addNode(&blockchain, 1);

    addBlockById(&blockchain, "block 1", 3);
    addBlockById(&blockchain, "block 2", 3);
    addBlockById(&blockchain, "block 3", 3);
    addBlockById(&blockchain, "block 4", 3);
    addBlockById(&blockchain, "block 5", 3);
    addBlockById(&blockchain, "block 6", 3);
    // printBlocksById(blockchain, 0);
    removeBlock(&blockchain, "block 3");
    // removeBlock(&blockchain, "block 32www");
    printBlocksById(blockchain, 3);
    printNodes(&blockchain);

    // printf("size: %d\n", countCode(&blockchain));
    // deleteAllNodes(&blockchain);
    // printf("size: %d\n", countCode(&blockchain));
    // printNodes(&blockchain);
    return 1;
}

int main() {
    
    // blockchain();
    char* arr = input();
        printf("%s\n", arr);
    return 0;
}
