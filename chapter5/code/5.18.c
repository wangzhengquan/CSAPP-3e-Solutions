#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

double poly(double a[], double x, long degree)
{
  long i;
  double result = a[0];
  double xpwr = x;  /* Equals x^i at start of loop */
  for (i = 1; i <= degree; i++)
  {
    result += a[i] * xpwr;
    xpwr = x * xpwr;
  }
  return result;
}

double poly2(double a[], double x, long degree)
{
  long i = 1;
  double res1 = a[0];
  double xpwr1 = x;
  double res2 = 0;
  double xpwr2 = x * x;
  double res3 = 0;
  double xpwr3 = x * x * x;
  double xpwr_step = x * x * x;
  int K = 3;

  while (degree >= 3)
  {
    res1 += a[i] * xpwr1;
    res2 += a[i + 1] * xpwr2;
    res3 += a[i + 2] * xpwr3;

    xpwr1 *= xpwr_step;
    xpwr2 *= xpwr_step;
    xpwr3 *= xpwr_step;
    degree -= K;
    i += K;
  }
  while (degree > 0)
  {
    res1 += a[i] * xpwr1;
    xpwr1 = x * xpwr1;
    degree--;
  }
  res1 = res1 + res2 + res3;
  return res1;
}

/* Apply Horner’s method */
double polyh(double a[], double x, long degree)
{
  long i;
  double result = a[degree];
  for (i = degree - 1; i >= 0; i--)
    result = a[i] + x * result;
  return result;
}

/* Apply Horner’s method */
double polyh2(double a[], double x, long degree)
{
  long i;
  double result = a[degree];
  for (i = degree - 1; i >= 2; i -= 3)
    result = a[i - 2] + a[i - 1] * x  + a[i] * x * x + x * x * x * result;
  for (; i >= 0; i--)
    result = a[i] + x * result;
  return result;
}

#define LOOP 1000
#define LEN 1000
void test () {

  double x = 1;
  double degree = LEN;
  double b[LEN + 1];

  for (int c = 0; c < LOOP; c++)
  {
    poly(b, x, degree);
  }
  for (int c = 0; c < LOOP; c++)
  {
    poly2(b, x, degree);
  }
  for (int c = 0; c < LOOP; c++)
  {
    polyh(b, x, degree);
  }
  for (int c = 0; c < LOOP; c++)
  {
    polyh2(b, x, degree);
  }
}

int main(int argc, char* argv[])
{
  double a[10 + 1] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  double x = 2;
  long degree = 10;

  assert(poly(a, x, degree) == 2 + 4 + 8 + 16 + 32 + (2 + 4 + 8 + 16 + 32) * 32);
  assert(poly2(a, x, degree) == poly(a, x, degree)) ;
  assert(polyh2(a, x, degree) == poly(a, x, degree));
  test();

  return 0;
}

