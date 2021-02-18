#include <string>
#include <memory>
#include "storage.h"
#include <sstream>
#include <iterator>
#include <iostream>
#include <cctype>
#include "iterable_result.h"

namespace joinstorage {

    class ICommand {
    public:
        virtual std::unique_ptr<IterableResult> execute() = 0;
        virtual ~ICommand() = default;
    };


    class Command : public ICommand {
    public:
        virtual ~Command() = default;
    protected:
        Command(Storage& js) : storage(js) {}
        Storage& storage;
    };


    class Command_Insert : public Command {
    private:
        std::string table;
        storage_index_t id;
        storage_data_t data;
    public:
        Command_Insert(Storage& js, std::string table, storage_index_t id, storage_data_t data)
            :Command(js), table(table), id(id), data(data) {
        }
        std::unique_ptr<IterableResult> execute() override {
            return std::unique_ptr<IterableResult>(new StringResult("INSERT OK"));
        }

        template <class InputStream>
        static std::tuple<std::unique_ptr<Command>, std::string> create(Storage& js, InputStream& sstr) {
            using namespace std;
            string table;
            sstr >> table;
            if (table.empty()) {
                return { nullptr, "no valid table provided" };
            }

            storage_index_t index;
            sstr >> index;
            if (!sstr) {
                return { nullptr, "no valid index provided" };
            }

            storage_data_t data;
            getline(sstr, data);
            if (data.empty()) {
                return { nullptr, "no valid data provided" };
            }

            return { std::unique_ptr<Command>(new Command_Insert(js, table,  index, data)), "" };
        }


    };


    class Command_Intersection : public Command {
    public:
        Command_Intersection(Storage& js) : Command(js) {}
        std::unique_ptr<IterableResult> execute() override {
            return std::unique_ptr<IterableResult>(new StringResult("INSERSECTION OK"));
        }

        template <class InputStream>
        static std::tuple<std::unique_ptr<Command>, std::string> create(Storage& js, [[maybe_unused]] InputStream& sstr) {
            return { std::unique_ptr<Command>(new Command_Intersection(js)) , "" };
        }
    };


    class Command_Difference : public Command {
    public:
        Command_Difference(Storage& js) : Command(js) {}
        std::unique_ptr<IterableResult> execute() override {
            return std::unique_ptr<IterableResult>(new StringResult("DIFFERENCE OK"));
        }

        template <class InputStream>
        static std::tuple<std::unique_ptr<Command>, std::string> create(Storage& js, [[maybe_unused]] InputStream& sstr) {
            return { std::unique_ptr<Command>(new Command_Difference(js)), "" };
        }
    };


    class Command_Truncate : public Command {
    private:
        std::string table;

    public:
        Command_Truncate(Storage& js, std::string table)
            : Command(js), table(table) {
        }
        std::unique_ptr<IterableResult> execute() override {
            return std::unique_ptr<IterableResult>(new StringResult("TRUNCATE OK"));
        }
        
        template <class InputStream>
        static std::tuple<std::unique_ptr<Command>, std::string> create(Storage& js, InputStream& sstr) {
            using namespace std;
            string table;
            sstr >> table;
            if (table.empty()) {
                return { nullptr, "no valid table provided" };
            }
            return { std::unique_ptr<Command>(new Command_Truncate(js, table)), "" };
        }

    };


    /**
     * @brief Parse line and creates Command
     * @param str line with user command
     * @return std::unique_ptr<ICommand>  with command or nullptr of parse error
     */
    inline std::tuple<std::unique_ptr<Command>, std::string> make_command(Storage& storage, std::string str) {
        using namespace std;
        // split string to words
        stringstream sstr(move(str));
        string strcmd;
        sstr >> strcmd;
        if (strcmd.empty()) {
            return { nullptr, "empty command" };
        }
        else if (strcmd == "INSERT") {
            return Command_Insert::create(storage, sstr);
        }
        else if (strcmd == "TRUNCATE") {
            return Command_Truncate::create(storage, sstr);
        }
        else if (strcmd == "INTERSECTION") {
            return Command_Intersection::create(storage, sstr);
        }
        else if (strcmd == "SYMMETRIC_DIFFERENCE") {
            return Command_Difference::create(storage, sstr);
        }
        return { nullptr , "unknown command" };
    }

}