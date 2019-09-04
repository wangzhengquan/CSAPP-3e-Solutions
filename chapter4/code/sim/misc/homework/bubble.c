#include <stdio.h>
void bubble_a(long *data, long count)
{
  long i, last;
  for (last = count - 1; last > 0; last--)
  {
    for (i = 0; i < last; i++)
      if (data[i + 1] < data[i])
      {
        long t = data[i + 1];
        data[i + 1] = data[i];
        data[i] = t;
      }
  }
}

void bubble_b(long *data, long count)
{
  long *i, *last;
  for (last = data + count - 1; last > data; last--)
  {
    for (i = data; i < last; i++)
      if (*(i + 1) < * (i))
      {
        long t = *(i + 1);
        *(i + 1) = *(i);
        *(i) = t;
      }
  }
}

void bubble_c(long *data, long *last)
{
  long *start;
  for (; last > data; last--)
  {
    for ( start = data; start < last; start++)
      if (*(start + 1) < *start)
      {
        long t = *(start + 1);
        *(start + 1) = *(start);
        *start = t;
      }
  }
}

void bubble_d(long *data, long *last)
{
  long *start;
  long *max;
  for (; last > data; last--)
  {

    max = data;
    for ( start = data+1; start <= last; start++)
      if (*start > *max)
      {
        max = start;
      }
      if (max != last) {
        long t = *last;
        *last = *max;
        *max= t;
      }
  }
}

void print_arr(long *data, long count)
{
  for (int i = 0; i < count; i++)
  {
    printf("%ld ", data[i]);
  }
  printf("\n");

}

int main()
{
  long arr[10] = { 9, 8, 7, 6, 5, 4, 3, 2, 1};
  bubble_d(arr, arr + 9);
  print_arr(arr, 10);

}
