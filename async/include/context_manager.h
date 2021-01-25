#include <memory>
#include <map>
#include <iostream>
#include <atomic>

template<typename CtxType, typename IndexType>
class ContextManager {
public:
    ContextManager() = default;
    ContextManager(const ContextManager& cm) = default;
    ContextManager(ContextManager&& cm) = default;
    ~ContextManager(){
        std::cout << "Good bye!" << std::endl;
        
    }
    
    std::weak_ptr<CtxType> fetch(IndexType id) {
        auto it = context.find(id);
        if (it == context.end())
            return std::weak_ptr<CtxType>{};
        return it->second;
    };

    template<typename... Args>
    std::pair<IndexType, std::weak_ptr<CtxType>> create(Args&&... args) {
        auto id = id_counter.fetch_add(1);
        context[id] = std::make_shared<CtxType>(args...);
        return  std::pair<IndexType, std::weak_ptr<CtxType> >(id, context[id]);
    }

    bool free(IndexType id) {
        context.erase(id);
        return true;
    };
private:
    std::map<IndexType, std::shared_ptr<CtxType>> context;
    inline static std::atomic<IndexType> id_counter = 0;
};

