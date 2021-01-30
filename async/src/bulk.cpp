/**
 * @file bulk.h
 * @author  Sergey Simonov
 * @brief Bulk, StaticBulk and DynamicBulk structures implementation
 */

#include "bulk.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cassert>


namespace async {

    /*---------- Bulk -------------------------------------------------*/

    const std::string Bulk::cmd_bulk_open = "{";
    const std::string Bulk::cmd_bulk_close = "}";

    /**
     * @brief Construct a new Bulk:: Bulk object
     * @param n maximum commands in Bulk
     */
    Bulk::Bulk(size_t n) :max_commands(n) {
    }

    /**
     * @brief Return timestamp of Bulk start time
     * @return If bulk isn't valid return 0, else returns time_t (posix time).
     */
    time_t Bulk::start_time() const {
        return std::chrono::system_clock::to_time_t(start);
    }

    /**
     * @brief Put bulk context to ouput stream
     * Format is "bulk: cmd1 cmd2 ... cmdn {newline}"
     * @param os
     */
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


    /*---------- Static Bulk ------------------------------------------*/

    /**
     * @brief Construct a new Static Bulk:: Static Bulk object
     * @param n maximum commands in  Bulk
     */
    StaticBulk::StaticBulk(size_t n) :Bulk(n) {
    }

    /**
     * @brief Add command to Bulk
     * When Bulk is compleated or interrupted with new Dynamic bulk
     * it constructs and return pointer to new Bulk
     * @param cmd command
     * @return std::unique_ptr<Bulk> (nullptr) if bulk is not compleate
     * @return std::unique_ptr<Bulk> to new Bulk if bulk is compleate
     */
    std::unique_ptr<Bulk> StaticBulk::addCommand(std::string cmd) {
        if (cmd == Bulk::cmd_bulk_open) {
            return std::unique_ptr<Bulk>(new DynamicBulk(max_commands));
        }
        else if (cmd == Bulk::cmd_bulk_close) {
            ;// input error? ignore it
        }
        else {
            // Static bulk starts with first command
            if (commands.empty()) {
                start = std::chrono::system_clock::now();
            }
            commands.push_back(cmd);
            if (commands.size() >= max_commands)
                return std::unique_ptr<Bulk>(new StaticBulk(max_commands));
        }
        return nullptr;
    }

    /**
     * @return true if Bulk is correct could be processed
     * @return false if Bulk is invalid
     */
    bool StaticBulk::is_valid() const {
        return !commands.empty();
    }


    /*---------- Dynamic Bulk -----------------------------------------*/

    /**
     * @brief Construct a new Dynamic Bulk:: Dynamic Bulk object
     * @param n maximum commands in Bulk (Static Bulk)
     */
    DynamicBulk::DynamicBulk(size_t n) :Bulk(n), brace_level(1) {
        // Dynamic bulk starts when it's created with open brace {
        start = std::chrono::system_clock::now();
    }

    /**
     * @brief Add command to Bulk
     * When Bulk is compleated construct and return pointer to new Static Bulk
     * @param cmd command
     * @return std::unique_ptr<Bulk> (nullptr) if bulk is not compleate
     * @return std::unique_ptr<Bulk> to new Bulk if bulk is compleate
     */
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
            commands.push_back(cmd);
        }
        return nullptr;
    }

    /**
     * @return true if Bulk is correct could be processed.
     * @return false if Bulk is invalid
     */
    bool DynamicBulk::is_valid() const {
        return (brace_level == 0);
    }

}