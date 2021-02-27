/**
 * @file main.cpp
 * @author  Sergey Simonov
 * async library demonstration program
*/

#include "async.h"
#include "generator.h"
#include <iostream>
#include <thread>

/**
 * Start several threads.
 * Eeach thread preform connect() call, a lot of receive() calls and disconnect() call
 */
int main(int, char**) {

    /**
     * Function for threads
     * @param gen generator of data
     * @param bulk_size - size of bulk (passed to connect() )
     */
    auto func = [](Generator<std::string>& gen, int bulk_size) {
        auto handle = async::connect(bulk_size);
        for (const auto& v : gen) {
            async::receive(handle, v.c_str(), v.size());
            // yield thread to shuffle data in workers queue
            std::this_thread::yield();
        }
        async::disconnect(handle);
    };

    std::vector<std::thread> threads;

    // Many random data thread 1
    RandomBraceGenerator g1("A_", 5000);
    threads.emplace_back(func, std::ref(g1), 3);

    // Many random data thread 2
    RandomBraceGenerator g2("B_", 5000);
    threads.emplace_back(func, std::ref(g2), 2);

    // Simple fixed and short data wihout braces
    std::vector<std::string> test_data_1 =
    { "cmd1","cmd2","cmd3","cmd4","cmd5","cmd6","cmd7","cmd8","cmd9" };
    IterGenerator<std::vector<std::string>::const_iterator> g3(test_data_1.cbegin(), test_data_1.cend());
    threads.emplace_back(func, std::ref(g3), 2);

    // Simple fixed and short data with braces
    std::vector<std::string> test_data_2 =
    { "cmd_stat_1","{","cmd_dinam_1","{","cmd_dinam_2","}","}","cmd_stat_3","cmd_stat_4" };
    IterGenerator<std::vector<std::string>::const_iterator> g4(test_data_2.cbegin(), test_data_2.cend());
    threads.emplace_back(func, std::ref(g4), 1);

    for (auto& t : threads) {
        if (t.joinable())
            t.join();
    }

    return 0;
}