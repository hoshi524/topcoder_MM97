import subprocess
import queue
import threading


subprocess.call(
    'g++ --std=c++0x -W -Wall -Wno-sign-compare -O2 -s -pipe -mmmx -msse -msse2 -msse3 -o out/main.out src/main.cpp', shell=True)
subprocess.call(
    'g++ --std=c++0x -W -Wall -Wno-sign-compare -O2 -s -pipe -mmmx -msse -msse2 -msse3 -o out/test.out src/test.cpp', shell=True)
subprocess.call('javac -d out src/PointsOnTheCircleVis.java', shell=True)
MAIN = './out/main.out'
TEST = './out/test.out'


def solve(command, seed):
    score = float(subprocess.check_output(
        'java -cp out PointsOnTheCircleVis -exec {0} -seed {1}'.format(command, seed), shell=True))
    return score


class State:
    main = 0
    test = 0
    lock = threading.Lock()

    def add(self, s, a, b):
        with self.lock:
            if abs(a - b) > 0.01:
                if a < b:
                    self.main += 1
                else:
                    self.test += 1
            print('{}\t{:8.3f}\t{:8.3f}\t{:d}\t{:d}'.format(
                s, a, b, self.main, self.test))


scores = State()
q = queue.Queue()


def worker():
    while True:
        seed = q.get()
        if seed is None:
            break
        a = solve(MAIN, seed)
        b = solve(TEST, seed)
        scores.add(seed, a, b)
        q.task_done()


num_worker_threads = 2
threads = []
for i in range(num_worker_threads):
    t = threading.Thread(target=worker)
    t.start()
    threads.append(t)


N = 5000
for seed in range(1, N):
    q.put(seed)

# block until all tasks are done
q.join()

# stop workers
for i in range(num_worker_threads):
    q.put(None)
for t in threads:
    t.join()
