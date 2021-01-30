/**
 * @file workers_pool.h
 * @author Sergey Simonov
 * @brief Simple thread pool declaration
*/

#pragma once
#include <thread>
#include <vector>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <iostream>

namespace async {
    /**
     * @brief Simple thread pool
     * Each worker processes jobs in its owne thread.
     * Job is a function: void f(int id), where id is worker's num
     */
    class WorkersPool {
    public:
        WorkersPool(int num_threads);
        ~WorkersPool();
        using job_func_t = std::function<void(int)>;
        void make_job_async(const job_func_t& job);
        void finish();
        void abort();
    private:
        void worker(int worker_number);
        std::vector<std::thread> threads;
        std::queue<job_func_t> jobs;
        std::mutex mutex_jobs;
        std::condition_variable condition_jobs;
        std::atomic<bool> flag_abort;
        std::atomic<bool> flag_finish;
    };

}