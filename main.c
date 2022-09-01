#include "main.h"
#include <stdlib.h>
#include <string.h>

enum bool {true = 1, false = 0};
typedef int  bool;

//for parse commands
#ifndef STRUCT_STRING_ARRAY
#define STRUCT_STRING_ARRAY
typedef struct s_string_array {
    int size;
    char** array;
} string_array;
#endif
char *my_strtok(char *s, const char *sep)
{
    static char *p = NULL;

    if (s == NULL && ((s = p) == NULL))
        return NULL;
    s += strspn(s, sep);
    if (!*s)
        return p = NULL;
    p = s + strcspn(s, sep);
    if (*p)
        *p++ = '\0';
    else 
        p = NULL;
    return s;
}

string_array* my_split(char *a, char *b)
{
    int s = 1;
    int i = 0;
    for (int i = 0; a[i] != '\0'; i++){
        if (a[i] == b[0]) {
            s++;
        }
    }
    string_array* c = (string_array*) malloc(sizeof(string_array));

    if (strcmp(a, "") == 0 && strcmp(b, "") == 0) {
        c->size = 0;
        c->array = malloc(1 * sizeof(char*));
        char* x = (char*) malloc(sizeof(char*)*1);
        x = "";
        c->array[0] = x;
        return c;
    }else{
        c->size = s;
        char* result = (char*) malloc(sizeof(char*)*strlen(a));
        c->array = malloc(s * sizeof(char*));
        result = my_strtok(a, b);
        while (result != 0) {
            int size = strlen(result);
            char* copy_s = malloc(size + 2);
            strcpy(copy_s, result);
            c->array[i++] = copy_s;
            result = my_strtok(result + size + 1, b);
        }
        return c;
    }
}

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

bool like(char* des, char*src) {
    if(des==NULL AND src==NULL)
        return true;
    for(int i=0; src[i] !='\0'; i++){
        if(des[i] == '\0') return false;
        if(des[i] != src[i]) return false;
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
            error_node_alrdy_exist(id);
            return false;
        }
        last = last->next;
    }
    last->next = new;
    return true;
}

void deleteAllNodes(nodelist** head) {
   nodelist* current = *head;
   nodelist* next ;
   while (current != NULL) {
       next = current->next;
       free(current);
       current = next;
   }
   *head = NULL;
}

