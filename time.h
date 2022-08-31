// #include <time.h>
// #include <stdio.h>
// #include <string.h>

// int main(void)
// {
//     time_t mytime = time(NULL);
//     char * time_str = ctime(&mytime);
//     time_str[strlen(time_str)-1] = '\0';
//     printf("Current Time : %s\n", time_str);

//     return 0;
// }
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
void format_time(char *output){
    time_t rawtime;
    struct tm * timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    sprintf(output, "[%d %d %d %d:%d:%d]", timeinfo->tm_mday,
            timeinfo->tm_mon + 1, timeinfo->tm_year + 1900,
            timeinfo->tm_hour+5, timeinfo->tm_min, timeinfo->tm_sec);
}
int main ()
{
    char* out = malloc(50);
    format_time(out);
    printf("%s\n", out);
//   time_t rawtime;
//   struct tm * timeinfo;

//   time ( &rawtime );
//   timeinfo = localtime ( &rawtime );
//   printf ( "Current local time and date: %s", asctime (timeinfo) );
  
  return 0;
}
