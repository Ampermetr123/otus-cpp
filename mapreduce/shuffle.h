#pragma once
#include <vector>

#include "helpers.h"

/**
     * @brief Shuffle data (one thread) from M vectors to N vecotrs
     * @note the same data go to the same container N
     * @note change the order direction
     * @tparam T type of data
     * @param mcont souce M vectors with sorted data
     * @param rcont destination N vectors with sorted data
     */
template <typename T>
void shuffle(std::vector< std::vector<T> >& mcont, std::vector< std::vector<T> >& rcont) {
    int mcont_size = mcont.size();
    int rcont_size = rcont.size();
    if ((mcont_size == 0) || (rcont_size == 0))
        return;

    auto maxval = maxval_of_ordered_conts(mcont);
    std::vector<T>* pr;
    while (max_size_of_containers(mcont)) {
        pr = less_filled_cont(rcont);
        for (auto& m : mcont) {
            while (m.size() && m.back() == maxval) {
                pr->push_back(std::move(m.back()));
                m.pop_back();
            }
        }
        maxval = maxval_of_ordered_conts(mcont);
    }
}