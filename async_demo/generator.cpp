#include "generator.h"

#include <random>

/****  Generator implementation  ******/

Generator::Iterator Generator::begin() {
    return Iterator(*this, false);
}

Generator::Iterator Generator::end() {
    return Generator::Iterator(*this, true);
}

/****  Generator::Iterator implementation  ******/

Generator::Iterator::Iterator(Generator& Generator, bool end)
    :gen(Generator), flag_end(end) {
    if (!flag_end)
        flag_end = !gen.get(val);
}

bool Generator::Iterator::operator!=(const Iterator& rv) {
    return !(flag_end == true && rv.flag_end == true);
}

Generator::Iterator& Generator::Iterator::operator++() {
    flag_end = !gen.get(val);
    return *this;
}

std::string Generator::Iterator::operator*() {
    return val;
}


/****  RandomGenerator implementation  ******/

RandomBraceGenerator::RandomBraceGenerator(std::string prefix, int len, int max_braces_depth) :
    Generator(),
    prefix(prefix),
    length(len),
    max_braces_depth(max_braces_depth),
    radnom_distib(1, 100) {
    // std::random_device rd;
    //random_engine = std::mt19937();

}

bool RandomBraceGenerator::get(std::string& val) {
    if (i <= length) {
       
        auto seed = radnom_distib(random_engine);
        if (seed < radnom_distib.max()/5  && braces < max_braces_depth) {
            val = std::string("{");
            braces++;
        }
        else if (seed <= radnom_distib.max()/3 && braces > 0) {
            braces--;
            val = std::string("}");
        }
        else {
            i++;
            val = prefix + std::string("_cmd") + std::to_string(i);
        }
        return true;
    }
    else {
        return false;
    }
}


