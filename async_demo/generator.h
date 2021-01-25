#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#pragma once

#include <string>

class Generator {
public:
    Generator() = default;
    class Iterator;
    Iterator begin();
    Iterator end();
    virtual bool get(std::string& val) = 0;
    //inline static const int end_iterator = 0;

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
private:

};

template <class Iter>
class IterGenerator :public Generator {
public:
    IterGenerator(const Iter& begin, const Iter& end) :it_begin(begin), it_end(end) {}
    bool get(std::string& val) override {
        if (it_begin != it_end) {
            val = *it_begin;
            ++it_begin;
            return true;
        }
        else {
            return false;
        }
    }
private:
    Iter it_begin;
    Iter it_end;
};

#include <random>

class RandomBraceGenerator :public Generator {
public:
    RandomBraceGenerator(std::string prefix, int len, int max_braces_depth = 2);
    bool get(std::string& val) override;
private:
    std::string prefix;
    int length;
    int max_braces_depth;
    int i = 0;
    int braces = 0;
    std::mt19937 random_engine;
    std::uniform_int_distribution<int> radnom_distib;

};

#endif // __GENERATOR_H__