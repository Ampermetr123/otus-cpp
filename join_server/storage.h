#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <set>
#include <exception>
#include "iterable_result.h"


using storage_data_t = std::string;
using storage_index_t = unsigned long;
using storage_result_t = std::unique_ptr<IterableResult>;


struct Record {
    storage_index_t id;
    storage_data_t value;
    bool operator<(const Record& r) const {
        return id < r.id;
    }
    operator std::string() {
        return std::to_string(id) + ' ' + value;
    }
};


struct QueryRecord {
    storage_index_t id;
    const std::string& val1_ref;
    const std::string& val2_ref;
    bool operator<(const Record& r) const {
        return id < r.id;
    }
    operator std::string() {
        return std::to_string(id) + "," + val1_ref + "," + val2_ref;
    }
};


class IStorage {
public:
    virtual storage_result_t insert(const std::string& table, storage_index_t id, const storage_data_t& data) = 0;
    virtual storage_result_t truncate(const std::string& table) = 0;
    virtual storage_result_t intersection() = 0;
    virtual storage_result_t difference() = 0;
    virtual bool checkTableName(const std::string& table_name) const = 0;
};


class Storage : public IStorage {
public:
    Storage(const std::string& table_name_A = "A", const std::string& table_name_B = "B");
    storage_result_t insert(const std::string& table, storage_index_t id, const storage_data_t& data) override;
    storage_result_t truncate(const std::string& table) override;
    storage_result_t intersection() override;
    storage_result_t difference() override;
    bool checkTableName(const std::string& table_name) const override;
private:
    const std::string nameA;
    const std::string nameB;
    std::set<Record> setA;
    std::set<Record> setB;
    std::set<Record>* get_set_ptr(const std::string& name);
    inline static const std::string empty_string = "";
};

