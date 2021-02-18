#include "storage.h"
namespace joinstorage
{
    std::unique_ptr<IterableResult> Storage::insert(std::string table, storage_index_t id, storage_data_t data) 
    {        
        (void)id;
        (void)data;
        return std::unique_ptr<IterableResult>(new StringResult(table +"INSERT\n"));
    }
    
    std::unique_ptr<IterableResult> Storage::trucate(std::string table) 
    {
        return std::unique_ptr<IterableResult>(new StringResult(table+" TRUNCATE\n"));
    }
    
    std::unique_ptr<IterableResult> Storage::intersection(std::string table) 
    {
        return std::unique_ptr<IterableResult>(new StringResult(table + "INTERSECTION\n"));
    }
    
    std::unique_ptr<IterableResult> Storage::difference(std::string table) 
    {
        return std::unique_ptr<IterableResult>(new StringResult(table +"DIFFERENCE\n"));
    }
}