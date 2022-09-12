#include "main.h"
#include <stdio.h>
enum bool {true = 1, false = 0};

typedef int  bool;

int countNodes(nodelist**node) {
    int count = 0;
    if(*node == NULL) return 0;
    nodelist*c = *node;
    while(c != NULL) {
        c = c->next;
        ++count;
    }
    return count;
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

bool addNode(nodelist** node, int id, bool hide) {
    if(id == 0){
        if(!hide)
            printf("node id cannot be '0'\n");
        return false;
    }
    if(isNodeExist(node, id)){
        error_node_alrdy_exist(id);
            return false;
    }
    nodelist* new = malloc(sizeof(nodelist*));
    new->id = id;
    new->blocklist = NULL;
    new->next = NULL;
    if(*node == NULL) {
        *node = new;
        return true;
    }
    nodelist *last = *node;
    while(last->next != NULL) {
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

void deleteNodeByKey(nodelist **head, int key) {
    nodelist *prev, *cur, *check = *head;
    if (*head != NULL && check->id == key) {
        prev = *head;
        *head = check->next;
        free(prev);
        return;
    }
    prev = NULL;
    cur  = *head;
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

// void removeBlockfromBlocklist(blocklist *block, char* key) {
//     blocklist *prev, *cur;
    
//     if (block != NULL && block->data == key) {
//         prev = block;
//         block = block->next;
//         free(prev);
//         return;
//     }
//     prev = NULL;
//     cur  = block;
//     while (cur != NULL) {
//         if (cur->data == key) {
//             if (prev != NULL) 
//                 prev->next = cur->next;
//             free(cur);
//             return;
//         } 
//         prev = cur;
//         cur = cur->next;
//     }
// }
//------------
// int deleteFirstByKey(blocklist* head, char* key)
// {
//     int totalDeleted = 0;
//     blocklist *prev, *cur;
//     /* Check if head node contains key */
//     while (head != NULL && like(head->data ,key))
//     {
//         // Get reference of head node
//         prev = head;
//         // Adjust head node link
//         head = head->next;
//         // Delete prev since it contains reference to head node
//         free(prev);
//         totalDeleted++;
//     }
//     prev = NULL;
//     cur  = head;
//     /* For each node in the list */
//     while (cur != NULL)
//     {
//         // Current node contains key
//         if (like(cur->data, key))
//         {
//             // Adjust links for previous node
//             if (prev != NULL) 
//             {
//                 prev->next = cur->next;
//             }
//             // Delete current node
//             free(cur);
//             cur = prev->next;
//             totalDeleted++;
//         } 
//         else
//         {
//             prev = cur;
//             cur = cur->next;
//         }        
//     }
//     return totalDeleted;
// }
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

void removeBlock(nodelist **head, char* data) {
    nodelist *node = *head;
    blocklist* test = malloc(sizeof(blocklist));
    test->data = data;
    bool is_block_exist = false;
    while(node != NULL) {
        blocklist** now = &node->blocklist;
        if(isBlockExistInNode(&node, data)){
            // printf("block found and delete in node %d\n", node->id);
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
        blocklist* b = last;
        while(b->next != NULL) {
            b = b->next;
        }
        b->next = *block;
        n->blocklist = last;
    }
    return 0;
}
void addBlockById(nodelist** nodes, char* data, int node_id) {
    if(!isNodeExist(nodes, node_id)) {
        error_node_exist(node_id);
        return ;
    }
    blocklist* new = malloc(sizeof(blocklist));
    new->data = data;
    new->size = strlen(data);
    new->next = NULL;
    nodelist *node = *nodes;
    while (node != NULL) {
        if (node->id == node_id) {
            if(isBlockExistInNode(&node, data)) {
                error_block_alrdy_exist(new->data);
                return ;
            }
            pushBlock(&node, &new);
            // new = NULL;
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

void printBlocksById(nodelist** list, int index) {
    nodelist* n = *list;
    while (n != NULL) {
        if(n->id == index) {
            blocklist* b = n->blocklist;
            while(b != NULL) {
                if(b->data==0) break;
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
            if(l AND !isNodeEmpty(&n)) {
               printf("%d: ", n->id);
               printBlocksById(&n, n->id);
            } 
            else
                printf("%d\n", n->id);
        n = n->next;
    }
}

// check block is exist
bool isBlockInBlocklist(blocklist** block, char* data) {
    blocklist* t = *block;
    if(*block == NULL) return false;
    while ( t ) {
        if(like(t->data, data)) 
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
        blocklist*block = node->blocklist;
        while(block != NULL) {
            if(!isBlockInBlocklist(&bl, block->data) AND block->data) {
                append(&bl, block->data);
            }
            block = block->next;
        } 
        node = node->next;
    }
    return bl;
}

// sync command 
void sync_nodes(nodelist**nodes) {
    nodelist* node = *nodes;
    while(node) {
        blocklist* sync_block = getCommonBlocklist(nodes);
        free(node->blocklist);
        node->blocklist = sync_block;
        node = node->next;
    }
}

bool is_block_sync(blocklist**b1, blocklist**b2) {
    blocklist* bl1 = *b1;
    blocklist* bl2 = *b2;
    if(*b1==0 AND *b2==0) return true;
    if((*b1==0 AND *b2!=0)OR(*b1!=0 AND *b2==0))
        return false;
    while(bl1) {
        if(bl2 == NULL)
            return false;
        if(!like(bl1->data,bl2->data))
            return false;
        bl1 = bl1->next;
        bl2 = bl2->next;
    }
    if(bl2)
        return false;
    return true;
}

bool is_sync(nodelist**nodes) {
    if(*nodes == 0) return true;
    nodelist* curr = *nodes;
    // nodelist* n_next = curr->next;
    int diff = 0;
    while(curr->next) {
        blocklist   *b1 = curr->blocklist,
                    *b2 = curr->next->blocklist;
        if(!is_block_sync(&b1, &b2))
            diff++;
        curr = curr->next;
    }
    return (!diff) ? true : false;
}

void writeLinkedList(nodelist* head){
    int fd = open(DB, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    nodelist* temp = head;
    char* arr = (char*)malloc(4050);
    int i=0;    
    while(temp!=NULL){
        char id[10];
        itoa(temp->id, id);
        strcat(arr, id);
        blocklist* b= temp->blocklist;
        while(b) {
            strcat(arr, " ");
            strcat(arr, b->data);
            b = b->next;
        }
        if(temp->next!=0)
            strcat(arr, "\n");
        i++;
        temp = temp->next;
        
    }
    if(write(fd, arr, strlen(arr)) > 0) {
        printf("Backing up blockchain...\n");
    } else {
        printf("Error write to file\n");
    }
   close(fd);
    
}
void readLinkedList(nodelist**nodes){
    int fd = open(DB, O_RDONLY);
    nodelist* temp = *nodes;
    char* res ; //= malloc(MAX_READ_SIZE);
    
    while( (res=my_readline(fd)) ) {
        char* clear_cmd = malloc(strlen(res)+1);
        trim(clear_cmd, res);
        string_array* cmd_arr = my_split(clear_cmd, " ");
        addNode(&temp, atoi(cmd_arr->array[0]), true);
        for(int i=1; i<cmd_arr->size;i++) {
                addBlockById(&temp, cmd_arr->array[i], atoi(cmd_arr->array[0]));
        }
        clear_cmd = NULL;
        free(res);
    }
    deleteNodeByKey(nodes, 0);
    return;
    
}

int blockchain(nodelist** nodes, string_array* cmds) {
    bool is_add=false,
        is_rm=false,
        is_node=false,
        is_block=false;
    if(cmds->size==1 OR cmds->size==2){
        if(like(cmds->array[0], "quit")) {
            writeLinkedList(*nodes);
            return EXIT;
        } else if(like(cmds->array[0], "ls") AND like(cmds->array[1], "-l")){
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
    } else if (cmds->size>2 AND cmds->array[2]!=0) {
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
        else if((is_rm OR is_add) AND like(cmds->array[1], "block") AND cmds->size>=3)
            is_block = true;
        else {
            error_commd;
            return 0;
        }
    } else {
        error_commd;
        return 0;
    }
    
    for(int st=2; st<cmds->size; st++) {

        if(is_add) {
            if(is_block) {
                if(cmds->size < 4) {
                    error_commd;
                    break;
                }
                if(like(cmds->array[3], ALL)) {
                    int *ids = node_ids(nodes);
                    addBlockAll(nodes, ids, cmds->array[2]);
                    break;
                } else {
                    if(st == 2) continue;
                    addBlockById(nodes, cmds->array[2], atoi(cmds->array[st]));
                    // printf("add %s to %d\n", cmds->array[2], atoi(cmds->array[st]));
                }
            } else if(is_node) {
                if(cmds->array[st] != 0)
                addNode(nodes, atoi(cmds->array[st]), false);
            }
        } else if(is_rm) {
            if(is_node) {
                if(like(cmds->array[2], ALL)){
                    deleteAllNodes(nodes);
                    break;
                } else {
                    if(cmds->array[st]!=0)
                    deleteNodeByKey(nodes, atoi(cmds->array[st]));
                }
            } else if(is_block) {
                if(cmds->array[st] != 0)
                    removeBlock(nodes, cmds->array[st]);
            }
        }
    }
    return 0;
}


int main() {
    nodelist *nodes = (nodelist*)malloc(sizeof(nodelist));
    // nodes = NULL;
    bool is_file = access(DB, F_OK ) != -1;
    if(is_file) {
        printf("Restoring From Backup\n");
    } else {
        printf("No Backup Found: Starting New Blockchain\n");
    }
    if(is_file) {
        readLinkedList(&nodes);
    }
    
    while(true) {
        char is_syn = is_sync(&nodes) ? 's' : '-';
        
        printf("[%c%d]>", is_syn, countNodes(&nodes));
        // char* s_cmd = malloc(MAX_READ_SIZE);
        // scanf("%[^\n]%*c", s_cmd);
        fflush(stdout);
        char* s_cmd = my_readline(0);
        if(like(s_cmd, "") OR like(s_cmd, " ") OR s_cmd==0) {
            // printf("you broke the rule 👢\n");
            break;
        }
        char* clear_cmd = malloc(strlen(s_cmd)+1);
        trim(clear_cmd, s_cmd);
        string_array* cmd_arr = my_split(clear_cmd, " ");
        int res = blockchain(&nodes, cmd_arr);
        if(res == EXIT)
            break;
       
    }
    return 0;
}
