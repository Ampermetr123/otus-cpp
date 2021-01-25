
#include "workers_pool.h"

WorkersPool::WorkersPool(int num_threads) : threads(num_threads) {

    flag_abort = (num_threads > 0) ? false : true;
    flag_finish = false;

    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(&WorkersPool::worker, this, i);
    }
}

WorkersPool::~WorkersPool() {
    finish();
   // abort();
}


void WorkersPool::make_job_async(const job_func_t& task) {
    if (flag_abort) {
        return;
    }
    std::lock_guard<std::mutex> guard(mutex_jobs);
    jobs.push(task);
    condition_jobs.notify_one();
}


void WorkersPool::worker(int thread_number) {
    job_func_t func = nullptr;
    while (true)
    {
        {
            std::unique_lock<std::mutex> lck{ mutex_jobs };
            while (!flag_abort && !flag_finish && jobs.empty()) {
                condition_jobs.wait(lck);
            }
            if (flag_abort) {
                break;
            }
            if (flag_finish && jobs.empty()) {
                break;
            }
            
            if (jobs.empty()){
                continue;
            }
            
            func = jobs.front();
            jobs.pop();
        }

        if (func) {
            func(thread_number);
        }
    }
}

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
