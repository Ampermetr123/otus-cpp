/**
 * @file context_manager.h
 * @author Sergey Simonov
 * @brief Thread safe context Manager
*/
#pragma once
#include <memory>
#include <map>
#include <type_traits>
#include <mutex>
#include <shared_mutex>

namespace async {
    /**
     * @brief Context Manager
     *  Constucts and manage access to objects (type CtxType) by unique id (type IndexType).
     *  Context Manager is thread-safe.
     * @tparam CtxType type of managed Objects
     * @tparam IndexType type of index
     */
    template<typename CtxType, typename IndexType>
    class ContextManager {
        static_assert(std::is_integral_v<IndexType>,
            "Contex Manager requires IndexType to be integral");
    private:
        ContextManager(const ContextManager& cm) = default;
        ContextManager(ContextManager&& cm) = default;
        std::map<IndexType, std::shared_ptr<CtxType>> context;
        inline static IndexType id_counter = 0;
        mutable std::shared_timed_mutex container_mutex;


    public:
        ContextManager() = default;
        ~ContextManager() = default;

        /**
         * @brief Construct new object
         * @param args args for Object's class constructor
         * @return std::pair<IndexType, std::weak_ptr<CtxType>> id of Object
         */
        template<typename... Args>
        std::pair<IndexType, std::weak_ptr<CtxType>> create(Args&&... args) {
            std::unique_lock<decltype(container_mutex)> lock(container_mutex);
            context[++id_counter] = std::make_shared<CtxType>(args...);
            return  std::pair<IndexType, std::weak_ptr<CtxType> >(id_counter, context[id_counter]);
        }

        /**
         * @brief Retrive pointer to object by id
         * @param id of Object
         * @return std::weak_ptr<CtxType> pointer to Object
         */
        std::weak_ptr<CtxType> fetch(IndexType id) {
            std::shared_lock<std::shared_timed_mutex> lock(container_mutex);
            auto it = context.find(id);
            if (it == context.end())
                return std::weak_ptr<CtxType>{};
            return it->second;
        };

        /**
         * @brief Destruct the object
         * @note due to smart pointer object could be destroyed later
         * @param id id of object
         */
        bool free(IndexType id) {
            std::unique_lock<decltype(container_mutex)> lock(container_mutex);
            context.erase(id);
            return true;
        };

    };

}