long a, b;

float c;

int A[5][10];

struct coll {
  float f;
  double d;
  long x;
  int A[100];
};

struct coll S, T[10][10];

struct bigcoll {
  long n;
  struct coll C[5][5];
} BC[10];

a = 10;
b = a;
c = a + b;
A[1][2] = 2;
A[2][3] = A[1][2] + 5;
