#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

float my_sin(float x) {
  int N = 2;
  float sum;
  float f;
  float x2;

  x -= (int)(x / (2 * M_PI)) * 2 * M_PI;
  sum = x;
  f = x;
  x2 = x * x;
  for (int n = 1; n <= N; n++) {
    f *= -x2 / ((2 * n + 1) * (2 * n));
    sum += f;
  }
  return sum;
}

float sin_(float x) {
  float sum = x, f = x;
  x = x * x;
  f *= -x / 6;
  sum += f;
  f *= -x / 20;
  sum += f;
  return sum;
}

constexpr int z = 1 << 14;
int sin__(int x) {
  int sum = x, f = x;
  x = x * x / z;
  f = x * f / 6 / z;
  sum -= f;
  f = x * f / 20 / z;
  sum += f;
  f = x * f / 42 / z;
  sum -= f;
  return sum;
}

int main() {
  int x = 0;
  for (float i = 0; i < 0.5; i += 0.01) {
    // x += sin(i * M_PI) * z;
    // x += sin__(i * M_PI * z);
    if (true) {
      int a = sin(i * M_PI) * z;
      int b = sin__(i * M_PI * z);
      cerr << i << " " << (int)(i * z * M_PI) << " " << a << " " << b << endl;
      // assert(abs(a - b) < 3);
    }
  }
  cerr << x << endl;
}
