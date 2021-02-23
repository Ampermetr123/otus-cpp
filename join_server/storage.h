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


class IStorage {
public:
    virtual storage_result_t insert(std::string table, storage_index_t id, storage_data_t data) = 0;
    virtual storage_result_t truncate(std::string table) = 0;
    virtual storage_result_t intersection() = 0;
    virtual storage_result_t difference() = 0;
    virtual bool checkTableName(std::string table_name) = 0;
};

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

struct ViewRecord {
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



class Storage : public IStorage {

public:
    Storage(std::string table_name_A = "A", std::string table_name_B = "B");
    storage_result_t insert(std::string table, storage_index_t id, storage_data_t data) override;
    storage_result_t truncate(std::string table) override;
    storage_result_t intersection() override;
    storage_result_t difference() override;
    bool checkTableName(std::string table_name) override;


private:

    std::string nameA;
    std::string nameB;
    std::set<Record> setA;
    std::set<Record> setB;
    std::set<Record>* get_set_ptr(std::string& name);
    inline static const std::string empty_string = "";


};

