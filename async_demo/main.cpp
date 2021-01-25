/*
    async library usage demo
    Set of threads processes comands
*/

#include "async.h"
#include "generator.h"
#include <iostream>
#include <thread>


int main(int, char**) {

    auto func = [](Generator* gen, int bulk_size) {
        auto handle = async::connect(bulk_size);
        for (const auto& v : *gen) {
            async::receive(handle, v.c_str(), v.size());
            std::this_thread::yield();
        }
        async::disconnect(handle);
    };

    std::vector<std::thread> threads;

    RandomBraceGenerator g1("rndA", 5000);
    threads.emplace_back(func, &g1, 3);

    RandomBraceGenerator g2("rndB", 5000);
    threads.emplace_back(func, &g2, 2);

    std::vector<std::string> test_data_1 =
    { "cmd1","cmd2","cmd3","cmd4","cmd5","cmd6","cmd7","cmd8","cmd9" };
    IterGenerator<std::vector<std::string>::const_iterator> g3(test_data_1.cbegin(), test_data_1.cend());
    threads.emplace_back(func, &g3, 2);

    std::vector<std::string> test_data_2 =
    { "cmd_stat_1","{","cmd_dinam_1","{","cmd_dinam_2","}","}","cmd_stat_3","cmd_stat_4" };
    IterGenerator<std::vector<std::string>::const_iterator> g4(test_data_2.cbegin(), test_data_2.cend());
    threads.emplace_back(func, &g4, 1);

    for (auto& t : threads) {
        if (t.joinable())
            t.join();
    }

    return 0;
}