#pragma once

#include <string>
#include <vector>
#include <functional>

namespace mapreduce {
   
    using mapper_func_t = std::function<std::string(const std::string&)>;
    using mapper_result_t = std::vector<std::string>;
    using reducer_func_t = std::function<void(const std::string&)>;
         
    /**
     * @brief MapReduce processor
     *  
     * @param src_file file with data (strings)
     * @param mnum number of mappers
     * @param mapper_func - mapper function
     * @param reducers - vector of functional objects for redeuce processing
     * @throw runtime_error, if src file is empty or not exists
     */
    void map_reduce(std::string src_file, size_t mnum, mapper_func_t& mapper_func, std::vector<reducer_func_t>& reducers);
    
    
}