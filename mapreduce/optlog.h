/**
 * @file optlog.h
 * @author  Sergey Simonov
 * @brief   Simple stream (<<) looger with filter by verbose and message level
 *
 */
#pragma once
#include <iostream>

namespace optlog {

    // Compile time logger creating
    template<std::ostream* stream, int verbLevel, int MsgLevel>
    struct ConstOptLog {
        template <typename ValType>
        const ConstOptLog& operator<<(const ValType& val) const {
            if constexpr (verbLevel >= MsgLevel) {
                (*stream) << val;
            }
            return *this;
        }
        // to support << std::endl
        const ConstOptLog& operator<<(std::basic_ostream<char>& (*manip)(std::basic_ostream<char>&)) const {
            if  constexpr (verbLevel >= MsgLevel) {
                return operator<< <>(manip);
            }
            else {
                return *this;
            }
        }
    };


    // Run time logger creating
    class OptLog {
    private:
        std::ostream*stream;
        int verbLevel;
        int msgLevel;
    public:
        OptLog(std::ostream& stream, int verbLevel, int msgLevel) :
            stream(&stream), verbLevel(verbLevel), msgLevel(msgLevel) {
        };
        
        OptLog& operator=(OptLog&& rv){
            stream = rv.stream;
            verbLevel = rv.verbLevel;
            msgLevel = msgLevel;
            return *this;
        };

        template <typename ValType>
        const OptLog& operator<<(const ValType& val) const {
            if (verbLevel >= msgLevel) {
                *stream << val;
            }
            return *this;
        }
        // to support << std::endl
        const OptLog& operator<<(std::basic_ostream<char>& (*manip)(std::basic_ostream<char>&)) const {
            if (verbLevel >= msgLevel) {
                return operator<< <>(manip);
            }
            else {
                return *this;
            }
        }
    };
    
    [[maybe_unused]] extern OptLog log0;
    [[maybe_unused]] extern OptLog log1;
    [[maybe_unused]] extern OptLog log2;

} // namespace optlog