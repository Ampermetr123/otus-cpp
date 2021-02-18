#pragma once

#include <functional>

class ResultIterator {
    std::function<const std::string& ()> deref_func;
    std::function<bool()> inc_func;
    bool flag_end;
public:

    ResultIterator() :flag_end(true) {
    }

    ResultIterator(std::function<std::string()> deref_func, std::function<bool()> inc_func)
        : deref_func(deref_func), inc_func(inc_func), flag_end(false) {
    }

    const std::string& operator*() {
        if (flag_end) {
            throw std::out_of_range("Can't dereference end StorageResultIterator");
        }
        if (!deref_func) {
            throw std::runtime_error("Invalid dereference function in StorageResultIterator");
        }
        return deref_func();
    }

    ResultIterator& operator++() {
        flag_end = inc_func ? true : !inc_func();
        return *this;
    }

    bool operator!=(const ResultIterator& rv) {
        return flag_end != rv.flag_end;
    };
};


class IterableResult {
    bool success;
public:
    IterableResult(bool success = false) : success(success) {};
    virtual ResultIterator begin() = 0;
    virtual ResultIterator end() = 0;
    operator bool() {
        return success;
    };
    virtual size_t size() = 0;
};



class StringResult : public IterableResult {
    std::string str;
public:
    StringResult(std::string str, bool success = true) : IterableResult(success), str(str) {};
    ResultIterator begin() override {
        auto deref = [&val = str]() {
            return val;
        };
        return ResultIterator(deref, nullptr);
    };
    ResultIterator end() override {
        return ResultIterator();
    };
    virtual size_t size() override {
        return 0;
    };
};

