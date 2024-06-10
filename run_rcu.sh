g++ -std=c++17 -o rcu_benchmark rcu_benchmark5.cpp -I/opt/homebrew/opt/userspace-rcu/include -L/opt/homebrew/opt/userspace-rcu/lib -lurcu-memb -lpthread

./rcu_benchmark