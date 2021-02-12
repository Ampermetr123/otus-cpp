#include "mapreduce.h"
#include <future>
#include <fstream>
#include <algorithm>
#include <functional>
#include <exception>
#include "helpers.h"
#include "optlog.h"


namespace mapreduce
{
    using mapper_result_t = std::vector<std::string>;

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
        while (maxval != std::string("")) {
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


    void map_reduce(std::string src_file, size_t mnum, mapper_func_t& mapper_func, std::vector<reducer_func_t>& reducers) {
        using optlog::log0;
        using optlog::log1;

        if (reducers.empty() || mnum == 0) {
            throw std::invalid_argument("number of mappers and number of reducers must be greater 0");
        }

        auto bounds = get_file_bounds(src_file, mnum);
        if (bounds.empty() || bounds[0].second == 0) {
            throw std::runtime_error("source file not found or has no data");
        }

        log1 << "--------------- Map Reduce -----------------------\n";
        std::vector<std::future<mapper_result_t>> mappers_futures;
        for (auto p : bounds) {
            mappers_futures.push_back(
                std::async(
                    [&mapper_func, src_file, a = p.first, b = p.second]() {
                        mapper_result_t retval;
                        auto f = std::ifstream(src_file);
                        f.seekg(a);
                        std::string line;
                        while (f.tellg() < b && std::getline(f, line)) {
                            retval.push_back(mapper_func(line));
                        }
                        std::sort(retval.begin(), retval.end());
                        return retval;
                    }
                )
            );
        }
        
        std::vector<mapper_result_t> mappers_rusults;
        for (auto& f : mappers_futures) {
            mappers_rusults.emplace_back(f.get());
        }
        log_containers("mapper result", mappers_rusults);

        // Shuffle
        std::vector<mapper_result_t> reducers_inputs(reducers.size());
        shuffle(mappers_rusults, reducers_inputs);
        log_containers("input to reducer", reducers_inputs);

        std::vector<std::future<void>> reducers_futures;
        for (size_t i = 0; i < reducers.size(); i++) {
            reducers_futures.push_back( std::async (
                [&rfunc = reducers[i], &data = reducers_inputs[i]]() {
                    for (auto& s : data) {
                    rfunc(s);
                }
             })
            );
        }
        for (auto& f : reducers_futures) {
            f.wait();
        }
        log1 << "Done, results are in reducer's objects" << std::endl;
    }
}