#pragma once

#include <functional>
#include <fstream>
#include <filesystem>
#include <tuple>
#include <list>
#include <deque>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/name_generator_md5.hpp>
#include <boost/uuid/nil_generator.hpp>

using hash_func_t = std::function<boost::uuids::uuid(const char* buf, size_t size)>;

const auto hash_md5 = [gen = boost::uuids::name_generator_md5(boost::uuids::ns::url())]
(const char* buf, size_t size) { return gen(buf, size);};

const auto hash_sha1 = [gen = boost::uuids::name_generator_sha1(boost::uuids::ns::url())]
(const char* buf, size_t size) { return gen(buf, size);};

/** Intend to find files with dublicates of content
 */
class FileDupFinder {
public:
    FileDupFinder(std::uintmax_t block_size, const hash_func_t& hash_func);
    std::list<std::list<std::filesystem::path>> find(const std::list<std::filesystem::path>& file_pathes) const;

private:
    const std::uintmax_t block_size;
    const hash_func_t& hash_func;
    struct Metafile {
        std::filesystem::path path;
        std::uintmax_t file_size;
        std::uintmax_t blocks_num;
        std::deque<boost::uuids::uuid> file_hash;
        Metafile(std::string path, std::uintmax_t fsize, std::uintmax_t block_size) :
            path(path),
            file_size(fsize),
            blocks_num(fsize / block_size + (fsize % block_size > 0 ? 1 : 0)) {
        };

    };
    bool compare(Metafile& m1, Metafile& m2, char* buf) const;
    std::tuple<boost::uuids::uuid, bool> block_hash(Metafile& m, std::uintmax_t n, std::ifstream& f, char* buf) const;
};


