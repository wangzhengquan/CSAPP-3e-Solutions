/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the
 *     GET method to serve static and dynamic content.
 */
#include "csapp.h"

void doit(int fd);
int read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg);
void  sigchld_handler(int sig);
void *client_thread(void *arg);

int main(int argc, char **argv)
{
  int listenfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  pthread_t tid;
  int *connfd_ptr;

  /* Check command line args */
  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  Signal(SIGCHLD, sigchld_handler);
  listenfd = Open_listenfd(argv[1]);
  while (1)
  {
    clientlen = sizeof(clientaddr);
    connfd_ptr = (int *)Malloc(sizeof(int));
    *connfd_ptr = Accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
    Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE,
                port, MAXLINE, 0);
    printf("Accepted connection from (%s, %s)\n", hostname, port);
    
    Pthread_create(&tid, NULL, client_thread, connfd_ptr);
  }
}

void  sigchld_handler(int sig)
{
  pid_t pid;
  int status;

  while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
  {
    printf("%d exit\n", pid);
  }

}

void *client_thread(void *arg)
{
  Pthread_detach(pthread_self());
  int connfd = *(int *)arg;
  Free(arg);
  doit(connfd);
  /*echo(connfd);*/
  Close(connfd);                                            //line:netp:tiny:close
  return NULL;
}

/*
 * doit - handle one HTTP request/response transaction
 */
/* $begin doit */
void doit(int fd)
{
  int is_static;
  struct stat sbuf;
  char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  char filename[MAXLINE], cgiargs[MAXLINE];
  rio_t rio;
  int content_length = 0;

  /* Read request line and headers */
  Rio_readinitb(&rio, fd);
  if (!Rio_readlineb(&rio, buf, MAXLINE))  //line:netp:doit:readrequest
    return;
  printf("%s", buf);
  sscanf(buf, "%s %s %s", method, uri, version);       //line:netp:doit:parserequest
  if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
  {
    clienterror(fd, method, "501", "Not Implemented",
                "Tiny does not implement this method");
    return;
  }                                                    //line:netp:doit:endrequesterr

  is_static = parse_uri(uri, filename, cgiargs);       //line:netp:doit:staticcheck

  content_length = read_requesthdrs(&rio);                              //line:netp:doit:readrequesthdrs
  if (strcasecmp(method, "POST") == 0)
  {
    Rio_readnb(&rio, cgiargs, content_length);
  }

  /* Parse URI from GET request */
  if (stat(filename, &sbuf) < 0)                       //line:netp:doit:beginnotfound
  {
    clienterror(fd, filename, "404", "Not found",
                "Tiny couldn't find this file");
    return;
  }                                                    //line:netp:doit:endnotfound

  if (is_static)   /* Serve static content */
  {
    if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))   //line:netp:doit:readable
    {
      clienterror(fd, filename, "403", "Forbidden",
                  "Tiny couldn't read the file");
      return;
    }
    serve_static(fd, filename, sbuf.st_size);        //line:netp:doit:servestatic
  }
  else   /* Serve dynamic content */
  {
    if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))   //line:netp:doit:executable
    {
      clienterror(fd, filename, "403", "Forbidden",
                  "Tiny couldn't run the CGI program");
      return;
    }
    serve_dynamic(fd, filename, cgiargs);            //line:netp:doit:servedynamic
  }
}
/* $end doit */

/*
 * read_requesthdrs - read HTTP request headers
 */
/* $begin read_requesthdrs */
int read_requesthdrs(rio_t *rp)
{
  char buf[MAXLINE];
  int length = 0;
  do
  {
    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    if (strstr(buf, "Content-Length"))
    {
      sscanf(buf, "Content-Length: %d", &length);
    }
  }
  while (strcmp(buf, "\r\n"));

  return length;
}


