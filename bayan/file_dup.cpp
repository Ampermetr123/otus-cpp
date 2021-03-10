#include <cassert>
#include "file_dup.h"

namespace fs = std::filesystem;

/**
 * @brief Construct a new File Dup Finder:: File Dup Finder object
 * @param block_size file block size to calculate hash on it
 * @param hash_func hash function to use
 */
FileDupFinder::FileDupFinder(std::uintmax_t block_size, const hash_func_t& hash_func) :
    block_size(block_size), hash_func(hash_func) {
}

/*** Find dublicates in files, pointed in files_pathes
 * @param file_pathes list of pathes to analize for dublicates
 * @return List of dublicates list. Dublicate list consist of pathes to files with the same content
 */
std::list<std::list<fs::path>> FileDupFinder::find(const std::list<fs::path>& file_pathes) const {
    std::list<std::list<fs::path>> dup_list; // result
    std::vector<char> buffer(block_size);

    std::list<Metafile> mf_list; // mf = metafile list
    for (const auto& p : file_pathes) {
        mf_list.emplace_back(Metafile(p.string(), fs::file_size(p), block_size));
    };

    while (mf_list.size() > 1) {
        
        auto target = mf_list.front();
        mf_list.pop_front();
        // Move all pathes with same file content from mf_list to rep_list
        std::list<fs::path> rep_list;
        auto it = std::begin(mf_list);
        while (it != std::end(mf_list)) {
            if (compare(target, *it, buffer.data())) {
                rep_list.push_back((*it).path);
                it = mf_list.erase(it);
            }
            else {
                it++;
            }
        }

        if (rep_list.size() > 0) {
            rep_list.push_front(target.path);
            dup_list.emplace_back(std::move(rep_list));
        }
    }
    return dup_list;
}


/**
 * @brief Compare to files by contnet
 *
 * @param m1 metafile for file1
 * @param m2 metafile for file2
 * @param buf buffer to use when calc hash. Must be not less then Metafile::block_size memory available
 * @return true, if file has the same content, or boath are zero-size
 */
bool FileDupFinder::compare(Metafile& m1, Metafile& m2, char* buf) const {

    if (m1.file_size != m2.file_size) {
        return false;
    }
    if (m1.file_size == 0) {
        return true;
    }

    std::ifstream f1, f2;
    bool equal = true;
    for (uintmax_t i = 0; i < m1.blocks_num; i++) {
        const auto& [uuid1, success_1] = block_hash(m1, i, f1, buf);
        const auto& [uuid2, success_2] = block_hash(m2, i, f2, buf);
        if (!success_1 || !success_2) {
            equal = false;
            break;
        }
        if (uuid1 != uuid2) {
            equal = false;
            break;
        }
    }

    // Care of resources
    if (f1.is_open()) {
        f1.close();
    }
    if (f2.is_open()) {
        f2.close();
    }
    return equal;
}


/**
 * @brief Get hash of block n 
 * @note  If hash present in cashe, return it
 *        else read block from file, calculate hash and store in cashe
 * @param m metafile
 * @param n block number
 * @param f file resource to use when file reading
 * @param buf buffer resource to use when calculate hash (must be >= m.block_size)
 * @return std::tuple<boost::uuids::uuid, bool>  { hash , true - if no error }
 */
std::tuple<boost::uuids::uuid, bool> FileDupFinder::block_hash(Metafile& m, std::uintmax_t n, std::ifstream& f, char* buf) const {
    assert(n < m.blocks_num);
    if (n < m.file_hash.size()) {
        return { m.file_hash[n], true };
    }
    if (!f.is_open()) {
        f.open(m.path, std::ios::binary);
    }

    f.seekg(block_size * n);
    f.read(buf, block_size);
    if (f.bad()) {
        return { boost::uuids::nil_uuid(), false };
    }
    if (f.eof()) {
        auto bytes_read = f.gcount();
        std::memset(buf + bytes_read, 0x00, block_size - bytes_read);
    }
    m.file_hash.push_back(hash_func(buf, block_size));
    return { m.file_hash[n], true };
}