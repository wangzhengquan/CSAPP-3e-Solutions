#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define N  60 
#define B  16
#define S 256
typedef uint8_t Block[B];

typedef struct
{
  int valid ;
  int tag;
  Block block;

} Line;

typedef struct
{
  Line lines;
} Set;

typedef Set Sets[S];

long count = 0;

uint32_t get_from_cache(int row, int col, uint8_t arr[], Sets cache_sets, long *hit, long *miss )
{
  /*printf("%ld\t", count++);*/
  int addr = (row * N + col) * 4;
  int offset = addr & 0xf;
  int index = (addr >> 4) & 0xff;
  int tag = addr >> 12;

  Line *line =  &(cache_sets[index].lines);
  if (line->tag == tag && line -> valid)
  {
    /*printf("1 ");*/
    (*hit)++;
    return *(uint32_t*)((line->block) + offset);
  }
  else
  {
    int addr_b = addr & (~0xf);
    int i;
    for (i = 0; i < B; i++)
    {
      line -> block[i] = arr[addr_b + i];
    }
    line -> valid = 1;
    line -> tag = tag;

    /*printf("0 ");*/
    (*miss)++;
    return *(uint32_t*)((line->block) + offset);

  }
}

typedef uint32_t array_t[N * N];

uint32_t sumA(array_t a)
{
  long hit = 0, miss = 0;
  Sets cache_sets;
  memset(cache_sets, 0, sizeof(cache_sets));

  int i, j;
  uint32_t sum = 0;
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      /*sum += a[i*N+j]; */
      sum += get_from_cache(i, j, (uint8_t*)a, cache_sets, &hit, &miss);
    }
  }
  printf("A hit=%ld, miss=%ld, miss rate=%f\n", hit, miss, (double)miss / (miss + hit));
  return sum;
}

uint32_t sumB(array_t a)
{
  long hit = 0, miss = 0;
  Sets cache_sets;
  memset(cache_sets, 0, sizeof(cache_sets));

  int i, j;
  uint32_t sum = 0;
  for (j = 0; j < N; j++)
  {
    for (i = 0; i < N; i++)
    {
      /*sum += a[i][j]; */
      sum += get_from_cache(i, j, (uint8_t*)a, cache_sets, &hit, &miss);
    }
  }
  printf("B hit=%ld, miss=%ld, miss rate=%f\n", hit, miss, (double)miss / (miss + hit));
  return sum;
}

uint32_t sumC(array_t a)
{
  long hit = 0, miss = 0;
  Sets cache_sets;
  memset(cache_sets, 0, sizeof(cache_sets));

  int i, j;
  uint32_t sum = 0;
  for (j = 0; j < N; j += 2 )
  {
    for (i = 0; i < N; i += 2 )
    {
      /*sum += a[i][j]; */
      sum += get_from_cache(i, j, (uint8_t*)a, cache_sets, &hit, &miss)
             + get_from_cache(i + 1, j, (uint8_t*)a, cache_sets, &hit, &miss)
             + get_from_cache(i, j + 1, (uint8_t*)a, cache_sets, &hit, &miss)
             + get_from_cache(i + 1, j + 1, (uint8_t*)a, cache_sets, &hit, &miss) ;
    }
  }
  printf("C hit=%ld, miss=%ld, miss rate=%f\n", hit, miss, (double)miss / (miss + hit));
  return sum;
}

int main()
{
  printf("N=%d\n", N);
  array_t a;
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      a[i * N + j] = i * N + j;
    }
  }
  uint32_t sum_a = sumA(a);
  printf("sum_a=%d\n", sum_a);
  uint32_t sum_b = sumB(a);
  printf("sum_b=%d\n", sum_b);
  uint32_t sum_c = sumC(a);
  printf("sum_c=%d\n", sum_c);
}
