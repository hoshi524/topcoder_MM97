#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

inline unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

constexpr int N = 1 << 8;

int n, k, x[N], r[N];
bool m[N][N];
float pi;

auto dist(int a, int b) {
  if (a > b) swap(a, b);
  return sin((b - a) * pi);
}

auto calc(int t) {
  int c = 0;
  float v = 0;
  for (int i = 0; i < k; ++i) {
    if (m[t][x[i]]) {
      ++c;
      v += dist(k, i);
    }
  }
  int z = c * (1 << 24) - v * (1 << 16) + (get_random() & 0xff);
  return z;
}

class PointsOnTheCircle {
 public:
  vector<int> permute(vector<int> matrix) {
    {  // init
      n = (int)sqrt(matrix.size());
      pi = M_PI / n;
      for (int i = 0; i < n; ++i) {
        r[i] = i;
        for (int j = 0; j < n; ++j) m[i][j] = matrix[i * n + j];
      }
    }
    {  // solve
      for (k = 0; k < n; ++k) {
        int a = 0, v = INT_MIN;
        for (int i = 0; i + k < n; ++i) {
          int t = calc(r[i]);
          if (v < t) {
            v = t;
            a = i;
          }
        }
        x[k] = r[a];
        r[a] = r[n - k - 1];
      }
    }
    {  // output
      vector<int> ret(n);
      for (int i = 0; i < n; ++i) ret[i] = x[i];
      return ret;
    }
  }
};

// -------8<------- end of solution submitted to the website -------8<-------
int main() {
  PointsOnTheCircle pc;
  int M;
  cin >> M;
  vector<int> matrix(M);
  for (int i = 0; i < M; ++i) cin >> matrix[i];

  vector<int> ret = pc.permute(matrix);
  cout << ret.size() << endl;
  for (int i = 0; i < (int)ret.size(); ++i) cout << ret[i] << endl;
  cout.flush();
}
