static sigjmp_buf jmpbuf;

static void readtimeout_handler(int sig)
{
  siglongjmp(jmpbuf, 1);
}

ssize_t Rio_readlineb2(rio_t *rp, void *usrbuf, size_t maxlen, int timeout)
{
  ssize_t rc;
  void (*prev_handler)(int) = SIG_DFL;
  if (!sigsetjmp(jmpbuf, 1))
  {
    prev_handler = Signal(SIGALRM, readtimeout_handler);
    alarm(timeout);
    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
      unix_error("Rio_readlineb error");
    alarm(0);
    Signal(SIGALRM, prev_handler);
    /*Signal(SIGALRM, SIG_IGN);*/
    return rc;
  }
  else
  {
    Signal(SIGALRM, prev_handler);
    /*Signal(SIGALRM, SIG_IGN);*/
    fprintf(stderr, "Rio_readlineb timeout");
    return -1;
  }
}
