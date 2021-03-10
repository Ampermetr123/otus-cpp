/**
 * @file file_list.h
 * @author  Sergey Simonov
 * @brief Function to scan directories and generate list of files according to filters
 */
#pragma once
#include <string>
#include <vector>
#include <list>
#include <filesystem>
#include <functional>
#include "wildcard.h"

namespace fs = std::filesystem;

/// Filter function type Input: file system entry. Output: true to pass entry to list.
using path_filter_func_t = std::function<bool(const fs::directory_entry&)>;

path_filter_func_t make_filter_exclude_dir(std::vector<fs::path> excl_dir);
path_filter_func_t make_filter_exclude_dir(const fs::path& excl_dir);
path_filter_func_t make_filter_min_file_size(const uintmax_t size);
path_filter_func_t make_filter_no_subdirs();
path_filter_func_t make_filter_wildcard(const std::string& wild_string);
path_filter_func_t make_filter_wildcard(const std::vector<std::string>& wild_strings);
path_filter_func_t make_filter_wildcard(const std::u8string& wild_string);
path_filter_func_t make_filter_wildcard(const std::vector<std::u8string>& wild_strings);


std::list<fs::path> build_file_list(
    const std::list<fs::path>& inc_dir,
    const std::vector<path_filter_func_t> &filters = std::vector<path_filter_func_t>()
);
