#include "iterable_result.h"


/*--------  ResultIterator ----------------------------------------*/

/**
 * @brief Construct a new Result Iterator like END iterator
 */
ResultIterator::ResultIterator()
    :deref_func(nullptr), inc_func(nullptr), flag_end(true) {
}

/**
 * @brief Construct a new Result Iterator
 * @param deref_func callback func for dereferencing
 * @param inc_func callback func for ++ operation
 */
ResultIterator::ResultIterator(deref_func_t deref_func, inc_func_t inc_func)
    : deref_func(deref_func), inc_func(inc_func), flag_end(false) {
}

bool ResultIterator::operator!=(const ResultIterator& rv) {
    return flag_end != rv.flag_end;
}

/**
 * @brief Iterator increment
 */
ResultIterator& ResultIterator::operator++() {
    flag_end = inc_func ? !inc_func() : true;
    return *this;
}

/**
 * @brief Dereferencing
 * @return const std::string
 */
const std::string ResultIterator::operator*() {
    if (flag_end) {
        throw std::out_of_range("Can't dereference end ResultIterator");
    }
    if (!deref_func) {
        throw std::runtime_error("Invalid dereference function in ResultIterator");
    }
    return deref_func();
}


/*--------  StringResult ----------------------------------------*/

StringResult::StringResult(std::string str, bool success)
    : IterableResult(success), str(str) {
}

ResultIterator StringResult::begin() {
    auto deref = [&s = str]() -> const std::string {
        return s;
    };
    return ResultIterator(deref, nullptr);
}

size_t StringResult::size() {
    return 1;
}








