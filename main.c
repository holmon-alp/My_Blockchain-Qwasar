#include "main.h"

/******************************************************************************************************
B B B   L         O O O    C C C C  K       K  C C C C  H       H        A        I I I I I  NN      N
B    B  L        O     O   C        K     K    C        H       H       A A           I      N N     N
B   B   L       O       O  C        K   K      C        H       H      A   A          I      N  N    N
B B     L       O       O  C        K  K       C        H H H H H     A A A A         I      N   N   N
B   B   L        O     O   C        K    K     C        H       H    A       A        I      N    N  N
B    B  L         O   O    C        K      K   C        H       H   A         A       I      N     N N
B B B   L L L L    O O     C C C C  K       K  C C C C  H       H  A           A  I I I I I  N      NN

From Qwasar Students Mirjamol Mirislomov and Hasanov Oybek.
     date: 20/09/2022
******************************************************************************************************/
int main() {
    nodelist *nodes = (nodelist*)malloc(sizeof(nodelist));
    bool is_file = access(DB, F_OK ) != -1;
    if(is_file) {
        printf("Restoring From Backup\n");
        read_linked_list(&nodes);
    } else {
        printf("No Backup Found: Starting New Blockchain\n");
        nodes = NULL;
    }
        
    while(true) {
        char is_syn = is_sync(&nodes) ? 's' : '-';
        printf("[%c%d]>", is_syn, count_nodes(&nodes));
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
