#include "main.h"

enum bool {true = 1, false = 0};
typedef int  bool;

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

void putch(char c) { write(1,&c, 1);}
void putstr(char* s) { while(*s) { putch(*s++);}  putch('\n'); }

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
int countNodes(nodelist**node) {
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

bool isNodeEmpty(nodelist** nodes) {
    nodelist* node = *nodes;
    if(node->blocklist == NULL)
        return true;
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
    if(!is_block_exist) error_block_exist(data);
}

int pushBlock(nodelist** nodes, blocklist** block) {
    nodelist* n = *nodes;
    blocklist *last = n->blocklist;
    if(last == NULL) {
        n->blocklist = *block;
        return 0;
    } else {
        blocklist* b =  malloc(sizeof(blocklist*));
        b = last;
        while(b->next != NULL) {
            b = b->next;
        }
        b->next = *block;
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
    if(!isNodeExist(&node, node_id)) {
        error_node_exist(node_id);
        return ;
    }
    while (node != NULL) {
        if (node->id == node_id) {
            if(isBlockExistInNode(&node, &new)) {
                printf("Block already exist\n");
                return ;
            }
            pushBlock(&node, &new);
            break;
        }
        node = node->next;
    }
}

void printBlocksById(nodelist* list, int index) {
    nodelist* n = list;
    while (n != NULL) {
        if(n->id == index) {
            blocklist* b = n->blocklist;
            while(b != NULL) {
                if(b->data)
                    printf("%s, ", b->data);
                b = b->next;
            }
            ENDL;
            break;
        }
        n = n->next;
    }
}

void printNodes(nodelist**list, bool l) {
    nodelist*n = *list;
    while(n != NULL) {
        if(n->id != 0) {
            if(l AND !isNodeEmpty(&n)) {
               printf("node - %d: ", n->id);
               printBlocksById(n, n->id);
            } 
            else
                printf("node - %d\n", n->id);
        } 
        n = n->next;
    }
}

void save_quit(nodelist** blockchain, const char* file) {
    nodelist* for_save = *blockchain;
    int fd = open(file, O_CREAT | O_RDWR | O_APPEND);
    if(fd < 0) {
        printf("File opening error\n");
        exit(EXIT_FAILURE);
    }
    write(fd, for_save, sizeof(for_save));
    close(fd);
}
// check block is exist
bool isBlockInBlocklist(blocklist** block, char* data) {
    blocklist* t = *block;
    if(t == NULL) return false;
    while ( t ) {
        if(t->data == data) 
            return true;
        t = t->next;
    }
    return false;
}

void append(blocklist**bl, char* data) {
    blocklist *newNode = malloc(sizeof(blocklist));
    newNode->data = data;
    newNode->next = NULL;
    if(*bl == NULL)
         *bl = newNode;
    else {
        blocklist *lastNode = *bl;
        while(lastNode->next != NULL) {
            lastNode = lastNode->next;
        }
        lastNode->next = newNode;
    }
}

blocklist* getCommonBlocklist(nodelist**nodes) {
    nodelist* node = *nodes;
    blocklist* bl = NULL;
    while(node != NULL) {
        if(node->id == 0) goto step;
        blocklist*block = node->blocklist;
        while(block != NULL) {
            // printf("-- %s --\n", block->data);
            if(!isBlockInBlocklist(&bl, block->data) AND block->data) {
                append(&bl, block->data);
            }
            block = block->next;
        }
        step:  
        node = node->next;
    }
    return bl;
}

// sync command 
void sync_nodes(nodelist**nodes) {
    nodelist* node = *nodes;
    blocklist* sync_block = getCommonBlocklist(&node);
    while(node) {
        node->blocklist = NULL;
        node->blocklist = sync_block;
        node = node->next;
    }
}
bool is_block_sync(blocklist*b1, blocklist*b2) {
    blocklist* bl1 = b1;
    blocklist* bl2 = b2;
    if(!bl1 AND !bl2) return true;
    while(bl1) {
        if(bl2 == NULL)
            return false;
        if(bl1->data != bl2->data)
            return false;
        bl1 = bl1->next;
        bl2 = bl2->next;
    }
    return true;
}

bool is_sync(nodelist**nodes) {
    nodelist* curr = *nodes;
    nodelist* n_next = curr->next;
    while(n_next) {
        blocklist   *b1 = curr->blocklist,
                    *b2 = n_next->blocklist;
        if(!is_block_sync(b1, b2))
            return true;
        curr = curr->next;
        n_next = n_next->next;
    }
    return false;
}

void RemoveChars(char *s, char c) {
    int writer = 0, reader = 1;

    while (s[reader]) {
        if (s[reader]==c AND s[reader+1]==c) {   
            s[writer] = s[reader++];
        } else
            s[writer++] = s[reader];
        reader++;       
    }

    s[writer]='\0';
}
void clear(char**str) {
    char* s = *str;
    for(;*s == ' ';s++);
    int len;
    for(len = 0;s[len] !='\0'; len++);
    while(s[len] == ' '){
        s[len--] = '\0';
    }
    // RemoveChars(str, ' ');
}

// void parse(char* m) {
//     int size = 0;
//     for()
// }

int blockchain() {
    nodelist *blockchain = malloc(sizeof *blockchain);
    // Genesis node
    blockchain->id = 0;
    blockchain->blocklist = malloc(sizeof(blocklist*));
    blockchain->next = NULL;
    blockchain->blocklist->data = "0";
    blockchain->blocklist->time = "00:00:00";
    blockchain->blocklist->next = NULL;
    // genessis node
    addNode(&blockchain, 2);
    addNode(&blockchain, 3);
    addNode(&blockchain, 4);
    addNode(&blockchain, 1);
    addNode(&blockchain, 5);
    addNode(&blockchain, 31);

    addBlockById(&blockchain, "block 1", 3);
    addBlockById(&blockchain, "block 1", 1);
    addBlockById(&blockchain, "block 11", 1);
    addBlockById(&blockchain, "block 2", 3);
    addBlockById(&blockchain, "block 4", 4);
    addBlockById(&blockchain, "block 45", 4);
    addBlockById(&blockchain, "block 4", 2);
    addBlockById(&blockchain, "block 5", 3);
    addBlockById(&blockchain, "block 6", 31);
    addBlockById(&blockchain, "block 2", 5);
    addBlockById(&blockchain, "block 1", 5);
    // printBlocksById(blockchain, 0);
    removeBlock(&blockchain, "block 3");
    // removeBlock(&blockchain, "block 32www");
    // printBlocksById(blockchain, 3);
    is_sync(&blockchain);
    printNodes(&blockchain, true);
    // printf("---------------------\n");
    // findBiggestBlock(&blockchain);
    sync_nodes(&blockchain);
    // printNodes(&blockchain, true);
    // printf("size: %d\n", countNodes(&blockchain));
    // deleteAllNodes(&blockchain);
    // printf("size: %d\n", countCode(&blockchain));
    // printNodes(&blockchain);
    // save_quit(&blockchain, "Nodes");
    return 1;
}


int main() {
    char* s = "  dew   rvv btbtvrv vtvtvt  ";
    printf("%ld\n", strlen(s));
    // clear(&s);
    for(;*s == ' ';s++);
    // int len;
    // for(len = 0;s[len] !='\0'; len++);
    // len--;
    // printf("len = %d\n last char >>%c<<\n", len, s[len]);
    // while(s[len] == ' '){
    //     printf("|%c|\n", s[len]);
    //     s[len] = '\0';
    //     len--;
    // }
    printf("%ld\n", strlen(s));
    // blockchain();
    return 0;
}
