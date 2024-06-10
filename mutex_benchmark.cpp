#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <mutex>
#include <list>

struct my_data
{
    int value;
};

std::atomic<bool> running(true);
std::list<my_data> data_list;
std::mutex list_mutex;

void reader(std::atomic<int> &read_count, std::atomic<long long> &read_time)
{
    while (running)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::unique_lock<std::mutex> lock(list_mutex);
        for (auto &data : data_list)
        {
            // Simulate some read work
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            read_count++;
        }
        auto end = std::chrono::high_resolution_clock::now();
        read_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
}

void writer(std::atomic<int> &write_count, std::atomic<long long> &write_time)
{
    while (running)
    {
        auto start = std::chrono::high_resolution_clock::now();
        my_data new_data;
        new_data.value = rand() % 100;

        {
            std::unique_lock<std::mutex> lock(list_mutex);
            data_list.push_back(new_data);
        }

        auto end = std::chrono::high_resolution_clock::now();
        write_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulate some write work
        write_count++;
    }
}

void benchmark(int reader_threads_count, int writer_threads_count)
{
    std::atomic<int> read_count(0);
    std::atomic<int> write_count(0);
    std::atomic<long long> read_time(0);
    std::atomic<long long> write_time(0);

    std::vector<std::thread> readers;
    std::vector<std::thread> writers;

    for (int i = 0; i < reader_threads_count; ++i)
    {
        readers.emplace_back(reader, std::ref(read_count), std::ref(read_time));
    }

    for (int i = 0; i < writer_threads_count; ++i)
    {
        writers.emplace_back(writer, std::ref(write_count), std::ref(write_time));
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
