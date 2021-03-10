#pragma once
#include <string>
#include <vector>

namespace wildcard_impl
{
    /** Convert multy-byte (utf-8) string to wide character string (utf-16) */
    std::wstring to_wide_string(std::u8string str);

    template<class CharT>
    struct Wildcard_traits {
        static CharT dot();
        static CharT quest();
        static CharT asterix();
        static CharT slash();
        static CharT dot_char();
    };

    template<>
    struct Wildcard_traits<char> {
        static char dot() { return '.'; }
        static char quest() { return '?'; }
        static char asterix() { return '*'; }
        static char slash() { return '\\'; }
        static char sep() { return '|'; }
    };

    template<>
    struct Wildcard_traits<wchar_t> {
        static wchar_t dot() { return L'.'; }
        static wchar_t quest() { return L'?'; }
        static wchar_t asterix() { return L'*'; }
        static wchar_t slash() { return L'\\'; }
        static wchar_t sep() { return L'|'; }
    };


    /**
     * @brief Crete string with regex by wildcard mask
     * @tparam CharT: char (string) or wchar_t (wstring) or char8_t (u8string - utf8)
     * @param masks each mask is wildcard with * and ? symbols for searching. Example: "fil?.txt", "*.txt", "file.txt".  
     * @return std::basic_string<CharT> regular expression
     */
    template<class CharT>
    std::basic_string<CharT> make_expression(std::vector <std::basic_string<CharT>> masks) {
        using wt = Wildcard_traits<CharT>;
        std::basic_string<CharT> umask; 
        for (auto& mask : masks) {
            if (umask.size()) {
                umask += wt::sep();
            }
            // . -> /.
            auto pos = mask.find_first_of(wt::dot());
            while (pos != std::string::npos) {
                mask.insert(pos, 1, wt::slash());
                pos = mask.find_first_of(wt::dot(), pos + 2);
            }
            // ? -> .
            pos = mask.find_first_of(wt::quest());
            while (pos != std::string::npos) {
                mask.at(pos) = wt::dot();
                pos = mask.find_first_of(wt::quest(), pos + 1);
            }
            // * -> .*
            pos = mask.find_first_of(wt::asterix());
            while (pos != std::string::npos) {
                mask.insert(pos, 1, wt::dot());
                pos = mask.find_first_of(wt::asterix(), pos + 2);
            }
            umask += mask;
        }
        return umask;
    }
} // wildcard_impl