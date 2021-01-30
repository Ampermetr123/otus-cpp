/**
 * @file generator.h
 * @author Sergey Simonov
 * @brief Helper class to generate and iterate through data
 *
 */
#pragma once

#include <string>

 /**
  * @brief String data Generator interface
 */
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
    virtual bool get(std::string& val) = 0;

    class Iterator {
        Generator& gen;
        std::string val;
        bool flag_end;
    public:
        Iterator(Generator& Generator, bool _end = false);
        std::string operator*();
        Iterator& operator++();
        bool operator!=(const Iterator& rv);
    };
};


/**
 * @brief Generator for any iterated data
 * @tparam Iter Iterator, that support *, ++ and != operation
 */
template <class Iter>
class IterGenerator :public Generator {
public:
    IterGenerator(const Iter& begin, const Iter& end) :it_begin(begin), it_end(end) {}
    bool get(std::string& val) override;
private:
    Iter it_begin;
    Iter it_end;
};

template<class Iter>
bool IterGenerator<Iter>::get(std::string& val) {
    if (it_begin != it_end) {
        val = *it_begin;
        ++it_begin;
        return true;
    }
    else {
        return false;
    }
}

#include <random>

/**
 * @brief Generator for random commands with braces
 * Generated string template is {prefix}{index}, where index = 1,2,3...len
 * or "{" or "}"
 */
class RandomBraceGenerator :public Generator {
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
