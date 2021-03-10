/**
 * @file main.cpp
 * @author Sergey Simonov
 * @brief Homework #7
 */
#include <boost/program_options.hpp>
#include <iostream>
#include <filesystem>
#include <list>
#include <memory>

#include "version.h"
#include "file_list.h"
#include "file_dup.h"
#include "log.h"

namespace po = boost::program_options;
namespace fs = std::filesystem;


/** Convert vector of string to list of canonical pathes.
 * All strings that aren't represent directroeis are skipped
 */
template< class Container >
void insert_dir_pathes(const std::vector<std::string>& dirs, std::back_insert_iterator<Container> inserter) {
    for (const auto& s : dirs) {
        auto p = fs::path(move(s));
        if (fs::is_directory(p)) {
            auto abs_p = fs::canonical(p);
            inserter = abs_p;
            log1 << abs_p.string() << std::endl;
        }
        else {
            log1 << "skip non directory: " << s << std::endl;
        }
    }
}


/** Helper function to validate [min,max] with boost::program_options
 */
template <typename T>
const auto range_opt_validator = [](T min, T max, char const* const opt_name) {
    return [min, max, opt_name](T val) {
        if (val < min || val > max) {
            throw po::validation_error(po::validation_error::invalid_option_value,
                opt_name, std::to_string(val));
        }
    };
};


int main(int argc, const char* argv[]) {
    using namespace std;
    uintmax_t min_file_size;
    uintmax_t block_size;
    int hash_algo;

    try {
        // Program options
        po::options_description descr{ "Command line options" };
        descr.add_options()
            ("include-dir,I", po::value<vector<string>>()->multitoken()->required(), "Including directories to scan")
            ("exclude-dir,E", po::value<vector<string>>()->multitoken(), "Excluding directories from scan")
            ("recursive,r", "Scan recursively (subdirectories included)")
            ("min-size,s", po::value<uintmax_t>(&min_file_size), "Minimum file size to scan")
            ("file-masks,M", po::value<vector<string>>()->multitoken(), "One or more file masks to scan")
            ("block-size,b", po::value<uintmax_t>(&block_size)->default_value(1024)->notifier(range_opt_validator<uintmax_t>(0, 1024 * 1024, "block-size")),
                "size of chunk in bytes for hash calculating, maximum 1 Mb")
            ("hash,H", po::value<int>(&hash_algo)->default_value(0)->notifier(range_opt_validator<int>(0, 1, "hash")),
                "Hash alogorithm: 0 - md5; 1 - sha1")
            ("help,h", "print this help message")
            ("version", "print version");
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(descr).run(), vm);

        // --help or --version handler
        if (vm.count("help")) {
            log0 << "bayan: otus homework #7 by Sergey Simonov \n" << descr << std::endl;
            return 0;
        }
        else if (vm.count("version")) {
            log0 << "bayan version is " << PROJECT_VERSION << std::endl;
            return 0;
        }

        // Validation
        vm.notify(); // raise exception when required options are missed

        // Prepare include dir list
        log1 << (vm.count("recursive") ? "Scan directories recursively:\n" : "Scan directories:\n");
        std::list<fs::path> include_dirs;
        insert_dir_pathes(vm["include-dir"].as<vector<string>>(), std::back_inserter(include_dirs));

        if (include_dirs.empty()) {
            log1 << "Nothing to scan" << endl;
            return 1;
        }

        // Building filters
        std::vector<path_filter_func_t> filters;
        filters.reserve(4);
        if (!vm.count("recursive")) {
            filters.push_back(make_filter_no_subdirs());
        }
        if (vm.count("exclude-dir")) {
            log1 << "Exclude directories:\n";
            std::vector<fs::path> exclude_dirs;
            auto& vm_excl_dirs = vm["exclude-dir"].as<vector<string>>();
            exclude_dirs.reserve(vm_excl_dirs.size());
            insert_dir_pathes(vm_excl_dirs, std::back_inserter(exclude_dirs));
            filters.push_back(make_filter_exclude_dir(move(exclude_dirs)));
        }
        if (vm.count("min-size")) {
            filters.push_back(make_filter_min_file_size(min_file_size));
        }
        if (vm.count("file-masks")) {
            const auto& file_masks = vm["file-masks"].as<vector<string>>();
            vector<u8string> u8_file_masks;
            u8_file_masks.reserve(file_masks.size());
            // We use u8string to force unicode matching (ciryllic simbos support)
            for (const auto& s : file_masks) {
                u8_file_masks.push_back(u8string(reinterpret_cast<const char8_t*>(s.c_str())));
            }
            filters.push_back(make_filter_wildcard(u8_file_masks));
        }

        // Build file list to scan
        auto file_list = build_file_list(include_dirs, filters);

        // Creating FileDupFinder according options
        std::unique_ptr<FileDupFinder> dup_finder;
        if (hash_algo == 0) {
            dup_finder.reset(new FileDupFinder(block_size, hash_md5));
        }
        else {
            dup_finder.reset(new FileDupFinder(block_size, hash_sha1));
        }

        // Dup seatching
        auto result_list = dup_finder->find(file_list);

        // Result printing
        log0 << "Scanned " << file_list.size() << " files." << endl;
        if (result_list.empty()) {
            log0 << "Dublicated files not found" << endl;
        }
        log0 << "Found " << result_list.size() << " cases of dublicated content:" << endl;
        size_t i = 0;
        for (auto& dup : result_list) {
            log0 << endl << "Dublicates #" << i++ << ", "<<dup.size()<<" files:" << endl;
            for (auto p : dup)
                log0 << p.string() << endl;
        };

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
