/**
 * @file main.cpp
 * @author Sergey Simonov
 * @brief Analizer for strings to match one or more wildcard patterns (* and ? symbols in string).
 * Based on boost::regex / std::regex. 
 */
#pragma once
#include <string>
#include <regex>
#include <vector>
#include "wildcard_impl.h"


template <class CharT>
class Wildcard {
    std::basic_regex<CharT> regexpr;
public:
    Wildcard(const CharT* mask) :
        Wildcard(std::vector< std::basic_string<CharT>>({ mask })) {
    }

    Wildcard(std::basic_string<CharT> mask) {
        regexpr = std::basic_regex<CharT>(wildcard_impl::make_expression(std::vector< std::basic_string<CharT>>({ mask })));
    }

    Wildcard(std::vector< std::basic_string<CharT> > masks) {
        regexpr = std::basic_regex<CharT>(wildcard_impl::make_expression(masks));
    }

    bool match(const std::basic_string<CharT>& str) const {
        return std::regex_match(str, regexpr);
    }
};


/** Multy-byte string support (utf8) */
template <>
class Wildcard<char8_t> {
   std::wregex regexpr;
   static inline auto init = std::locale::global(std::locale(""));
public:
    Wildcard(const char8_t* mask) :
        Wildcard(std::vector<std::u8string>({ mask })) {
    }

    Wildcard(std::u8string mask) {
        regexpr = std::wregex(
            wildcard_impl::make_expression(std::vector< std::wstring>({ wildcard_impl::to_wide_string(mask) })),
            std::regex::ECMAScript | std::regex::optimize
        );
    }

    Wildcard(std::vector< std::u8string > masks) {
        std::vector<std::wstring> wmasks;
        std::transform(std::begin(masks), std::end(masks), std::back_inserter(wmasks),
            [](const std::u8string& s) { return wildcard_impl::to_wide_string(s);}
        );
        regexpr = std::wregex(wildcard_impl::make_expression(wmasks));
    }

    bool match(const std::u8string& str) const {
        return std::regex_match(wildcard_impl::to_wide_string(str), regexpr);
    }
 
};

