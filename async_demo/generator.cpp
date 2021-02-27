#include "generator.h"



/**
 * @brief Construct a new Random Brace Generator:: Random Brace Generator object
 * Generated string template is {prefix}{index}, where index = 1,2,3...len
 * or "{" or "}"
 * @param prefix  prefix for generated strings
 * @param len length of generated data
 * @param max_braces_depth maximum braces depth
 */
RandomBraceGenerator::RandomBraceGenerator(std::string prefix, int len, int max_braces_depth) :
    Generator<std::string>(),
    prefix(prefix),
    length(len),
    max_braces_depth(max_braces_depth),
    radnom_distib(1, 100) {
}

/**
 * @brief Retrive a string from generator
 * @param val [out] where to store new generated data
 * @return true if there are more data
 * @return false when store to the val last data
 */

bool RandomBraceGenerator::get(std::string& val) {
    if (i <= length) {
        auto seed = radnom_distib(random_engine);
        if (seed < radnom_distib.max() / 5 && braces < max_braces_depth) {
            val = std::string("{");
            braces++;
        }
        else if (seed <= radnom_distib.max() / 3 && braces > 0) {
            braces--;
            val = std::string("}");
        }
        else {
            i++;
            val = prefix + std::to_string(i);
        }
        return true;
    }
    else {
        return false;
    }
}
