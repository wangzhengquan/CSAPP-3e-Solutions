/* $begin sbufc */
#include "csapp.h"
#include "sbuf.h"

/* Create an empty, bounded, shared FIFO buffer with n slots */
/* $begin sbuf_init */
void sbuf_init(sbuf_t *sp, int n)
{
  sp->buf = Calloc(n, sizeof(int));
  sp->n = n;                       /* Buffer holds max of n items */
  sp->front = sp->rear = 0;        /* Empty buffer iff front == rear */
  Sem_init(&sp->mutex, 0, 1);      /* Binary semaphore for locking */
  Sem_init(&sp->slots, 0, n);      /* Initially, buf has n empty slots */
  Sem_init(&sp->items, 0, 0);      /* Initially, buf has zero data items */
}
/* $end sbuf_init */

/* Clean up buffer sp */
/* $begin sbuf_deinit */
void sbuf_deinit(sbuf_t *sp)
{
  Free(sp->buf);
}
/* $end sbuf_deinit */

/* Insert item onto the rear of shared buffer sp */
/* $begin sbuf_insert */
void sbuf_insert(sbuf_t *sp, int item)
{
  P(&sp->slots);                          /* Wait for available slot */
  P(&sp->mutex);                          /* Lock the buffer */
  //sp->rear = (++sp->rear) % (sp->n);
  sp->buf[(++sp->rear) % (sp->n)] = item;   /* Insert the item */
  V(&sp->mutex);                          /* Unlock the buffer */
  V(&sp->items);                          /* Announce available item */
}
/* $end sbuf_insert */

/* Remove and return the first item from buffer sp */
/* $begin sbuf_remove */
int sbuf_remove(sbuf_t *sp)
{
  int item;
  P(&sp->items);                          /* Wait for available item */
  P(&sp->mutex);                          /* Lock the buffer */
  //sp->front = (++sp->front) % (sp->n);
  item = sp->buf[(++sp->front) % (sp->n)];  /* Remove the item */
  V(&sp->mutex);                          /* Unlock the buffer */
  V(&sp->slots);                          /* Announce available slot */
  return item;
}

int sbuf_isempty(sbuf_t *sp)
{
  P(&sp->mutex);                          /* Lock the buffer */
  int b = sp->front == sp->rear;
  V(&sp->mutex);                          /* Unlock the buffer */
  return b;
}

int sbuf_isfull(sbuf_t *sp)
{
  P(&sp->mutex);                          /* Lock the buffer */
  int b = (sp->rear + 1) % sp->n == sp->front % sp->n;
  V(&sp->mutex);                          /* Unlock the buffer */
  return b;
}
