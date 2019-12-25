#include "csapp.h"


void echo_clients();
int echo_line(int connfd);
void command(void);

int  listenfd, maxfd;
fd_set read_set, ready_set;
int main(int argc, char **argv)
{
  int connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(0);
  }
  listenfd = Open_listenfd(argv[1]);  //line:conc:select:openlistenfd
  maxfd = listenfd;

  FD_ZERO(&read_set);              /* Clear read set */ //line:conc:select:clearreadset
  FD_SET(STDIN_FILENO, &read_set); /* Add stdin to read set */ //line:conc:select:addstdin
  FD_SET(listenfd, &read_set);     /* Add listenfd to read set */ //line:conc:select:addlistenfd

  while (1)
  {
    ready_set = read_set;
    Select(maxfd + 1, &ready_set, NULL, NULL, NULL); //line:conc:select:select
    if (FD_ISSET(STDIN_FILENO, &ready_set)) //line:conc:select:stdinready
      command(); /* Read command line from stdin */
    if (FD_ISSET(listenfd, &ready_set))   //line:conc:select:listenfdready
    {
      clientlen = sizeof(struct sockaddr_storage);
      connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
      FD_SET(connfd, &read_set);
      maxfd = maxfd > connfd ? maxfd : connfd;
    }
    echo_clients();
  }
}

void echo_clients()
{
  for (int connfd = listenfd + 1; connfd <= maxfd; connfd++)
  {
    if (FD_ISSET(connfd, &ready_set))
    {
      if (echo_line(connfd) == 0 )
      {
        Close(connfd);
        FD_CLR(connfd, &ready_set);
      }
    }
  }
}


int echo_line(int connfd)
{
  int n;
  int total = 0;
  char buf[MAXLINE];

  while ((n = Rio_readn(connfd, buf, 1)) > 0)
  {
    printf("server received %d bytes\n", n);
    Rio_writen(connfd, buf, n);
    total += n;
    if (buf[0] == '\n')
      break;
  }
  return total;
}

void command(void)
{
  char buf[MAXLINE];
  if (!Fgets(buf, MAXLINE, stdin))
    exit(0); /* EOF */
  printf("%s", buf); /* Process the input command */
}
