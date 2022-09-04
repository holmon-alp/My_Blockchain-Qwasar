#include "main.h"
typedef int  bool;
//for parse commands
enum bool {true = 1, false = 0};
// quest08 help
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
// quest 08
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

void putch(char c) { write(1,&c, 1);}
void putstr(char* s) { while(*s) { putch(*s++);}  putch('\n'); }

bool like(char* des, char*src) {
    if(des==0 AND src==0)
        return true;
    if(des == 0)
        return false;
    if(strlen(des) != strlen(src))
        return false;
    for(int i=0; src[i] !='\0'; i++){
        if(des[i] == '\0') return false;
        if(des[i] != src[i]) return false;
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

void reverse(char s[]) {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }
 void itoa(int n, char s[]) {
     int i, sign;
 
     if ((sign = n) < 0)  
         n = -n;          
     i = 0;
     do {       
         s[i++] = n % 10 + '0';   
     } while ((n /= 10) > 0);     
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }
