#include "wildcard_impl.h"

#include <clocale>

namespace wildcard_impl {
    /** Convert multy-byte (utf-8) string to wide character string (utf-16)
     */
    std::wstring to_wide_string(std::u8string str) {
        std::wstring wstr(str.size(), L'_');
        [[maybe_unused]] static char* loc = setlocale(LC_ALL, "");
        const char* buf = reinterpret_cast<char*>(str.data());
        wchar_t* wbuf = wstr.data();
        int i = -1;
        int n = 0;
        do {
            i++;
            n = mbtowc(wbuf + i, buf, MB_CUR_MAX);
            buf += n;
        } while (n > 0);

        wstr.resize(i);
        return wstr;
    }

}