/**
 * @file workers_pool.cpp
 * @author Sergey Simonov
 * @brief Simple thread pool implementation
 */
#include "workers_pool.h"

namespace async {

    /**
     * @brief Construct a new Workers Pool:: Workers Pool object
     * @param num_threads number of workers in pool
     */
    WorkersPool::WorkersPool(int num_threads) : threads(num_threads) {
        flag_abort = (num_threads > 0) ? false : true;
        flag_finish = false;
        for (int i = 0; i < num_threads; i++) {
            threads.emplace_back(&WorkersPool::worker, this, i);
        }
    }

    /**
     * @brief Destroy the Workers Pool:: Workers Pool object
     * @note Is blocking to finish all jobs in queue to workers
     */
    WorkersPool::~WorkersPool() {
        finish();   // finish all jobs in queue and exit
        // abort(); // finish current job and exit 
    }

    /**
     * @brief Add job to pool
     * @param job lamba or std::function with interface void func(int worker_id)
     */
    void WorkersPool::make_job_async(const job_func_t& job) {
        if (flag_abort) {
            return;
        }
        std::lock_guard<std::mutex> guard(mutex_jobs);
        jobs.push(job);
        condition_jobs.notify_one();
    }

    /**
     * @brief Thread worker
     * @param worker_number number of worker
     */
    void WorkersPool::worker(int worker_number) {
        job_func_t func = nullptr;
        while (true)
        {
            {
                std::unique_lock<std::mutex> lck{ mutex_jobs };
                while (!flag_abort && !flag_finish && jobs.empty()) {
                    condition_jobs.wait(lck);
                }
                if (flag_abort || (flag_finish && jobs.empty())) {
                    break;
                }
                if (jobs.empty()) {
                    continue;
                }
                func = jobs.front();
                jobs.pop();
            }

            if (func) {
                func(worker_number);
            }
        }
    }


    /**
     * @brief Block until all jobs done and stop workers
     */
    void WorkersPool::finish() {
        if (flag_abort || flag_finish) {
            return;
        }
        flag_finish = true;
        condition_jobs.notify_all();

        for (auto& thread : threads) {
            if (thread.joinable())
                thread.join();
        }
    }

    /**
     * @brief Block until curent jobs done and stop workers
     */
    void WorkersPool::abort() {
        if (!flag_abort) {
            flag_abort = true;
            condition_jobs.notify_all();
            for (auto& thread : threads) {
                if (thread.joinable())
                    thread.join();
            }
        }
    }

}