#pragma once

#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include "optlog.h"


inline std::vector<std::pair<std::streampos, std::streampos> >
get_file_bounds(std::string file_name, int n)     {
    std::vector<std::pair<std::streampos, std::streampos>> v;
    auto f = std::ifstream(file_name);
    f.seekg(0, std::ios::end);
    int file_sz = f.tellg();
    if (file_sz <= 0 || f.bad()) {
        return v;
    }
    int page_sz = file_sz / n;

    f.seekg(0, std::ios::beg);
    int beg = 0;
    int end = 0;
    for (int i = 0; i < n; i++) {
        f.seekg(beg + page_sz);
        if (f.eof()) {
            if (beg != file_sz) {
                v.push_back({ beg, file_sz });
            }
            break;
        }
        else {
            end = f.tellg();
            char ch = 0;
            while (ch != '\n' && f.good()) {
                f.read(&ch, 1);
            }
            end = f.tellg();
            if (f.eof()) {
                end = file_sz;
            }
            v.push_back({ beg,end - 1 });
            beg = end;
        }
    }
    return v;
}


template <class TCont>
void log_containers(std::string prefix, std::vector<TCont>& containers, std::string sep = ", ") {
    using optlog::log1;
    int cont_no = 0;
    std::string seporator;
    for (auto& cont : containers) {
        seporator.clear();
        log1 << prefix << " #" << ++cont_no << "\n[";
        for (auto& s : cont) {
            log1 << seporator;
            log1 << s;
            seporator = sep;
        }
        log1 << "]\ntotal " << cont.size() << " elements" << std::endl << std::endl;
    }
}


template <typename T, template <class, class> typename Container>
T maxval_of_ordered_conts(std::vector< Container<T, std::allocator<T> > >& vcont) {
    std::string retval;
    for (auto& cont : vcont) {
        if (cont.size() && cont.back() > retval) {
            retval = cont.back();
        }
    }
    return retval;
}


template <typename Container>
Container* less_filled_cont(std::vector<Container>& vcont) {
    assert(vcont.size() > 0);
    auto min_size = vcont[0].size();
    auto* pRet = &vcont[0];
    for (auto& c : vcont) {
        if (c.size() < min_size) {
            min_size = c.size();
            pRet = &c;
        }
    }
    return pRet;
}

