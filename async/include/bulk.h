#pragma once
#include <vector>
#include <string>
#include <memory>
#include <ostream>
#include <chrono>

namespace async {

    class Bulk {
    public:
        Bulk(size_t n);
        virtual ~Bulk();
        virtual std::unique_ptr<Bulk> addCommand(std::string cmd) = 0;
        static const std::string cmd_bulk_open;
        static const std::string cmd_bulk_close;
       
        virtual bool is_valid() = 0;
        auto start_time(){
            return std::chrono::system_clock::to_time_t(start);
        }
           
        void output(std::ostream& os) const;
        
    protected:
        void store_command(std::string cmd);
        size_t max_commands;
        std::chrono::system_clock::time_point start;
        std::vector<std::string> commands;
    };

    


    class StaticBulk : public Bulk {
    public:
        StaticBulk(size_t n);
        ~StaticBulk();
        std::unique_ptr<Bulk> addCommand(std::string cmd) override;
        bool is_valid() override;
    };
  
    


    class DynamicBulk : public Bulk {
    public:
        DynamicBulk(size_t n);
        ~DynamicBulk();
        std::unique_ptr<Bulk> addCommand(std::string cmd) override;
        bool is_valid() override;
    private:
        int brace_level;

    };

}