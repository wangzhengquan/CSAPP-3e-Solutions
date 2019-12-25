/*
 * 12.31.c
 */
#include <stdio.h>
#include "csapp.h"

sigjmp_buf buf;

void sigchild_handler(int sig)
{
  siglongjmp(buf, 1);
}

char *tfgets(char *s, int size, FILE *stream)
{
  if (Fork() == 0)
  {
    Sleep(5);
    exit(0);
  }
  handler_t *pre_handler = SIG_DFL;
  switch (sigsetjmp(buf, 1))
  {
  case 0:
    pre_handler =  Signal(SIGCHLD, sigchild_handler);
    char * re = fgets(s, size, stream);
    Signal(SIGCHLD, pre_handler);
    return re;
  case 1:
    Signal(SIGCHLD, pre_handler);
    return NULL;
  }
}

int main(int argc, char* argv[])
{
  char buf[MAXLINE];

  if (tfgets(buf, MAXLINE, stdin) == NULL)
    printf("BOOM!\n");
  else
    printf("%s", buf);

  return 0;
}



