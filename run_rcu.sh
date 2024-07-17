g++ -std=c++17 -o rcu_benchmark rcu_benchmark.cpp -I/opt/homebrew/opt/userspace-rcu/include -L/opt/homebrew/opt/userspace-rcu/lib -lurcu-memb -lpthread

time ./rcu_benchmark
