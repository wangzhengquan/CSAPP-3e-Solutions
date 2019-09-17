#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
void* basic_memset(void *s, int c, size_t n) {
  size_t cnt = 0;
  unsigned char *schar = (unsigned char *) s;
  while (cnt < n) {
    *schar++ = (unsigned char) c;
    cnt++;
  }
  return s;
}
void *effective_memset2(void *s, unsigned long c, size_t n)
{
  if (n == 0) 
    return s;

  size_t K = sizeof(unsigned long);
  size_t limit ;
  size_t cnt = n;
  unsigned char *schar = s;
  unsigned long *slong;

  int rem = (size_t)schar % K;
  if (rem != 0)
  {
    limit = K - rem;
    limit = limit > n ? n : limit;
    while (limit)
    {
      *schar++ = (unsigned char) c;
      limit--;
      cnt--;
    }
  }

  if (cnt == 0)
    return s;

  slong = (unsigned long *)schar;
  while (cnt>=K )
  {
    *slong++ = (unsigned long) c;
    cnt -= K;
  }

  if (cnt == 0)
    return s;
 
  schar = (unsigned char*)slong;
  while (cnt)
  {
    *schar++ = (unsigned char) c;
    cnt--;
  }
  return s;
}
/*
 * K = sizeof(unsigned long)
 * cs store K chars for memset
 */
void* effective_memset(void *s, unsigned long cs, size_t n) {
  /* align to K */
  size_t K = sizeof(unsigned long);
  size_t cnt = 0;
  unsigned char *schar = s;
  while (cnt < n) {
    if ((size_t)schar % K == 0) {
      break;
    }
    *schar++ = (unsigned char)cs;
    cnt++;
  }

  /* set K chars one time */
  unsigned long *slong = (unsigned long *)schar;
  size_t rest = n - cnt;
  size_t loop = rest / K;
  size_t tail = rest % K;

  for (size_t i = 0; i < loop; i++) {
    *slong++ = cs;
  }

  /* pad the tail part */
  schar = (unsigned char *)slong;
  for (size_t i = 0; i < tail; i++) {
    *schar++ = (unsigned char)cs;
  }
  return s;
}

int main(int argc, char* argv[]) {
  size_t space = sizeof(char) * 65537;
  // careful! malloc SIZE_MAX size memory will make sys slow
  // or crash down
  // size_t space = SIZE_MAX;

  void* basic_space = malloc(space);
  void* effective_space = malloc(space);

  int basic_fill = 0xFF;
  unsigned long effective_fill = ~0;

  basic_memset(basic_space, basic_fill, space);
  effective_memset2(effective_space, effective_fill, space);

  assert(memcmp(basic_space, effective_space, space) == 0);

  free(basic_space);
  free(effective_space);
  return 0;
}
