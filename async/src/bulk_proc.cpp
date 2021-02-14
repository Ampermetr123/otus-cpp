#include "bulk_proc.h"

#include <functional>
#include <filesystem>
#include <fstream>

namespace async
{
    BulkProcessor::BulkProcessor(std::size_t bulk_size, WorkersPool& wp_cout, WorkersPool& wp_file) :
        pool_cout(wp_cout), pool_file(wp_file) {
        spBulk = std::make_shared<StaticBulk>(bulk_size);
    }


    BulkProcessor::~BulkProcessor() {
        handle_current_bulk();
    }


    void BulkProcessor::save(std::shared_ptr<Bulk> spBulk, std::string file_suffix) {
        if (!spBulk) {
            return;
        }
        std::string fnamebase = "bulk" + std::to_string(spBulk->start_time());
        const std::string ext = ".log";

        // Ensure each bulk has unique file
        std::filesystem::path fpath = fnamebase + "." + file_suffix + ext;
        for (int i = 0; std::filesystem::exists(fpath); i++) {
            fpath = fnamebase + "." + file_suffix + "." + std::to_string(i) + ext;
        };

        std::ofstream f(fpath.generic_string());
        if (f) {
            spBulk->output(f);
        };
    }


    void BulkProcessor::receive(std::string cmd) {

        auto newBulk = spBulk->add_command(cmd);
        if (newBulk) {
            handle_current_bulk();
            spBulk = std::move(newBulk);
        }
    }


    void BulkProcessor::handle_current_bulk()  {
        if (spBulk->processible()) {
            auto f1 = [sp = spBulk]([[maybe_unused]] int thread_number) { sp->output(std::cout); };
            pool_cout.make_job_async(f1);

            auto f2 = [sp = spBulk, this](int thread_number) { save(sp, std::to_string(thread_number)); };
            pool_file.make_job_async(f2);
        }
    }


} // namespace async
