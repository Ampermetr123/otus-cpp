/**
 * @file generator.h
 * @author Sergey Simonov
 * @brief Helper classes to generate and iterate through data
 *
 */
#pragma once

#include <string>
#include <random>

 /**
  * @brief data of type T Generator interface
 */
template <class T>
class Generator {
public:
    Generator() = default;
    class Iterator;
    Iterator begin();
    Iterator end();
    /**
     * @brief Retrive string from generator
     * @param val [out] where to store new generated data
     * @return true if there are more data
     * @return false when store to the val last data
     */
    virtual bool get(T& val) = 0;

    class Iterator {
        Generator& gen;
        T val;
        bool flag_end;
    public:
        Iterator(Generator& generator, bool _end = false);
        T operator*();
        Iterator& operator++();
        bool operator!=(const Iterator& rv);
    };
};

template <class T>
typename Generator<T>::Iterator Generator<T>::begin() {
    return Iterator(*this, false);
}

template <class T>
typename Generator<T>::Iterator Generator<T>::end() {
    return Generator::Iterator(*this, true);
}

template<class T>
Generator<T>::Iterator::Iterator(Generator& generator, bool end) 
:gen(generator), flag_end(end) {
    if (!flag_end)
        flag_end = !gen.get(val);
}

template <class T>
bool Generator<T>::Iterator::operator!=(const Iterator& rv) {
    return !(flag_end == true && rv.flag_end == true);
}

template <class T>
typename Generator<T>::Iterator& Generator<T>::Iterator::operator++() {
    flag_end = !gen.get(val);
    return *this;
}

template <class T>
T Generator<T>::Iterator::operator*() {
    return val;
}

/**
 * @brief Generator for any iterated data
 * @tparam Iter Iterator, that support *, ++ and != operation
 */
template <class Iter>
class IterGenerator :public Generator<typename Iter::value_type> {
public:
    IterGenerator(const Iter& begin, const Iter& end) :it_begin(begin), it_end(end) {}
    bool get(typename Iter::value_type &val) override;
private:
    Iter it_begin;
    Iter it_end;
};

template<class Iter>
bool IterGenerator<Iter>::get(typename Iter::value_type &val) {
    if (it_begin != it_end) {
        val = *it_begin;
        ++it_begin;
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Generator for random commands with braces
 * Generated string template is {prefix}{index}, where index = 1,2,3...len
 * or "{" or "}"
 */

class RandomBraceGenerator :public Generator<std::string> {
public:
    RandomBraceGenerator(std::string prefix, int len, int max_braces_depth = 2);
    bool get(std::string& val) override;
private:
    std::string prefix;
    int length;
    int max_braces_depth;
    int i = 0; /// counter of generated data
    int braces = 0; /// current braces level
    std::mt19937 random_engine;
    std::uniform_int_distribution<int> radnom_distib;

};