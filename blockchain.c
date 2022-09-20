
#include "main.h"
/**********************************************************************
   Q Q Q   w              w      A      S S S S        A      R R R
  Q     Q   w      w     w      A A    S              A A     R     R
 Q       Q   w    w w   w      A   A    S S S S      A   A    R    R 
  Q     Q     w  w   w w      A A A A          S    A A A A   R R R 
   Q Q Q       w      w      A       A  S S S S    A       A  R    R
        Q                                                     R     R
***********************************************************************/
int count_nodes(nodelist**node) {
    int count = 0;
    if(*node == NULL) return 0;
    nodelist*c = *node;
    while(c != NULL) {
        c = c->next;
        ++count;
    }
    return count;
}

bool is_node_exist(nodelist** nodes, int id) {
    nodelist* n = *nodes;
    while (n != NULL) {
        if(n->id == id)
            return true;
        n = n->next;
    }
    return false;
}

bool is_node_empty(nodelist** nodes) {
    nodelist* node = *nodes;
    if(node->blocklist == NULL)
        return true;
    return false;
}

bool add_node(nodelist** node, int id, bool hide) {
    if(id == 0){
        if(!hide)
            printf("node id cannot be '0'\n");
        return false;
    }
    if(is_node_exist(node, id)){
        error_node_alrdy_exist;
            return false;
    }
    nodelist* new;
    if((new = (nodelist*)malloc(sizeof(nodelist)))) {
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
    error_resourse;
    return false;
}

bool delete_all_nodes(nodelist** head) {
   nodelist* current = *head;
   if(current==NULL){
       return true;
   }
   nodelist* next ;
   while (current != NULL) {
       next = current->next;
       free(current);
       current = next;
   }
   *head = NULL;
   return true;
}

bool delete_node_by_key(nodelist **head, int key) {
    if(!is_node_exist(head, key)) {
        error_node_exist;
        return false;
    }
    nodelist *prev, *cur, *check = *head;
    if (*head != NULL && check->id == key) {
        prev = *head;
        *head = check->next;
        free(prev);
        return true;
    }
    prev = NULL;
    cur  = *head;
    while (cur != NULL) {
        if (cur->id == key) {
            if (prev != NULL) 
                prev->next = cur->next;
            free(cur);
            return true;
        } 

        prev = cur;
        cur = cur->next;
    }
    return false;
}


void delete_block(blocklist **head, char* data) {
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
        error_block_exist;
    }
}
//------------
bool is_block_exist_in_node(nodelist** node, char*data) {
    nodelist*n = *node;
    blocklist *b = n->blocklist;
    while (b != NULL) {
        if(like(b->data, data))
            return true;
        b = b->next;
    }
    return false;
}

bool remove_block(nodelist **head, char* data) {
    nodelist *node = *head;
    blocklist* test = malloc(sizeof(blocklist));
    test->data = data;
    bool is_block_exist = false;
    while(node != NULL) {
        blocklist** now = &node->blocklist;
        if(is_block_exist_in_node(&node, data)){
            delete_block(now, data);
            is_block_exist = true;
        }
        node = node->next;
    }
    if(!is_block_exist){
        error_block_exist;
        return false;
    }
    return true;
}

void push_block(nodelist** nodes, blocklist** block) {
    nodelist* n = *nodes;
    blocklist *last = n->blocklist;
    if(last == NULL) {
        n->blocklist = *block;
        return ;
    } else {
        blocklist* b = last;
        while(b->next != NULL) {
            b = b->next;
        }
        b->next = *block;
        n->blocklist = last;
    }
    return ;
}
bool add_block_by_id(nodelist** nodes, char* data, int node_id) {
    if(!is_node_exist(nodes, node_id)) {
        error_node_exist;
        return false;
    }
    blocklist* new;
    if((new = (blocklist*)malloc(sizeof(blocklist)))) {
        new->data = data;
        new->size = strlen(data);
        new->next = NULL;
        nodelist *node = *nodes;
        while (node != NULL) {
            if (node->id == node_id) {
                if(is_block_exist_in_node(&node, data)) {
                    error_block_alrdy_exist;
                    return false;
                }
                push_block(&node, &new);
                // new = NULL;
                return true;
            }
            node = node->next;
        }
        return true;
    }
    error_resourse;
    return false;
}
bool add_block_all(nodelist**nodes, char*data) {
    blocklist* new = malloc(sizeof(blocklist));
    new->data = malloc(strlen(data)+1);
    new->data = data;
    new->next = NULL;
    nodelist *node = *nodes;
    int i=0;
    while(node) {
        if(!is_block_exist_in_node(&node, data)) {
            add_block_by_id(&node, data, node->id);
            i++;
        }
        node = node->next;
    }
    if(i>0)
        return true;
    error_resourse;
    return false;
}

