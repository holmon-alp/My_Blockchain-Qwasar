#include "main.h"
#include <stdlib.h>
#include <string.h>

enum bool {true = 1, false = 0};
typedef int  bool;
#define all_key 100001
//for parse commands

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
    if(des==0 AND src==0)
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

int* node_ids(nodelist** nodes) {
    int *res = malloc(sizeof(*nodes)), i=0;
    nodelist*n = *nodes;
    while(n) {
        res[i++] = n->id;
        n = n->next;
    }
    return res;
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
    if(*node == NULL) {
        *node = new;
        return true;
    }
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
bool isBlockExistInNode(nodelist** node, char*data) {
    nodelist*n = *node;
    blocklist *b = n->blocklist;
    while (b != NULL) {
        if(like(b->data, data))
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
//------------
int deleteFirstByKey(blocklist* head, char* key)
{
    int totalDeleted = 0;
    blocklist *prev, *cur;

    /* Check if head node contains key */
    while (head != NULL && like(head->data ,key))
    {
        // Get reference of head node
        prev = head;
        // Adjust head node link
        head = head->next;
        // Delete prev since it contains reference to head node
        free(prev);

        totalDeleted++;
    }

    prev = NULL;
    cur  = head;

    /* For each node in the list */
    while (cur != NULL)
    {
        // Current node contains key
        if (like(cur->data, key))
        {
            // Adjust links for previous node
            if (prev != NULL) 
            {
                prev->next = cur->next;
            }

            // Delete current node
            free(cur);

            cur = prev->next;

            totalDeleted++;
        } 
        else
        {
            prev = cur;
            cur = cur->next;
        }        

    }

    return totalDeleted;
}
void delete_block(blocklist **head, char* data)
{
 blocklist *tmp = NULL;
 blocklist *tmp1 = NULL;

 if (head == NULL || *head == NULL) return;

 if(like((*head)->data ,data)) {
  /*Delete the head node*/
    tmp = *head;
    *head = (*head)->next;
    free(tmp);
    return;
 }

 tmp = *head;

 while(tmp->next && !like(tmp->next->data, data)) tmp = tmp->next;

 if(tmp->next) {
  tmp1 = tmp->next;
  tmp->next = tmp1->next;
  free(tmp1);
 } else {
     error_block_exist(data);
 }
}
//------------


void removeBlock(nodelist **head, char* data) {
    nodelist *node = *head;
    blocklist* test = malloc(sizeof(blocklist));
    test->data = data;
    bool is_block_exist = false;
    while(node != NULL) {
        blocklist** now = &node->blocklist;
        if(isBlockExistInNode(&node, data)){
            printf("found block %d\n", node->id);
            // removeBlockfromBlocklist(now, data);
            // deleteFirstByKey(*now, data);
            delete_block(now, data);
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
    new->next = NULL;
    nodelist *node = *nodes;
    if(!isNodeExist(&node, node_id)) {
        error_node_exist(node_id);
        return ;
    }
    while (node != NULL) {
        if (node->id == node_id) {
            if(isBlockExistInNode(&node, data)) {
                error_block_alrdy_exist(new->data);
                return ;
            }
            pushBlock(&node, &new);
            new = NULL;
            return;
        }
        node = node->next;
    }
}
void addBlockAll(nodelist**nodes, int*ids, char*data) {
    blocklist* new = malloc(sizeof(blocklist));
    new->data = malloc(strlen(data)+1);
    new->data = data;
    new->next = NULL;
    nodelist *node = *nodes;
    int i=0;
    while(node) {
        addBlockById(&node, data, ids[i++]);
        node = node->next;
    }
}

// void addBlockAll(nodelist**nodes, char*data) {
//     blocklist* new = malloc(sizeof(blocklist));
//     new->data = malloc(strlen(data)+1);
//     new->data = data;
//     new->next = NULL;
//     nodelist *node = *nodes;
//     while (node != NULL) {
//         if(node->id == 0) goto jump; 
//             if(!isBlockExistInNode(&node, data)) {
//                 // printf("block doesn't exist- %s --> %d\n", new->data, node->id);
//                 pushBlock(&node, &new);
//             }
//         jump:
//         node = node->next;
//     }
//     new =NULL;
// }

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
    if(n==NULL){ printf("blockchain is empty\n"); return;}
    while(n != NULL) {
        // if(n->id != 0) {
            if(l AND !isNodeEmpty(&n)) {
               printf("node - %d: ", n->id);
               printBlocksById(n, n->id);
            } 
            else
                printf("node - %d\n", n->id);
        // } 
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
        // if(node->id == 0) goto step;
        blocklist*block = node->blocklist;
        while(block != NULL) {
            // printf("-- %s --\n", block->data);
            if(!isBlockInBlocklist(&bl, block->data) AND block->data) {
                append(&bl, block->data);
            }
            block = block->next;
        }
        // step:  
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
    if(*nodes == 0) return true;
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


you_do* blockchain(nodelist** nodes, string_array* cmds) {
    bool is_add=false,
        is_rm=false,
        is_node=false,
        is_block=false,
        is_all;
    you_do* reply = malloc(sizeof(you_do*));
    reply->add_node = false;
    reply->rm_node = false;
    reply->rm_all_nodes = false;
    reply->add_block = false;
    reply->add_block_all = false;
    reply->rm_block = false;
    if(cmds->size==1 OR cmds->size==2){
        if(like(cmds->array[0], "quit")) {
            save_quit(nodes, "Nodes");
            return 0;
        } else if(like(cmds->array[0], "ls") AND cmds->array[1]!=NULL AND like(cmds->array[1], "-l")){
            printNodes(nodes, true);
            return 0;
        } else if(like(cmds->array[0], "ls")) {
            printNodes(nodes, false);
            return 0;
        } else if(like(cmds->array[0], "sync")){
            sync_nodes(nodes);
            return 0;
        } else {
            error_commd;
            return 0;
        }
    } else if (cmds->size>2) {
        if(like(cmds->array[0], "add"))
            is_add = true;
        else if(like(cmds->array[0], "rm"))
            is_rm = true;
        else {
            error_commd;
            return 0;
        }
        if((is_add OR is_rm) AND like(cmds->array[1], "node"))
            is_node = true;
        else if((is_rm OR is_add) AND like(cmds->array[1], "block"))
            is_block = true;
        else {
            error_commd;
            return 0;
        }
    } else {
        error_commd;
        return 0;
    }
    if(cmds->size >= 3) {
        if(like(cmds->array[2],"*") OR like(cmds->array[3], "*"))
            is_all = true;
    }
    if(is_rm AND is_node AND is_all)
        reply->rm_all_nodes = true;
    else if(is_rm AND is_node)
        reply->rm_node = true;
    else if(is_rm AND is_block)
        reply->rm_block = true;
    else if(is_add AND is_node)
        reply->add_node = true;
    else if(is_add AND is_block AND is_all)
        reply->add_block_all = true;
    else if(is_add AND is_block)
        reply->add_block = true;
    reply->array = malloc(sizeof(cmds));
    reply->array->size = cmds->size-2;
    int i=0;
    for(int st=2; st<cmds->size; st++) {
        reply->array->array[i++] = cmds->array[st];
        // if(is_add) {
        //     if(is_node) {
        //         addNode(nodes, atoi(cmds->array[start]));
        //     } else if(is_block AND cmds->size>3) {
        //         if(like(cmds->array[3], "*")) {
        //             int* ids = node_ids(nodes);
        //             addBlockAll(nodes, ids, "er");
        //             return 0;
        //         } else {
        //             printf("start:%d\n", start);
        //             addBlockById(nodes, cmds->array[2], atoi(cmds->array[start]));                }
        //     } else {
        //         error_commd;
        //         return 0;
        //     }
        // } else if(is_rm) {
        //     if(is_node) {
        //         if(like(cmds->array[2], "*")) {
        //             // nodelist* n = *nodes;
        //             deleteAllNodes(nodes);
        //             return 0;
        //         } else if(is_node AND cmds->size>2) {
        //             nodelist* toDelete = *nodes;
        //             deleteNodeByKey(toDelete, atoi(cmds->array[start]));
        //         } else {
        //             error_commd;
        //             return 0;
        //         }               
        //     } else if(is_block) {
        //         removeBlock(nodes, cmds->array[start]);
        //     } else {
        //         error_commd;
        //         return 0;
        //     }
        // }
    }
    return reply;
}

int main() {
    nodelist *nodes = malloc(sizeof (*nodes));
    nodes = NULL;
    
    while(true) {
        char is_syn = is_sync(&nodes) ? 's' : '-';
        printf("[%c%d]> ", is_syn, countNodes(&nodes));
        char* s_cmd = malloc(100);
        scanf("%[^\n]%*c", s_cmd);
        // printf("%s\n", s_cmd);
        char* clear_cmd = malloc(strlen(s_cmd)+1);
        trim(clear_cmd, s_cmd);
        string_array* cmd_arr = my_split(clear_cmd, " ");
        you_do *reply = blockchain(&nodes, cmd_arr);
        if(reply != 0) {
            if(reply->rm_all_nodes)
                deleteAllNodes(&nodes);
            else if(reply->rm_node){
                for(int i=0; reply->array->array[i]!=NULL; i++)
                    deleteNodeByKey(nodes, atoi(reply->array->array[i]));
            } else if(reply->rm_block){
                for(int i=0; reply->array->array[i]!=NULL; i++)
                    removeBlock(&nodes, reply->array->array[i]);
            } else if(reply->add_node) {
                for(int i=0; reply->array->array[i]!=NULL; i++)
                    addNode(&nodes, atoi(reply->array->array[i]));
            } else if (reply->add_block_all) {
                int *ar = node_ids(&nodes);
                addBlockAll(&nodes, ar, reply->array->array[0] );
            } else if(reply->add_block) {
                for(int i=1; reply->array->array[i]!=NULL; i++)
                addBlockById(&nodes, reply->array->array[0], atoi(reply->array->array[i]));
            }
        }
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
    // addBlockById(&nodes, "block 2", 3);
    // addBlockById(&nodes, "block 43", 4);
    // addBlockById(&nodes, "block 45", 4);
    // addBlockById(&nodes, "block 43", 2);
    // addBlockById(&blockchain, "block 5", 3);
    // addBlockById(&blockchain, "block 6", 31);
    // addBlockById(&blockchain, "block 2", 5);
    // addBlockById(&blockchain, "block 1", 5);
    // printBlocksById(blockchain, 0);
    // int * ids = node_ids(&nodes);
    // printNodes(&nodes, true);
    // removeBlock(&nodes, "block 45");
    // removeBlock(&nodes, "block 43");
    // addBlockAll(&nodes, ids, "new");
    // addBlockById(&nodes, "block 99", 2);
    // // removeBlock(&blockchain, "block 32www");
    // // printBlocksById(blockchain, 3);
    // is_sync(&blockchain);
    // printNodes(&nodes, true);
    // // printf("---------------------\n");
    // sync_nodes(&blockchain);
    // // printNodes(&blockchain, true);
    // // printf("size: %d\n", countNodes(&blockchain));
    // deleteAllNodes(&nodes);
    // addNode(&nodes, 1);
    // // printf("size: %d\n", countCode(&blockchain));
    // printNodes(&nodes, true);
    // // save_quit(&blockchain, "Nodes");
   
    
    return 0;
}
