
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

bool startsWith(char* src, char*sub) {
    for(int i=0; sub[i]!='\0'; i++) {
        if(src[i] != sub[i])
            return false;
    }
    return true;
}

void addLast(nodelist **head, int val) {
    nodelist *newNode = malloc(sizeof(nodelist));
    newNode->id = val;
    newNode->next = NULL;
    if(*head == NULL)
         *head = newNode;
    else {
        nodelist *lastNode = *head;
        while(lastNode->next != NULL)
            lastNode = lastNode->next;
        lastNode->next = newNode;
    }
}

bool addNode(nodelist** node, int id) {
    nodelist* new = malloc(sizeof(nodelist*));
    new->id = id;
    new->next = NULL;
    nodelist *last = *node;
    while(last->next != NULL) {
        if(last->id == id) {
            perror("this node already exists");
            return false;
        }
        last = last->next;
    }
    last->next = new;
    printf("node id : %d\n", last->next->id);
    return true;
}

int blockchain() {

    nodelist *blockchain = malloc(sizeof *blockchain);
    blockchain->id = 1;
    blockchain->blocklist = malloc(sizeof(blocklist*));
    blockchain->next = NULL;
    blockchain->blocklist->data = "Block !!";
    blockchain->blocklist->hash = "bwefjkb2jk3huhbvhi5u4njhgj#$#@WFe";
    blockchain->blocklist->next = NULL;
    printf("blockchain.\nid: %d\n", blockchain->id);
    printf("block data: %s\n", blockchain->blocklist->data);
    printf("block hash: %s\n", blockchain->blocklist->hash);
    addNode(&blockchain, 2);
    // addLast(&blockchain, 2);
    // printf("new node: %d\n", blockchain->next->id);
    return 1;
}

int main() {
    
    blockchain();
    return 0;
}
