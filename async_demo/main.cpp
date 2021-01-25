/*
    async library usage demo
    3 threads process 3 different commands set  
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
    };
    
    RandomBraceGenerator g1("rndA", 500);
    RandomBraceGenerator g2("rndB", 500);
    std::vector<std::string> test_data_1 = { "cmd1","cmd2","cmd3","cmd4","cmd5","cmd6","cmd7","cmd8","cmd9" };
    IterGenerator<std::vector<std::string>::const_iterator> g3(test_data_1.cbegin(), test_data_1.cend());
    std::vector<std::string> test_data_2 = { "cmd_stat_1","{","cmd_dinam_1","{","cmd_dinam_2","}","}","cmd_stat_3","cmd_stat_4" };
    IterGenerator<std::vector<std::string>::const_iterator> g4(test_data_2.cbegin(), test_data_2.cend());

    std::thread t1(func, &g1, 3);
    std::thread t2(func, &g2, 3);
    std::thread t3(func, &g3, 2);
    std::thread t4(func, &g4, 1);
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}