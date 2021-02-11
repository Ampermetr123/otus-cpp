#pragma once

#include "bulk.h"
#include "workers_pool.h"

#include <functional>
#include <memory>
#include <iostream>
#include <queue>


namespace async {

    class BulkProcessor {
    public:
        BulkProcessor(std::size_t bulk_size, WorkersPool& pool_cout, WorkersPool& pool_file);
        ~BulkProcessor();
        void receive(std::string cmd);

    private:
        void handle_current_bulk();
        void save(std::shared_ptr<Bulk> spBulk, std::string file_suffix);
        WorkersPool& pool_cout;
        WorkersPool& pool_file;
        std::shared_ptr<Bulk> spBulk;
    };


}