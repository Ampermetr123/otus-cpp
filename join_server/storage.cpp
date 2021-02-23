#include <thread>
#include <chrono>
#include <algorithm>

#include "storage.h"
using namespace std::string_literals;

Storage::Storage(std::string table_name_A, std::string table_name_B) :
    nameA(table_name_A), nameB(table_name_B) {
}

bool Storage::checkTableName(std::string table_name) {
    return (table_name == nameA) || (table_name == nameB);
}

std::set<Record>* Storage::get_set_ptr(std::string& name) {
    if (name == nameA) {
        return &setA;
    }
    else if (name == nameB) {
        return &setB;
    }
    return nullptr;
}

storage_result_t Storage::insert(std::string table, storage_index_t id, storage_data_t data) {
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

storage_result_t Storage::truncate(std::string table) {
    if (auto set_ptr = get_set_ptr(table)) {
        set_ptr->clear();
        return storage_result_t(new StringResult("ОК"));
    }
    return storage_result_t(new StringResult("ERR invalid table", false));
}

storage_result_t Storage::intersection() {
    std::vector<ViewRecord> r;
    r.reserve(std::min(setA.size(), setB.size()));
    auto itA = setA.begin();
    auto itB = setB.begin();
    while (itA != setA.end() && itB != setB.end()) {
        if (*itA < *itB) {
            ++itA;
        }
        else {
            if (!(*itB < *itA)) {
                r.push_back(ViewRecord{ itA->id, itA->value, itB->value });
            }
            ++itB;
        }
    }
    return storage_result_t(new ContResult<std::vector<ViewRecord>>(std::move(r)));
}


//  while (first1 != last1) {
//     if (first2 == last2) return std::copy(first1, last1, d_first);
//     if (*first1 < *first2) {
//         *d_first++ = *first1++;
//     } else {
//         if (*first2 < *first1) {
//             *d_first++ = *first2;
//         } else {
//             ++first1;
//         }
//         ++first2;
//     }
// }
storage_result_t Storage::difference() {
    std::vector<ViewRecord> r;
    r.reserve(std::min(setA.size(), setB.size()));
    auto itA = setA.begin();
    auto itB = setB.begin();

    if (itB == setB.end()) {
        std::transform(itA, setA.end(), std::back_inserter(r),
            [](const Record& rec) { return ViewRecord{ rec.id, rec.value, Storage::empty_string }; }
        );
        return storage_result_t(new ContResult<std::vector<ViewRecord>>(std::move(r)));
    }

    while (itA != setA.end()) {
        if (*itA < *itB) {
            r.push_back(ViewRecord{ itA->id, itA->value, Storage::empty_string });
            ++itA;
        }
        else {
            if (*itB < *itA) {
                r.push_back(ViewRecord{ itB->id, Storage::empty_string, itB->value });
            }
            else {
                ++itA;
            }
            ++itB;
        }
    }
    std::transform(itB, setB.end(), std::back_inserter(r),
        [](const Record& rec) { return ViewRecord{ rec.id, Storage::empty_string, rec.value }; }
    );
    return storage_result_t(new ContResult<std::vector<ViewRecord>>(std::move(r)));
}