/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
/* $begin parse_uri */
int parse_uri(char *uri, char *filename, char *cgiargs)
{
  char *ptr;

  if (!strstr(uri, "cgi-bin")) {  /* Static content */ //line:netp:parseuri:isstatic
    strcpy(cgiargs, "");                             //line:netp:parseuri:clearcgi
    strcpy(filename, ".");                           //line:netp:parseuri:beginconvert1
    strcat(filename, uri);                           //line:netp:parseuri:endconvert1
    if (uri[strlen(uri) - 1] == '/')                 //line:netp:parseuri:slashcheck
      strcat(filename, "home.html");               //line:netp:parseuri:appenddefault
    return 1;
  }
  else {  /* Dynamic content */                        //line:netp:parseuri:isdynamic
    ptr = index(uri, '?');                           //line:netp:parseuri:beginextract
    if (ptr)
    {
      strcpy(cgiargs, ptr + 1);
      *ptr = '\0';
    }
    else
      strcpy(cgiargs, "");                         //line:netp:parseuri:endextract
    strcpy(filename, ".");                           //line:netp:parseuri:beginconvert2
    strcat(filename, uri);                           //line:netp:parseuri:endconvert2
    return 0;
  }
}
/* $end parse_uri */

/*
 * serve_static - copy a file back to the client
 */
/* $begin serve_static */
void serve_static(int fd, char *filename, int filesize)
{
  int srcfd;
  char *srcp, filetype[MAXLINE], buf[MAXBUF];

  /* Send response headers to client */
  get_filetype(filename, filetype);       //line:netp:servestatic:getfiletype
  sprintf(buf, "HTTP/1.0 200 OK\r\n");    //line:netp:servestatic:beginserve
  sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
  sprintf(buf, "%sConnection: close\r\n", buf);
  sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
  sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
  Rio_writen(fd, buf, strlen(buf));       //line:netp:servestatic:endserve
  printf("Response headers:\n");
  printf("%s", buf);

  /* Send response body to client */
  /*srcfd = Open(filename, O_RDONLY, 0);    //line:netp:servestatic:open*/
  /*srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);//line:netp:servestatic:mmap*/
  /*Close(srcfd);                           //line:netp:servestatic:close*/
  /*Rio_writen(fd, srcp, filesize);         //line:netp:servestatic:write*/
  /*Munmap(srcp, filesize);                 //line:netp:servestatic:munmap*/

  srcfd = Open(filename, O_RDONLY, 0);    //line:netp:servestatic:open
  srcp = (char *)malloc(filesize);
  Rio_readn(srcfd, srcp, filesize);
  Rio_writen(fd, srcp, filesize);
  Close(srcfd);
}

/*
 * get_filetype - derive file type from file name
 */
void get_filetype(char *filename, char *filetype)
{
  if (strstr(filename, ".html"))
    strcpy(filetype, "text/html");
  else if (strstr(filename, ".gif"))
    strcpy(filetype, "image/gif");
  else if (strstr(filename, ".png"))
    strcpy(filetype, "image/png");
  else if (strstr(filename, ".jpg"))
    strcpy(filetype, "image/jpeg");
  else
    strcpy(filetype, "text/plain");
}
/* $end serve_static */

/*
 * serve_dynamic - run a CGI program on behalf of the client
 */
/* $begin serve_dynamic */
void serve_dynamic(int fd, char *filename, char *cgiargs)
{
  char buf[MAXLINE], *emptylist[] = { NULL };

  /* Return first part of HTTP response */
  sprintf(buf, "HTTP/1.0 200 OK\r\n");
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Server: Tiny Web Server\r\n");
  Rio_writen(fd, buf, strlen(buf));

  if (Fork() == 0) { /* Child */ //line:netp:servedynamic:fork
    /* Real server would set all CGI vars here */
    setenv("QUERY_STRING", cgiargs, 1); //line:netp:servedynamic:setenv
    Dup2(fd, STDOUT_FILENO);         /* Redirect stdout to client */ //line:netp:servedynamic:dup2
    Execve(filename, emptylist, environ); /* Run CGI program */ //line:netp:servedynamic:execve
  }
}
/* $end serve_dynamic */

/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
  char buf[MAXLINE], body[MAXBUF];

  /* Build the HTTP response body */
  sprintf(body, "<html><title>Tiny Error</title>");
  sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
  sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
  sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
  sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

  /* Print the HTTP response */
  sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-type: text/html\r\n");
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
  Rio_writen(fd, buf, strlen(buf));
  Rio_writen(fd, body, strlen(body));
}
/* $end clienterror */
