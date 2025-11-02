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
A[4][5] = A[1][2] - A[2][3] + c * 123;

T[5][5].x = 100.;
T[5][5].d = T[5][5].x;
S.A[25] = (a + b) * (a - c);
S.x = S.A[25];

BC[5].C[4][3].x = 100;
BC[6].C[5][4].A[50] = BC[5].C[4][3].x;
BC[6].C[5][4].A[80] = BC[5].C[4][3].A[50] + 345.;
BC[A[a / b][c / b]].n =
    A[a - 6][BC[6].C[5][4].A[80] / BC[5].C[4][3].x + 1.35792468];
BC[6].C[5][4] = T[7][7];
