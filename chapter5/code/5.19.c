
#include <stdio.h>
#include <assert.h>

void psum(float a[], float p[], long n)
{
  long i;
  p[0] = a[0];
  for (i = 1; i < n; i++)
    p[i] = p[i - 1] + a[i];
}

void psum1a(float a[], float p[], long n)
{
  long i;
  float val;
  val = p[0] = a[0];
  for (i = 1; i < n; i++)
  {
    val = val + a[i];
    p[i] = val;
  }
}

void psum_4_1a(float a[], float p[], long n)
{
  long i;
  float val0, val1, val2, val3;
  val0 = p[0] = a[0];
  long limit = n - 3;
  for (i = 1; i < limit; i += 4)
  {
    val0 = val0 + a[i] ;
    p[i] = val0;

    val1 = val0 + a[i + 1] ;
    p[i + 1] = val1;

    val2 = val1 + a[i + 2] ;
    p[i + 2] = val2;

    val3 = val2 + a[i + 3] ;
    p[i + 3] = val3;

    val0 = val3;

  }

  for (; i < n; i++)
  {
    val0 = val0 + a[i];
    p[i] = val0;
  }
}

#define LOOP 1000
#define LEN  1000

int main(int argc, char* argv[])
{
  float a[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  float p[10];
  psum1a(a, p, 10);
  float q[10];
  psum_4_1a(a, q, 10);
  assert(q[9] == p[9]);

  /* for prof */
  for (int i = 0; i < LOOP; i++)
  {
    float s[LEN];
    float d[LEN];
    psum1a(s, d, LEN);
    psum_4_1a(s, d, LEN);
  }
  return 0;
}
