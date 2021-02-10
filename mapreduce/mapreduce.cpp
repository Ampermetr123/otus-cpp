#include "mapreduce.h"
#include <future>
#include <fstream>
#include <algorithm>
#include <functional>


/** To simplify it uses the only one thread */

namespace mapreduce
{
    std::vector<std::pair<std::streampos, std::streampos> > 
    get_file_bounds(std::string file_name, int n) 
    {
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
                v.push_back({ beg,end-1 });
                beg = end;
            }
        }
        return v;
    }



}