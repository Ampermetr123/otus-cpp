/**
 * @file bulk.h
 * @author  Sergey Simonov
 * @brief Bulk, StaticBulk and DynamicBulk structures declaration
 */

#pragma once
#include <vector>
#include <string>
#include <memory>
#include <ostream>
#include <chrono>

namespace async {

    class Bulk {
    protected:
        static const std::string cmd_bulk_open;
        static const std::string cmd_bulk_close;

    public:
        Bulk(size_t n);
        virtual ~Bulk() = default;
        virtual std::unique_ptr<Bulk> add_commad(std::string cmd) = 0;
        virtual bool is_valid() const = 0;
        time_t start_time() const;
        void output(std::ostream& os) const;

    protected:
        size_t max_commands;
        std::chrono::system_clock::time_point start;
        std::vector<std::string> commands;
    };


    class StaticBulk : public Bulk {
    public:
        StaticBulk(size_t n);
        ~StaticBulk() = default;
        std::unique_ptr<Bulk> add_commad(std::string cmd) override;
        bool is_valid() const override;
    };


    class DynamicBulk : public Bulk {
    public:
        DynamicBulk(size_t n);
        ~DynamicBulk() = default;
        std::unique_ptr<Bulk> add_commad(std::string cmd) override;
        bool is_valid() const override;
    private:
        int brace_level;
    };

}