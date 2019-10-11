/*
 * convert.c
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// a large prime number
#define MATRIX_N 5000
#define MEM_SIZE (sizeof(int) * MATRIX_N * MATRIX_N)
#define BLOCK 8

void randomize(int *arr, int N)
{
  srand(time(0));

  int i, j;
  for (i = 0; i <= N - 1; i++)
    for (j = 0; j <= N - 1; j++)
      arr[i * N + j] = rand() % 2;
}

void convert(int *src, int N)
{
  int i, j;

  for (i = 0; i <= N - 1; i++)
    for (j = 0; j <= N - 1; j++)
      src[j * N + i] = src[i * N + j] || src[j * N + i];
}

void effect_convert1(int *G, int dim)
{
  int i, j;
  int val;
  for (i = 0; i < dim; i++)
    for (j = 0; j < i; j++)
    {
      val = G[i * dim + j] || G[j * dim + i];
      G[i * dim + j] = val;
      G[j * dim + i] = val;
    }
}

void effect_convert2(int *G, int dim)
{
  int i , j, a, b;
  int val;
  for (i = dim - 1; i >= BLOCK; i -= BLOCK)
  {
    for (j = i; j >= BLOCK; j -= BLOCK)
    {
      for (a = i; a > i - BLOCK; a--)
        for (b = j; b > j - BLOCK; b--)
        {
          val = G[a * dim + b] || G[b * dim + a];
          G[a * dim + b] = val;
          G[b * dim + a] = val;
        }
    }
    for (; j >= 0; j--)
    {
      for (a = i; a > i - BLOCK; a--)
      {
        val = G[a * dim + j] || G[j * dim + a];
        G[a * dim + j] = val;
        G[j * dim + a] = val;
      }
    }
  }

  for (; i >= 0; i--)
  {
    for (j = i; j >= BLOCK; j -= BLOCK)
    {
      for (b = j; b > j - BLOCK; b--)
      {
        val = G[i * dim + b] || G[b * dim + i];
        G[i * dim + b] = val;
        G[b * dim + i] = val;
      }
    }
    for (; j >= 0; j--)
    {
      val = G[i * dim + j] || G[j * dim + i];
      G[i * dim + j] = val;
      G[j * dim + i] = val;
    }

  }

}



void test(void)
{
  int *s = (int *)malloc(MEM_SIZE);
  int *e = (int *)malloc(MEM_SIZE);
  int *e2 = (int *)malloc(MEM_SIZE);

  randomize(s, MATRIX_N);
  memcpy(e, s, MEM_SIZE);
  memcpy(e2, s, MEM_SIZE);


  convert(s, MATRIX_N);
  effect_convert1(e, MATRIX_N);
  effect_convert2(e2, MATRIX_N);

  for (int i = 0; i < MATRIX_N; i++)
    for (int j = 0; j < MATRIX_N; j++)
    {
      assert(s[i * MATRIX_N + j] == e[i * MATRIX_N + j] );
      assert(s[i * MATRIX_N + j] == e2[i * MATRIX_N + j] );
    }

  free((void *)s);
  free((void *)e);
  free((void *)e2);
}

int main(int argc, char *argv[])
{
  test();

  /* prof(); */
  /* prof_effect(); */

  return 0;
}
