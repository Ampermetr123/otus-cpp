/**
 * @file main.cpp
 * @author Sergey Simonov
 * @brief MapReduce. Otus homework #11.
 */

#include <limits>
#include <cstdlib>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "mapreduce.h"
#include "reducer.h"
#include "version.h"
#include "optlog.h"

namespace optlog {
    OptLog log0(std::cout, 0, 0);
    OptLog log1(std::cout, 1, 1);
}

using namespace mapreduce;


int main(int argc, const char* argv[]) {
    using optlog::log0;
    using optlog::log1;
    try {

        // Parsing command line 
        po::options_description descr{ "Command line options" };
        descr.add_options()
            ("src", po::value<std::string>()->default_value("test.txt"), "Source text file with data")
            ("mnum", po::value<int>()->default_value(3), "number of mapper threads")
            ("rnum", po::value<int>()->default_value(3), "number of reduce threads")
            ("files", po::value<bool>()->default_value(true), "output reducers work to files")
            ("verbose", po::value<int>()->default_value(0), "verbose level (0 or 1)")
            ("help,h", "print this help message")
            ("version", "print version");
        po::positional_options_description pos_descr;
        pos_descr.add("src", 1);
        pos_descr.add("mnum", 1);
        pos_descr.add("rnum", 1);
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(descr).positional(pos_descr).run(), vm);

        // --help or --version handler
        if (vm.count("help")) {
            log0 << "Otus homework #11 by Sergey Simonov\n--------------------\n" << descr << std::endl;
            return 0;
        }
        else if (vm.count("version")) {
            log0 << "mapreduce version is " << PROJECT_VERSION << std::endl;
            return 0;
        }
        const auto src_file_name(vm["src"].as<std::string>());
        const auto verbose = vm["verbose"].as<int>();
        const auto mnum = vm["mnum"].as<int>();
        const auto rnum = vm["rnum"].as<int>();
        const auto files = vm["files"].as<bool>();

        // Loggers setup
        log0 = optlog::OptLog(std::cout, verbose, 0);
        log1 = optlog::OptLog(std::cout, verbose, 1);

        log1 << "Started with [ " << "source file: " << src_file_name <<
            " | mnum: " << mnum << " | rnum: " << rnum << " | files: " << files << " ]\n" << std::endl;

        // Creating reducers
        std::vector<reducer_func_t> reducers;
        std::string output_filename;
        for (int i = 1; i <= rnum; i++) {
            if (files) {
                output_filename = "reducer_" + std::to_string(i) + ".txt";
            }
            reducers.push_back(CheckRepeatsReducer(output_filename));
        }

        // Find miniumum prefix using mapreduce
        unsigned int prefix_length = 0;
        while (true)
        {
            prefix_length++;
            for (auto& r : reducers){
                r.target<CheckRepeatsReducer>()->reset(prefix_length);
            }

            mapper_func_t mapper = [prefix_length](std::string& s) -> std::string {
                if (s.size() > prefix_length) {
                    s.resize(prefix_length);
                };
                return s;
            };

            mapreduce::map_reduce(src_file_name, mnum, mapper, reducers);
            
            {   // Check prifix increments have no sence
                size_t exceed_count = 0;
                for (auto& rf : reducers) {
                    auto r = rf.target<CheckRepeatsReducer>();
                    if (r->max_length() < prefix_length) {
                        exceed_count++;
                    }
                }
                if (exceed_count == reducers.size()) {
                    log0 << "No minimum prefix. We need to type whole string: " << prefix_length - 1 << std::endl;
                    break;
                }
            }
            
            {   // Check all strigns with prefix length where unique 
                bool repeats_found = false;
                for (size_t i = 0; i < reducers.size(); i++) {
                    auto r = reducers.at(i).target<CheckRepeatsReducer>();
                    repeats_found = (repeats_found || r->repeats_found());
                    log1 << "reducer_" << std::to_string(i) << " -> repeated strings: " << (r->repeats_found() ? "yes" : "no") << std::endl;
                }
                if (repeats_found == false) {
                    log0 << "Minimum prefix is: " << prefix_length << std::endl;
                    break;
                }
            }
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
