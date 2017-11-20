#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

int main(void)
{
    char line[100];

    while (fgets(line, sizeof(line), stdin) != NULL) {
    
      char *p;
      
      int proc;
      char act;
      int lock;
      
      p = strtok(line,",");
      proc = atoi(p);
      
      p = strtok(NULL, ",");
      act = p[0];
      
      p = strtok(NULL, ",");
      lock = atoi(p);
      
      if (act == 'R') {
        rag_request(proc, lock);
      } else if (act == 'A') {
        rag_alloc(proc, lock);
      } else if (act == 'D') {
        rag_dealloc(proc, lock);
      } else {
        printf("parse error\n");
      }
      
    }

    int res = deadlock_detect();
    printf("Deadlock? %s.\n", res ? "yes" : "no");

    return 0;
}
