/**
 * @file main.cpp
 * @author Sergey Simonov
 * @brief Bulk server implemented with boost:asio. Otus homework #9.
 */

#include <boost/program_options.hpp>

#include "version.h"
#include "server.h"

namespace po = boost::program_options;


int main(int argc, const char* argv[]) {
    try {
        // 1) parse command line 
        po::options_description descr{ "Command line options" };
        descr.add_options()
            ("port", po::value<std::uint16_t>()->default_value(9000), "TCP port for incoming connections")
            ("size", po::value<size_t>()->default_value(3), "size of bulk")
            ("verbose", po::value<int>()->default_value(0), "verbose level (0 or 1)")
            ("help,h", "print this help message")
            ("version", "print version");
        po::positional_options_description pos_descr;
        pos_descr.add("port", 1);
        pos_descr.add("size", 2);
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(descr).positional(pos_descr).run(), vm);

        // 2) --help or --version handler
        if (vm.count("help")) {
            std::cout << "Otus homework #9 by Sergey Simonov \n \n" << descr << std::endl;
            return 0;
        } else if (vm.count("version")) {
            std::cout << "Bulk server version is " << PROJECT_VERSION << std::endl;
            return 0;
        }
        po::notify(vm);

        // 3) create and start server
        std::unique_ptr<BulkServer> pServer(
            new BulkServer( vm["port"].as<uint16_t>(), 
                            vm["size"].as<size_t>(),
                            vm["verbose"].as<int>() )
        );
        pServer->run();

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
