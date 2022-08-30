#include "main.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

int putch(char c) { return write(1,&c, 1);}
int putstr(char* s) { while(*s) { putch(*s++);} return putch('\n'); }

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
    if(!is_block_exist) putstr("block doesn't exist");
}

int pushBlock(nodelist** nodes, blocklist** block) {
    nodelist* n = *nodes;
    blocklist *last = n->blocklist;
    if(last == NULL) {
        n->blocklist = *block;
        // printf("Add block to first (%s)\n", n->blocklist->data);
        return 0;
    } else {
        blocklist* b =  malloc(sizeof(blocklist*));
        b = last;
        while(b->next != NULL) {
            b = b->next;
        }
        b->next = *block;
        // printf("new added (%s) -- previus block (%s) - - node: %d\n\n", b->next->data, b->data, n->id);
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

void printBlocksById(nodelist** list, int index) {
    nodelist* n = *list;
    while (n != NULL) {
        if(n->id == index) {
            while(n->blocklist != NULL) {
                if(n->blocklist->data)
                    printf("%s, ", n->blocklist->data);
                n->blocklist = n->blocklist->next;
            }
           ENDL;
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
               printBlocksById(&n, n->id);
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
int countBlocks(nodelist** n) {
    nodelist* node = *n;
    blocklist* block = node->blocklist;
    int count = 0;
    while(block != NULL)  {
        ++count;
        block = block->next;
    }
    return count;
} 

bool blockInBlock(blocklist** block, char* data) {
    blocklist* t = *block;
    printf("new data: %s\n", data);
    if(t == NULL) return false;
    while ( t ) {
        printf(">>>>%s\n", t->data);
        if(t->data == data) 
            return true;
        t = t->next;
    }
    return false;
}

/* Given a reference (pointer to pointer) to the head
of a list and an int, appends a new node at the end */
void append(blocklist** head_ref, blocklist* new_block)
{
	/* 1. allocate node */
	// struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));

	blocklist *last = *head_ref; /* used in step 5*/

	/* 2. put in the data */
	// new_node->data = new_data;

	/* 3. This new node is going to be the last node, so make next
		of it as NULL*/
	new_block->next = NULL;

	/* 4. If the Linked List is empty, then make the new node as head */
	if (*head_ref == NULL)
	{
	    *head_ref = new_block;
	    return;
	}
	
	/* 5. Else traverse till the last node */
	while (last->next != NULL)
		last = last->next;

	/* 6. Change the next of last node */
	last->next = new_block;
	return;
}


blocklist* findBiggestBlock(nodelist**nodes) {
    nodelist* n = *nodes;
    blocklist* bl = malloc(sizeof(blocklist*));
    while( n ) {
        if(n->id != 0) {
            printf("node:%d \n", n->id);
            blocklist* new = n->blocklist;
            while(new != NULL) {
                if(!blockInBlock(&bl, new->data)) {
                    printf("|  %s  |\n", new->data);
                    append(&bl, new);
                }
                printf(">> %s <<\n", new->data);
                new = new->next;
            }
        }
           
        n = n->next;
    } 
    blocklist* test = bl;
    while(test) {
        printf("data: %s\n", test->data);
        test = test->next;
    }
    return bl;
}

int blockchain() {
    nodelist *blockchain = malloc(sizeof *blockchain);
    // Genesis block
    blockchain->id = 0;
    blockchain->blocklist = malloc(sizeof(blocklist*));
    blockchain->next = NULL;
    blockchain->blocklist->data = "0";
    blockchain->blocklist->hash = "0000000";
    blockchain->blocklist->next = NULL;
    // genessis block
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
    // removeBlock(&blockchain, "block 3");
    // removeBlock(&blockchain, "block 32www");
    // printBlocksById(blockchain, 3);
    printNodes(&blockchain, true);
    findBiggestBlock(&blockchain);
    // printf("size: %d\n", countNodes(&blockchain));
    // deleteAllNodes(&blockchain);
    // printf("size: %d\n", countCode(&blockchain));
    // printNodes(&blockchain);
    // save_quit(&blockchain, "Nodes");
    return 1;
}

// void parse(char* m) {

// }

int main() {
    blockchain();
    return 0;
}
