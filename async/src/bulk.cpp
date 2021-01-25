#include "bulk.h"
#include <iostream>
#include <algorithm>
#include <iterator>


namespace async {


    /*---------- Bulk -------------------------------------------------*/

    const std::string Bulk::cmd_bulk_open = "{";
    const std::string Bulk::cmd_bulk_close = "}";

    Bulk::Bulk(size_t n) :max_commands(n) {
        //  std::cout << " up " << std::endl;
    }


    Bulk::~Bulk() {
        //  std::cout << " down " << std::endl;
    }


    void Bulk::output(std::ostream& os) const {
        os << "bulk: ";
        if (commands.size()) {
            auto last = std::prev(commands.end());
            std::copy(commands.begin(), last, std::ostream_iterator<std::string>(os, ", "));
            os << *last << std::endl;
        }
        else {
            os << std::endl;
        }
    }

    void Bulk::store_command(std::string cmd) {
        if (commands.empty()) {
            start = std::chrono::system_clock::now();
        }
        commands.push_back(cmd);
    }

    // void Bulk::log() {

    //     this->output(std::cout);

  
    // }

    /*---------- Static Bulk ------------------------------------------*/

    StaticBulk::StaticBulk(size_t n) :Bulk(n) {}

    StaticBulk::~StaticBulk() {
        //    this->log();
    }

    std::unique_ptr<Bulk> StaticBulk::addCommand(std::string cmd) {
        if (cmd == Bulk::cmd_bulk_open) {
            return std::unique_ptr<Bulk>(new DynamicBulk(max_commands));
        }
        else if (cmd == Bulk::cmd_bulk_close) {
            ;// input error? ignore it
        }
        else {
            store_command(cmd);
            if (commands.size() >= max_commands)
                return std::unique_ptr<Bulk>(new StaticBulk(max_commands));
        }
        return nullptr;
    }

    bool StaticBulk::is_valid() {
        return !commands.empty();
    }

    /*---------- Dynamic Bulk -----------------------------------------*/

    DynamicBulk::DynamicBulk(size_t n) :Bulk(n), brace_level(1) {}


    std::unique_ptr<Bulk> DynamicBulk::addCommand(std::string cmd) {
        if (cmd == Bulk::cmd_bulk_open) {
            brace_level++;
        }
        else if (cmd == Bulk::cmd_bulk_close) {
            brace_level--;
            if (brace_level == 0) {
                return std::unique_ptr<Bulk>(new StaticBulk(max_commands));
            }
        }
        else {
            store_command(cmd);
        }
        return nullptr;
    }

    bool DynamicBulk::is_valid() {
        return (brace_level == 0);
    }
    DynamicBulk::~DynamicBulk() {
    }

}