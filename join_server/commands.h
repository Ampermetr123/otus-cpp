#include <string>
#include <memory>
#include <sstream>
#include <iterator>
#include <iostream>
#include <cctype>
#include "iterable_result.h"
#include "storage.h"

class ICommand {
public:
    virtual std::unique_ptr<IterableResult> execute() = 0;
    virtual ~ICommand() = default;
};


class Command : public ICommand {
public:
    virtual ~Command() = default;
protected:
    Command(IStorage& js) : storage(js) {}
    IStorage& storage;
};


/* -----------------    INSERT    ------------------------------*/
class Command_Insert : public Command {
private:
    std::string table;
    storage_index_t id;
    storage_data_t data;

public:
    Command_Insert(IStorage& js, std::string table, storage_index_t id, storage_data_t data)
        :Command(js), table(table), id(id), data(data) {
    }
    std::unique_ptr<IterableResult> execute() override {
        return storage.insert(table, id, data);
    }

    template <class InputStream>
    static std::tuple<std::unique_ptr<Command>, std::string> create(IStorage& js, InputStream& sstr) {
        using namespace std;
        string table;
        sstr >> table;
        if (table.empty()) {
            return { nullptr, "ERR no valid table provided" };
        }
        else if (js.checkTableName(table) == false) {
            return  { nullptr, "ERR invalid table '" + table + "'" };
        }

        string index_str;
        sstr >> index_str;
        if (!sstr || index_str.empty() || index_str[0] == '-') {
            return { nullptr, "ERR no valid index provided" };
        }
        unsigned long index;
        try {
            index = std::stoul(index_str);
        }
        catch (const std::exception& e) {
            return { nullptr, "ERR no valid index provided" };
        }

        storage_data_t data;
        sstr.get(); // to skip space beetween index and data string
        getline(sstr, data);
        // some clients finish line with \r\n, so we check and remove \r
        if (data.size() && data.back() == '\r') {
            data.pop_back();
        }
        if (data.size() == 0) {
            return { nullptr, "ERR no valid data provided" };
        }

        return { std::unique_ptr<Command>(new Command_Insert(js, table,  index, data)), "" };
    }
};


/* -----------------    INSERSECTION    ------------------------------*/
class Command_Intersection : public Command {
public:
    Command_Intersection(IStorage& js) : Command(js) {}
    std::unique_ptr<IterableResult> execute() override {
        return storage.intersection();
    }

    template <class InputStream>
    static std::tuple<std::unique_ptr<Command>, std::string> create(IStorage& js, [[maybe_unused]] InputStream& args) {
        std::string s;
        args >> s;
        if (s.size() > 0) {
            return { nullptr, "ERR redundant arguments" };
        }
        return { std::unique_ptr<Command>(new Command_Intersection(js)) , "" };
    }
};


/* -----------------    DIFFERENCE    ------------------------------*/
class Command_Difference : public Command {
public:
    Command_Difference(IStorage& js) : Command(js) {}
    std::unique_ptr<IterableResult> execute() override {
        return storage.difference();
    }

    template <class InputStream>
    static std::tuple<std::unique_ptr<Command>, std::string>
        create(IStorage& js, [[maybe_unused]] InputStream& args) {
        std::string s;
        args >> s;
        if (s.size() > 0) {
            return { nullptr, "ERR redundant arguments" };
        }
        return  { std::unique_ptr<Command>(new Command_Difference(js)), "" };
    }
};


/* -----------------    TRUNCATE     ------------------------------*/
class Command_Truncate : public Command {
private:
    std::string table;

public:
    Command_Truncate(IStorage& js, std::string table)
        : Command(js), table(table) {
    }
    std::unique_ptr<IterableResult> execute() override {
        return storage.truncate(table);
    }

    template <class InputStream>
    static std::tuple<std::unique_ptr<Command>, std::string> create(IStorage& js, InputStream& args) {
        using namespace std;
        string table;
        args >> table;
        if (table.empty()) {
            return { nullptr, "ERR no valid table provided" };
        }
        else if (js.checkTableName(table) == false) {
            return  { nullptr, "ERR invalid table '" + table + "'" };
        }

        std::string s;
        args >> s;
        if (s.size() > 0) {
            return { nullptr, "ERR redundant arguments" };
        }
        return { std::unique_ptr<Command>(new Command_Truncate(js, table)), "" };
    }
};

/* ------------------- Factory method ----------------------- */
    /**
     * @brief Parse line and creates Command
     * @param str line with user command
     * @return std::unique_ptr<ICommand>  with command or nullptr of parse error
     */
inline std::tuple<std::unique_ptr<Command>, std::string> make_command(IStorage& storage, std::string str) {
    using namespace std;
    stringstream sstr(move(str));
    string strcmd;
    sstr >> strcmd;
    sstr.get(); // extract space after command
    if (strcmd.empty()) {
        return { nullptr, "ERR empty command" };
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
    return { nullptr , "ERR unknown command "s + strcmd };
}

