import subprocess

subprocess.call(
    'g++-5 -std=gnu++1y -O2 -o out/main.out src/main.cpp', shell=True)
subprocess.call('javac -d out src/PointsOnTheCircleVis.java', shell=True)

for seed in range(1, 10):
    subprocess.call(
        'java -cp out PointsOnTheCircleVis -exec out/main.out -seed {0}'.format(seed), shell=True)
