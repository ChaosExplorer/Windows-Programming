#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int a;
  char b;
  float c;
  int d[10];
  double* p;
} T;

int main(int argc, char **argv)
{
  T t1, t2;
  t1.a = 1;
  t1.b = 'b';
  t1.c = 0.1;
  t1.p = (double*)malloc(sizeof(double));
  *t1.p = 2.2;

  for(int i=0; i< 10; ++i)
  {
     t1.d[i] = i;
  }

  t2 = t1;
  printf("%d, %c, %f\n", t2.a, t2.b, t2.c);

  printf("t1:%p, t2:%p\n", &t1, &t2);
  printf("t1.d[2]:%p, t2.d[2]:%p\n", t1.d+2, t2.d+2);
  printf("t1.p:%p, t2.p:%p", t1.p, t2.p);
  for(int i=0; i< 10; ++i)
  {
     printf("%d\n",t2.d[i]);
  }
}
