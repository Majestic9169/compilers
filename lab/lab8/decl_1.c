long a, b;
float c;
long Catalan[50];
int i, j, n;

struct matrix {
  int row, col;
  double A[100][100];
} M;

a = 10;
b = a;
c = a + b;

if (c > a) {
  a = a + 5;
  b = b + 5;
}

if (c > 0 && c < 100 || a != b) {
  a = a + 5;
  b = b + 5;
} else {
  a = a - 5;
  b = b - 5;
}
