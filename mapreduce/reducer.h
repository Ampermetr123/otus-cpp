#pragma once

#include <string>
#include <fstream>


/**
 * @brief Reducer for solving "Find min prefix" problem
 * Looks for repeats in incoming data
  * And checks for repeats of sring with size >= length_factor 
 * 
 * 
 * Also do ouput of incoming strings to file
 * 
 * @note
 * We could jast skip all incoming string after first repeat found and don't calculate maxumum
 * But Otus wants we print data to ouput 
 */
class CheckRepeatsReducer {
private:
    std::string prev;
    long current_repeats = 0;
    bool have_repeats = false;
    size_t max_line_length = 0;
    std::string file_to_save;
    std::ofstream file;
    size_t length_factor=0;

public:
    CheckRepeatsReducer(std::string file_to_save) :file_to_save(file_to_save) {
        if (file_to_save.size()) {
            file.open(file_to_save);
        }
    }
    
    CheckRepeatsReducer(CheckRepeatsReducer &&rv) {
        reset(rv.length_factor);
        file_to_save = std::move(rv.file_to_save);
        if (file_to_save.size()) {
            file.open(file_to_save);
        }
    }
    
    ~CheckRepeatsReducer() { 
        reset(0); 
    }
    
    void reset(size_t length) {
        if (prev.size() && file.is_open()) {
                file << prev << " - " << current_repeats << " times\n\n";
        }
        prev = "";
        current_repeats = 0;
        max_line_length = 0;
        have_repeats = false;
        length_factor = length;
    }

    void operator()(std::string s) {
        if (s.empty()) {
            return;
        }
        
        if (s == prev) {
            current_repeats++;
        }
        else {
            if (prev.size() && file.is_open()) {
                file << prev << " - " << current_repeats << " times \n";
            }
            current_repeats = 1;
        }

        if (current_repeats > 1 && s.size()>=length_factor) {
            have_repeats = true;
        }
        
        if (s.length() > max_line_length) {
            max_line_length = s.length();
        }
        prev = s;
    }

    bool repeats_found() {
        return have_repeats;
    }
    
    size_t max_length() {
        return max_line_length;
    }
};