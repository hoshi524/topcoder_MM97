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

int n, x[N];
bool m[N][N];
float pi;

auto dist(int a, int b) {
  if (a > b) swap(a, b);
  return sin((b - a) * pi);
}

auto calc(int t) {
  float v = 0;
  for (int i = 0; i < n; ++i)
    if (m[t][i]) v += dist(x[t], x[i]);
  return v;
}

auto score() {
  float v = 0;
  for (int i = 0; i < n; ++i) v += calc(i);
  return v;
}

class PointsOnTheCircle {
 public:
  vector<int> permute(vector<int> matrix) {
    {  // init
      n = (int)sqrt(matrix.size());
      pi = M_PI / n;
      for (int i = 0; i < n; ++i) {
        x[i] = i;
        for (int j = 0; j < n; ++j) m[i][j] = matrix[i * n + j];
      }
    }
    {  // solve
      constexpr int LOG_SIZE = 1 << 10;
      float log_[LOG_SIZE];
      float p[N];
      for (int i = 0; i < n; ++i) p[i] = calc(i);
      while (true) {
        float time = TIME_LIMIT - timer.getElapsed();
        if (time < 0) break;
        for (int i = 0; i < LOG_SIZE; ++i) {
          log_[i] = -1 * log((i + 0.5) / LOG_SIZE) * time / TIME_LIMIT;
        }
        for (int i = 0; i < 0xffff; ++i) {
          int a = get_random() % n;
          int b = get_random() % n;
          if (a == b) continue;
          swap(x[a], x[b]);
          float va = calc(a);
          float vb = calc(b);
          if (va + vb - p[a] - p[b] > log_[get_random() & (LOG_SIZE - 1)]) {
            swap(x[a], x[b]);
          } else {
            for (int i = 0; i < n; ++i) {
              if (m[i][a]) p[i] += dist(x[a], x[i]) - dist(x[b], x[i]);
              if (m[i][b]) p[i] += dist(x[b], x[i]) - dist(x[a], x[i]);
            }
            p[a] = va;
            p[b] = vb;
          }
        }
        // for (int i = 0; i < n; ++i) assert(abs(p[i] - calc(i)) < 0.001);
      }
    }
    {  // output
      vector<int> ret(n);
      for (int i = 0; i < n; ++i) ret[x[i]] = i;
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
