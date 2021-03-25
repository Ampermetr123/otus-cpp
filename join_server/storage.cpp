#include <thread>
#include <chrono>
#include <algorithm>

#include "storage.h"
using namespace std::string_literals;

Storage::Storage(const std::string& table_name_A, const std::string& table_name_B) :
    nameA(table_name_A), nameB(table_name_B) {
}


bool Storage::checkTableName(const std::string& table_name) const {
    return (table_name == nameA) || (table_name == nameB);
}


std::set<Record>* Storage::get_set_ptr(const std::string& name) {
    if (name == nameA) {
        return &setA;
    }
    else if (name == nameB) {
        return &setB;
    }
    return nullptr;
}


storage_result_t Storage::insert(const std::string& table, storage_index_t id, const storage_data_t& data) {
    if (auto ptr = get_set_ptr(table)) {
        auto r = ptr->insert(Record{ id,data });
        if (r.second) {
            return storage_result_t(new StringResult("OK"));
        }
        else {
            return storage_result_t(
                new StringResult("ERR dublicate"s + std::to_string(id) + "\n", false));
        }
    }
    return storage_result_t(new StringResult("ERR invalid table", false));
}


storage_result_t Storage::truncate(const std::string& table) {
    if (auto set_ptr = get_set_ptr(table)) {
        set_ptr->clear();
        return storage_result_t(new StringResult("ОК"));
    }
    return storage_result_t(new StringResult("ERR invalid table", false));
}


storage_result_t Storage::intersection() {
    std::vector<QueryRecord> r;
    r.reserve(std::min(setA.size(), setB.size()));
    auto itA = setA.begin();
    auto itB = setB.begin();
    while (itA != setA.end() && itB != setB.end()) {
        if (*itA < *itB) {
            ++itA;
        }
        else {
            if (!(*itB < *itA)) {
                r.push_back(QueryRecord{ itA->id, itA->value, itB->value });
            }
            ++itB;
        }
    }
    return storage_result_t(new ContResult<std::vector<QueryRecord>>(std::move(r)));
}


storage_result_t Storage::difference() {
    std::vector<QueryRecord> r;
    r.reserve(std::min(setA.size(), setB.size()));
    auto itA = setA.begin();
    auto itB = setB.begin();

    if (itB == setB.end()) {
        std::transform(itA, setA.end(), std::back_inserter(r),
            [](const Record& rec) { return QueryRecord{ rec.id, rec.value, Storage::empty_string }; }
        );
        return storage_result_t(new ContResult<std::vector<QueryRecord>>(std::move(r)));
    }

    while (itA != setA.end()) {
        if (*itA < *itB) {
            r.push_back(QueryRecord{ itA->id, itA->value, Storage::empty_string });
            ++itA;
        }
        else {
            if (*itB < *itA) {
                r.push_back(QueryRecord{ itB->id, Storage::empty_string, itB->value });
            }
            else {
                ++itA;
            }
            ++itB;
        }
    }
    std::transform(itB, setB.end(), std::back_inserter(r),
        [](const Record& rec) { return QueryRecord{ rec.id, Storage::empty_string, rec.value }; }
    );
    return storage_result_t(new ContResult<std::vector<QueryRecord>>(std::move(r)));
}

