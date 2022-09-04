#include "main.h"
#ifndef READLINE_READ_SIZE
#define READLINE_READ_SIZE 10
#endif
int my_strlen(char* p1)
{
    int length = 0;
    while ( (p1[length++]) );
    return length - 1;
}

void my_strcpy(char* p1, char* p2)
{
    for (int i = 0; (p1[i] = p2[i]) ; i++);
}

char* my_realloc(char* string, int old_size, int size)
{
    char* out = malloc(size);
    for (int i = 0; i < old_size ; i++)
        out[i] = string[i];
    free(string);
    return out;
}

char* read_buf(int fd, char* buffer, int* out_size, int* buf_size, int* read_size)
{
    int stop = 0;
    *out_size = 0;

    while( !stop )
    {
        for( ; *out_size < *buf_size; (*out_size)++ )
        {
            if (buffer[*out_size] == '\n')
            {
                stop = 1;
                break;
            }
        }

        if(stop == 1)
            break;

        buffer = my_realloc(buffer, *buf_size, (*buf_size + READLINE_READ_SIZE + 1) );
        *read_size = read(fd, &buffer[*out_size], READLINE_READ_SIZE);

        if ( *read_size > 0 )
            *buf_size += *read_size;
        else
            stop = 1;
    }

    return buffer;
}

char* split_buf(char* buffer, char* output, char* rest, int out_size, int buf_size)
{
    int j, z;

    output = my_realloc(output, 1, out_size + 1);

    for ( j = 0 ; j < out_size; j++ )
        output[j] = buffer[j];
    output[j++] = '\0';

    for ( z = 0 ; j < buf_size; z++ )
        rest[z] = buffer[j++];
    rest[z] = '\0';

    return output;
}

char* my_readline(int fd)
{
    static char rest[READLINE_READ_SIZE + 1];
    char *output = malloc(1);
    char *buffer = malloc(READLINE_READ_SIZE + 1);
    int out_size = 0;
    int read_size = 0;
    int buf_size = my_strlen(rest);
    my_strcpy(buffer, rest);

    buffer = read_buf(fd, buffer, &out_size, &buf_size, &read_size);
    output = split_buf(buffer, output, rest, out_size, buf_size);

    free(buffer);
    
    if (read_size < 0)
    {
        free(output);
        write(1, "my_readline: Error reading file.\n", 33);
        return (char*)NULL;
    }
    else if (read_size == 0 && buf_size == 0)
    {
        free(output);
        // write(1, "my_readline: End of file was reached.\n", 38);
        return 0;
    }
    else
        return output;
}