void print_blocks_by_id(nodelist** list, int index) {
    nodelist* n = *list;
    while (n != NULL) {
        if(n->id == index) {
            blocklist* b = n->blocklist;
            while(b != NULL) {
                if(b->data==0) break;
                    printf("%s", b->data);
                if(b->next!=NULL)
                    printf(", ");
                b = b->next;
            }
            ENDL;
            break;
        }
        n = n->next;
    }
}

void print_nodes(nodelist**list, bool l) {
    nodelist*n = *list;
    if(n==NULL){ printf("blockchain is empty\n"); return;}
    while(n != NULL) {
            if(l AND !is_node_empty(&n)) {
               printf("%d: ", n->id);
               print_blocks_by_id(&n, n->id);
            } 
            else
                printf("%d\n", n->id);
        n = n->next;
    }
}

// check block is exist
bool is_block_in_blocklist(blocklist** block, char* data) {
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
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
blocklist* get_common_blocklist(nodelist**nodes) {
    nodelist* node = *nodes;
    blocklist* bl = NULL;
    while(node != NULL) {
        blocklist*block = node->blocklist;
        while(block != NULL) {
            if(!is_block_in_blocklist(&bl, block->data) AND block->data) {
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
        blocklist* sync_block = get_common_blocklist(nodes);
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
// * ** * * * * * * * * * * * * * * ** * * ** ** * * * *  * * * ** ** * ** * * *
void write_linked_list(nodelist* head){
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
    if(write(fd, arr, strlen(arr)) >= 0) {
        printf("Backing up blockchain...\n");
    } else {
        printf("Error write to file\n");
    }
   close(fd);
    
}
void read_linked_list(nodelist**nodes){
    int fd = open(DB, O_RDONLY);
    nodelist* temp = *nodes;
    char* res ; //= malloc(MAX_READ_SIZE);
    
    while( (res=my_readline(fd)) !=NULL) {
        char* clear_cmd = malloc(strlen(res)+1);
        trim(clear_cmd, res);
        string_array* cmd_arr = my_split(clear_cmd, " ");
        add_node(&temp, atoi(cmd_arr->array[0]), true);
        for(int i=1; i<cmd_arr->size;i++) {
                add_block_by_id(&temp, cmd_arr->array[i], atoi(cmd_arr->array[0]));
        }
        clear_cmd = NULL;
        // free(res);
    }
    delete_node_by_key(nodes, 0);
    
}

int blockchain(nodelist** nodes, string_array* cmds) {
    bool is_add=false,
         is_rm=false,
         is_node=false,
         is_block=false;
    if(cmds->size==1 OR cmds->size==2){
        if(like(cmds->array[0], "quit")) {
            write_linked_list(*nodes);
            OK;
            return EXIT;
        } else if(like(cmds->array[0], "ls") AND like(cmds->array[1], "-l")){
            print_nodes(nodes, true);
            return 0;
        } else if(like(cmds->array[0], "ls")) {
            print_nodes(nodes, false);
            return 0;
        } else if(like(cmds->array[0], "sync")){
            sync_nodes(nodes);
            OK;
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
    int is_repeat = false;
    for(int st=2; st<cmds->size; st++) {

        if(is_add) {
            if(is_block) {
                if(cmds->size < 4) {
                    error_commd;
                    break;
                }
                if(like(cmds->array[3], ALL)) {
                    // int *ids = node_ids(nodes);
                    if(add_block_all(nodes, cmds->array[2]))
                        OK;
                    break;
                } else {
                    if(st == 2) continue;
                    if(add_block_by_id(nodes, cmds->array[2], atoi(cmds->array[st])) AND !is_repeat){
                        OK;
                        is_repeat = true;
                    }
                    // printf("add %s to %d\n", cmds->array[2], atoi(cmds->array[st]));
                }
            } else if(is_node) {
                if(cmds->array[st] != 0)
                if(add_node(nodes, atoi(cmds->array[st]), false) AND !is_repeat){
                    OK;
                    break;
                }
            }
        } else if(is_rm) {
            if(is_node) {
                if(like(cmds->array[2], ALL)){
                    if(delete_all_nodes(nodes)) 
                        OK;
                    break;
                } else {
                    if(cmds->array[st]!=0)
                    if(delete_node_by_key(nodes, atoi(cmds->array[st])) AND !is_repeat){
                        OK;
                        is_repeat = true;
                    }
                }
            } else if(is_block) {
                if(cmds->array[st] != 0)
                    if(remove_block(nodes, cmds->array[st]) AND !is_repeat) {
                        OK;
                        is_repeat = false;
                    }
            }
        }
    }
    return 0;
}

