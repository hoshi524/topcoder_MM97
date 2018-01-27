#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

class PointsOnTheCircle {
 public:
  vector<int> permute(vector<int> matrix) {
    int N = (int)sqrt(matrix.size());
    vector<int> ret(N);
    for (int i = 0; i < N; ++i) {
      ret[i] = N - i - 1;
    }
    return ret;
  }
};

// -------8<------- end of solution submitted to the website -------8<-------
template <class T>
void getVector(vector<T>& v) {
  for (int i = 0; i < v.size(); ++i) cin >> v[i];
}

int main() {
  PointsOnTheCircle pc;
  int M;
  cin >> M;
  vector<int> matrix(M);
  getVector(matrix);

  vector<int> ret = pc.permute(matrix);
  cout << ret.size() << endl;
  for (int i = 0; i < (int)ret.size(); ++i) cout << ret[i] << endl;
  cout.flush();
}
