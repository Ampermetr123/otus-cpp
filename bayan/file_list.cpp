/**
 * @file file_list.c
 * @author  Sergey Simonov
 * @brief Implementation of function to scan directories and generate list of files according to filters
 */

#include <algorithm>
#include <list>

#include "file_list.h"
#include "wildcard.h"
#include "log.h"


/** Create filter for excluding dirs
 * @param excl_dirs vector of pathes to directories
 */
path_filter_func_t make_filter_exclude_dir(std::vector<fs::path> excl_dirs) {
    return [excl_dirs = std::move(excl_dirs)](const fs::directory_entry& p)
    {
        if (p.is_directory()) {
            bool not_found = std::find_if(excl_dirs.begin(), excl_dirs.end(),
                [loc_path = p.path()](const fs::path& p) { return fs::equivalent(loc_path, p);}
            ) == excl_dirs.end();
            return not_found;
        }
        return true;
    };
}


/** Create filter for exclude directory
 * @param excl_dirs  pathes to directory
 */
path_filter_func_t make_filter_exclude_dir(const fs::path& excl_dir) {
    return [excl_dir](const fs::directory_entry& p) {
        if (p.is_directory()) {
            return  !fs::equivalent(p.path(), excl_dir);
        }
        return true;
    };
}

/** Create filter for minimal size of regular file
 * @param size  files which size is not less are passed
 */
path_filter_func_t make_filter_min_file_size(const uintmax_t size) {
    return [size](const fs::directory_entry& p) {
        if (p.is_regular_file()) {
            return (fs::file_size(p) >= size);
        }
        return true;
    };
}

/** Create filter to skip all subdirs
 * @param size  files which size is not less are passed
 */
path_filter_func_t make_filter_no_subdirs() {
    return [](const fs::directory_entry& p) {
        return !p.is_directory();
    };
}

/** Create filter to pass only files by mask
 * @param wild_string  wildcard mask to padd file
 */
path_filter_func_t make_filter_wildcard(const std::string& wild_string) {
    return [w = Wildcard(wild_string)](const fs::directory_entry& p){
        return  p.is_regular_file() ? w.match(p.path().filename().string()) : true;
    };
}
path_filter_func_t make_filter_wildcard(const std::vector<std::string>& wild_strings) {
    return [w = Wildcard(wild_strings)](const fs::directory_entry& p){
        return p.is_regular_file() ? w.match(p.path().filename().string()) : true;
    };
}
path_filter_func_t make_filter_wildcard(const std::u8string& wild_string) {
    return [w = Wildcard(wild_string)](const fs::directory_entry& p){
        return p.is_regular_file() ? w.match(p.path().filename().u8string()) : true;
    };
}
path_filter_func_t make_filter_wildcard(const std::vector<std::u8string>& wild_strings) {
    return [w = Wildcard(wild_strings)](const fs::directory_entry& p){
        return p.is_regular_file() ? w.match(p.path().filename().u8string()) : true;
    };
}

/**
 * Scan direcories to build list of regular file pathes 
 * @param inc_dir list of directories where to scan 
 * @param filters list of filters to apply while scanning
 * @return std::list<fs::path> 
 */
std::list<fs::path> build_file_list(
    const std::list<fs::path>& inc_dir,
    const std::vector<path_filter_func_t>& filters) {

    std::list<fs::path> list;
    if (inc_dir.empty()) return list;

    std::list<fs::path> subdir_list;
    for (const auto& dir : inc_dir) {
        for (const auto& p : fs::directory_iterator(dir, fs::directory_options::skip_permission_denied)) {
            {   // filtering 
                bool passed = true;
                for (auto& pass_filter : filters) {
                    if (pass_filter(p) == false) {
                        passed = false;
                        break;
                    }
                }
                if (!passed) {
                    continue;
                }
            }

            if (p.is_directory()) {
                subdir_list.push_back(p.path());
            }
            else if (p.is_regular_file()) {
                list.push_back(p);
            }
        }
    }

    if (subdir_list.size()) {
        list.merge(build_file_list(subdir_list, filters));
    }

    return list;
}
