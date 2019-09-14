#include <stdio.h>

long ncopy(long *src, long *dst, long len) {
  long limit = len -3;
  long i, count=0;
  long val;
  for (i=0; i<limit; i+=4) {
    val = *src;
    *dst = val;
    if(val>0) count++;
    val = *(src+1); 
    *(dst+1) = val;
    if(val>0)
      count++;
    val = *(src+2); 
    *(dst+2) = val;
    if(val>0)
      count++;
    val = *(src+3); 
    *(dst+3) = val;
    if(val>0)
      count++;
    src += 4;
    dst += 4;
  }
  for (; i<len; i++) {
    *dst = *src;
  }
  return count;
}
