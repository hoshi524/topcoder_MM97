#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

class Timer {
 public:
  void restart();
  double getElapsed();

  Timer();

 private:
  static double rdtsc_per_sec_inv;

  double getTimeOfDay();
  unsigned long long int getCycle();

  double start_time;
  unsigned long long int start_clock;
};
double Timer::rdtsc_per_sec_inv = -1;

inline double Timer::getElapsed() {
  if (rdtsc_per_sec_inv != -1)
    return (double)(getCycle() - start_clock) * rdtsc_per_sec_inv;

  const double RDTSC_MEASUREMENT_INTERVAL = 0.1;
  double res = getTimeOfDay() - start_time;
  if (res <= RDTSC_MEASUREMENT_INTERVAL) return res;

  rdtsc_per_sec_inv = 1.0 / (getCycle() - start_clock);
  rdtsc_per_sec_inv *= getTimeOfDay() - start_time;
  return getElapsed();
}

inline void Timer::restart() {
  start_time = getTimeOfDay();
  start_clock = getCycle();
}

Timer::Timer() { restart(); }

inline double Timer::getTimeOfDay() {
  timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec * 0.000001;
}

inline unsigned long long int Timer::getCycle() {
  unsigned int low, high;
  __asm__ volatile("rdtsc" : "=a"(low), "=d"(high));
  return ((unsigned long long int)low) | ((unsigned long long int)high << 32);
}

Timer timer;

inline unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

constexpr float TIME_LIMIT = 1.9;
constexpr int N = 1 << 8;
constexpr int Z = 1 << 16;

uint8_t n, x[N], p[N], best[N], m[N][N >> 1];
int D[N];

inline int dist(int a, int b) { return D[abs(a - b)]; }

int calc(int a, int b) {
  int v = 0;
  for (int i = 0; m[a][i] < 0xff; ++i)
    if (m[a][i] != b) v += dist(x[b], x[m[a][i]]) - dist(x[a], x[m[a][i]]);
  return v;
}

class PointsOnTheCircle {
 public:
  vector<int> permute(vector<int> matrix) {
    {  // init
      n = (int)sqrt(matrix.size());
      memset(m, 0xff, sizeof(m));
      for (int i = 0; i < n; ++i) {
        x[i] = i;
        p[i] = i;
        D[i] = sin(M_PI * i / n) * Z;
        for (int j = 0; j < n; ++j)
          if (matrix[i * n + j])
            for (int k = 0;; ++k)
              if (m[i][k] == 0xff) {
                m[i][k] = j;
                break;
              }
      }
    }
    {  // solve
      constexpr int LOG_SIZE = 1 << 10;
      int log_[LOG_SIZE];
      int bs = INT_MAX, cs = 0;
      int dmin = 5;
      int dmax = n / 3;
      while (true) {
        float time = TIME_LIMIT - timer.getElapsed();
        if (time < 0) break;
        for (int i = 0; i < LOG_SIZE; ++i) {
          log_[i] = -Z * log((i + 0.5) / LOG_SIZE) * time / TIME_LIMIT;
        }
        int d = dmin + (dmax - dmin) * time;
        for (int i = 0; i < 0xffff; ++i) {
          int a = get_random() % n;
          int t = x[a] - d;
          if (t < 0) t += n;
          int b = p[(t + get_random() % (d * 2 + 1)) % n];
          if (a == b) continue;
          t = calc(a, b) + calc(b, a);
          if (t <= log_[get_random() & (LOG_SIZE - 1)]) {
            swap(x[a], x[b]);
            p[x[a]] = a;
            p[x[b]] = b;
            cs += t;
            if (bs > cs) {
              bs = cs;
              memcpy(best, x, sizeof(x));
            }
          }
        }
        // for (int i = 0; i < n; ++i) assert(abs(p[i] - calc(i)) < 0.001);
      }
    }
    {  // output
      vector<int> ret(n);
      for (int i = 0; i < n; ++i) ret[best[i]] = i;
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