void deleteNodeByKey(nodelist *head, int key) {
    nodelist *prev, *cur;
    if (head != NULL && head->id == key) {
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
bool isBlockExistInNode(nodelist** node, blocklist** block) {
    nodelist*n = *node;
    blocklist *b = n->blocklist;
    blocklist* in = *block;
    while (b != NULL) {
        if(like(b->data, in->data))
            return true;
        b = b->next;
    }
    return false;
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
    new->data = data;
    nodelist *node = *nodes;
    if(!isNodeExist(&node, node_id)) {
        error_node_exist(node_id);
        return ;
    }
    while (node != NULL) {
        if (node->id == node_id) {
            if(isBlockExistInNode(&node, &new)) {
                error_block_alrdy_exist(new->data);
                return ;
            }
            pushBlock(&node, &new);
            break;
        }
        node = node->next;
    }
}
void addBlockAll(nodelist**nodes, char*data) {
    blocklist* new = malloc(sizeof(blocklist));
    new->data = malloc(strlen(data)+1);
    new->data = data;
    new->next = NULL;
    nodelist *node = *nodes;
    while (node != NULL) {
            if(!isBlockExistInNode(&node, &new)) {
                pushBlock(&node, &new);
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
        if(like(bl1->data,bl2->data))
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
            return false;
        curr = curr->next;
        n_next = n_next->next;
    }
    return true;
}

void trim(char* dest, char* src) {
    int readIndex = 0, writeIndex;
     while(src[readIndex] == ' '){
        readIndex++;
    }
    for(writeIndex = 0;src[readIndex] != '\0'; readIndex++){
      if(src[readIndex]==' ' && src[readIndex-1]==' '){
          continue;
      }
      dest[writeIndex] = src[readIndex];
      writeIndex++;
    }
    dest[writeIndex] = '\0';
}


int blockchain(nodelist* nodes, string_array* cmds) {
    bool is_add=false,
        is_rm=false,
        is_node=false,
        is_block=false;
    if(cmds->size==1 OR cmds->size==2){
        if(like(cmds->array[0], "quit")) {
            save_quit(&nodes, "Nodes");
            return 0;
        } else if(like(cmds->array[0], "ls") AND cmds->array[1]!=NULL AND like(cmds->array[1], "-l")){
            printNodes(&nodes, true);
            return 0;
        } else if(like(cmds->array[0], "ls")) {
            printNodes(&nodes, false);
            return 0;
        } else if(like(cmds->array[0], "sync")){
            sync_nodes(&nodes);
            return 0;
        } else {
            error_commd;
            return -1;
        }
    } else if (cmds->size>2) {
        if(like(cmds->array[0], "add"))
            is_add = true;
        else if(like(cmds->array[0], "rm"))
            is_rm = true;
        else {
            error_commd;
            return -1;
        }
        if((is_add OR is_rm) AND like(cmds->array[1], "node"))
            is_node = true;
        else if((is_rm OR is_add) AND like(cmds->array[1], "block"))
            is_block = true;
        else {
            // error_commd;
            return -1;
        }
    } else {
        error_commd;
        return -1;
    }
    
    for(int start=2; start<cmds->size; start++) {
        if(is_add) {
            if(is_node) {
                addNode(&nodes, atoi(cmds->array[start]));
            } else if(is_block AND cmds->size>3) {
                if(like(cmds->array[3], "*")) {
                    addBlockAll(&nodes, cmds->array[2]);
                    return 0;
                } else {
                    printf("start:%d\n", start);
                    addBlockById(&nodes, cmds->array[2], atoi(cmds->array[start]));                }
            } else {
                error_commd;
                return -1;
            }
        } else if(is_rm) {
            if(is_node) {
                if(like(cmds->array[2], "*")) {
                    // nodelist* n = *nodes;
                    deleteAllNodes(&nodes);
                    return 0;
                } else if(is_node AND cmds->size>2) {
                    nodelist* toDelete = nodes;
                    deleteNodeByKey(toDelete, atoi(cmds->array[start]));
                } else {
                    error_commd;
                    return -1;
                }               
            } else if(is_block) {
                removeBlock(&nodes, cmds->array[start]);
            } else {
                error_commd;
                return -1;
            }
        }
    }
    return 1;
}

int main() {
    nodelist *nodes = malloc(sizeof (*nodes));
    // Genesis node
    nodes->id = 0;
    nodes->blocklist = malloc(sizeof(blocklist*));
    nodes->next = NULL;
    nodes->blocklist->data = "0";
    nodes->blocklist->time = "00:00:00";
    nodes->blocklist->next = NULL;
    // genessis node
    
    while(true) {
        char is_syn = is_sync(&nodes) ? 's' : '-';
        printf("[%c%d]> ", is_syn, countNodes(&nodes));
        char* s_cmd = malloc(100);
        scanf("%[^\n]%*c", s_cmd);
        // printf("%s\n", s_cmd);
        char* clear_cmd = malloc(strlen(s_cmd)+1);
        trim(clear_cmd, s_cmd);
        string_array* cmd_arr = my_split(clear_cmd, " ");
        blockchain(nodes, cmd_arr);
    }



    // addNode(&nodes, 2);
    // addNode(&nodes, 3);
    // addNode(&nodes, 4);
    // addNode(&nodes, 1);
    // addNode(&nodes, 5);
    // addNode(&nodes, 31);
    // addNode(&nodes, 31);

    // addBlockById(&nodes, "block 1", 3);
    // addBlockById(&nodes, "block 1", 1);
    // addBlockById(&nodes, "block 1", 1);
    // addBlockById(&blockchain, "block 2", 3);
    // addBlockById(&blockchain, "block 4", 4);
    // addBlockById(&blockchain, "block 45", 4);
    // addBlockById(&blockchain, "block 4", 2);
    // addBlockById(&blockchain, "block 5", 3);
    // addBlockById(&blockchain, "block 6", 31);
    // addBlockById(&blockchain, "block 2", 5);
    // addBlockById(&blockchain, "block 1", 5);
    // printBlocksById(blockchain, 0);.
    // removeBlock(&nodes, "block 3");
    // // removeBlock(&blockchain, "block 32www");
    // // printBlocksById(blockchain, 3);
    // is_sync(&blockchain);
    // printNodes(&nodes, true);
    // // printf("---------------------\n");
    // // findBiggestBlock(&blockchain);
    // sync_nodes(&blockchain);
    // // printNodes(&blockchain, true);
    // // printf("size: %d\n", countNodes(&blockchain));
    // deleteAllNodes(&nodes);
    // // printf("size: %d\n", countCode(&blockchain));
    // printNodes(&nodes, true);
    // // save_quit(&blockchain, "Nodes");
   
    
    return 0;
}
