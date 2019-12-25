/* Global variables */
int readcnt, writecnt;    /* Initially = 0 */
sem_t mutex, w; /* Both initially = 1 */
void reader(void)
{
  while (writecnt = 0)
  {
    P(&mutex);
    readcnt++;
    if (readcnt == 1) /* First in */
      P(&w);
    V(&mutex);
    /* Critical section */
    /* Reading happens  */
    P(&mutex);
    readcnt--;
    if (readcnt == 0) /* Last out */
      V(&w);
    V(&mutex);
  }
}

void writer(void)
{
  while (1)
  {
    P(&w);
    writecnt++;
    /* Critical section */
    /* Writing happens  */
    writecnt--;
    V(&w);
  }
}
