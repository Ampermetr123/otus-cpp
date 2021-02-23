#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <set>


/**
 * @brief Iterator for IterableReuslt
 * Intended to support range for
 */
class ResultIterator {
    using deref_func_t = std::function<const std::string()>;
    using inc_func_t = std::function<bool()>;
public:
    ResultIterator();
    ResultIterator(deref_func_t deref_func, inc_func_t inc_func);
    ResultIterator& operator=(const ResultIterator&) = delete;
    ResultIterator(ResultIterator&) = delete;
    const std::string operator*();
    ResultIterator& operator++();
    bool operator!=(const ResultIterator& rv);
private:
    deref_func_t deref_func;
    inc_func_t inc_func;
    bool flag_end;
};


/**
 * @brief Base for IterableResult - the strings, that could be forward iterated
 * IterableResult also have logic marker success or false
 */
class IterableResult {
    bool success;
public:
    IterableResult(bool success = false) : success(success) {};
    virtual ResultIterator begin() = 0;
    virtual ResultIterator end() { return ResultIterator(); };
    operator bool() const { return success; };
    virtual size_t size() = 0;
};


/**
 * @brief IterableResult with just one string
 */
class StringResult : public IterableResult {
    std::string str;
public:
    StringResult(std::string str, bool success = true);
    ResultIterator begin() override;
    virtual size_t size() override;
};

/**
 * @brief IterableResult with container of string
  * @tparam T_Containter set<string>, vector<string> etc..
 */
template <typename T_Containter,
    class C = std::enable_if<std::is_convertible<typename T_Containter::value_type, std::string>::value, void >::type>
class ContResult : public IterableResult {
    T_Containter cont;
public:
    ContResult(T_Containter cont_of_str, bool success = true)
        : IterableResult(success), cont(std::move(cont_of_str)) {
    };

    ResultIterator begin() override {
        auto iter = std::make_shared<typename T_Containter::iterator>(cont.begin());
        auto deref = [it = iter]() mutable -> const std::string {
            auto r = *(*it);
            std::string s = r;
            return s;
        };
        auto increment = [end = cont.end(), it = iter]() {
            return (++(*it) != end);
        };
        return ResultIterator(deref, increment);
    };

    virtual size_t size() override {
        return cont.size();
    };

};
