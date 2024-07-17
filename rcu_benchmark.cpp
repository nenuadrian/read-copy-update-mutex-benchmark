#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <urcu.h>         // Userspace RCU
#include <urcu/rculist.h> // RCU list

struct my_data
{
    int value;
    struct cds_list_head node; // RCU list node
};

std::atomic<bool> running(true);

void reader(struct cds_list_head *head, std::atomic<int> &read_count, std::atomic<long long> &read_time)
{
    while (running)
    {
        auto start = std::chrono::high_resolution_clock::now();

        urcu_memb_read_lock();
        struct my_data *data;
        cds_list_for_each_entry_rcu(data, head, node)
        {
            // Simulate some read work
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            read_count++;
        }
        urcu_memb_read_unlock();

        auto end = std::chrono::high_resolution_clock::now();
        read_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
}

void writer(struct cds_list_head *head, std::atomic<int> &write_count, std::atomic<long long> &write_time)
{
    while (running)
    {
        auto start = std::chrono::high_resolution_clock::now();

        struct my_data *new_data = new my_data();
        new_data->value = rand() % 100;

        urcu_memb_read_lock();
        cds_list_add_rcu(&new_data->node, head);
        urcu_memb_read_unlock();

        urcu_memb_synchronize_rcu(); // Ensure all readers have seen the previous state

        auto end = std::chrono::high_resolution_clock::now();
        write_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulate some write work
        write_count++;
    }
}

void benchmark(int reader_threads_count, int writer_threads_count)
{
    struct cds_list_head head;
    CDS_INIT_LIST_HEAD(&head);
    std::atomic<int> read_count(0);
    std::atomic<int> write_count(0);
    std::atomic<long long> read_time(0);
    std::atomic<long long> write_time(0);

    std::vector<std::thread> readers;
    std::vector<std::thread> writers;

    for (int i = 0; i < reader_threads_count; ++i)
    {
        readers.emplace_back(reader, &head, std::ref(read_count), std::ref(read_time));
    }

    for (int i = 0; i < writer_threads_count; ++i)
    {
        writers.emplace_back(writer, &head, std::ref(write_count), std::ref(write_time));
    }

    std::this_thread::sleep_for(std::chrono::seconds(10)); // Run the benchmark for 10 seconds
    running = false;

    for (auto &t : readers)
    {
        t.join();
    }

    for (auto &t : writers)
    {
        t.join();
    }

    std::cout << "Read operations: " << read_count.load() << std::endl;
    std::cout << "Write operations: " << write_count.load() << std::endl;
    std::cout << "Total read time: " << read_time.load() << " microseconds" << std::endl;
    std::cout << "Total write time: " << write_time.load() << " microseconds" << std::endl;
    if (read_count.load() > 0)
    {
        std::cout << "Average read time: " << read_time.load() / read_count.load() << " microseconds" << std::endl;
    }
    if (write_count.load() > 0)
    {
        std::cout << "Average write time: " << write_time.load() / write_count.load() << " microseconds" << std::endl;
    }
}

int main()
{
    int reader_threads_count = 8; // Number of reader threads
    int writer_threads_count = 2; // Number of writer threads

    benchmark(reader_threads_count, writer_threads_count);

    return 0;
}
