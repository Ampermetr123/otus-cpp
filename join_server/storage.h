#pragma once

#include <memory>
#include <string>

#include <exception>
#include "iterable_result.h"


namespace joinstorage {

    using storage_data_t = std::string;
    using storage_index_t = unsigned long;

    class Storage {
    public:
        std::unique_ptr<IterableResult> insert(std::string table, storage_index_t id, storage_data_t data);
        std::unique_ptr<IterableResult> trucate(std::string table);
        std::unique_ptr<IterableResult> intersection(std::string table);
        std::unique_ptr<IterableResult> difference(std::string table);
    };



}