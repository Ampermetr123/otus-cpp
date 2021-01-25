#define ASYNC_API_EXPORTS
#include "async.h"
#include "context_manager.h"
#include "bulk_proc.h"

namespace async {

    using index_t = std::uintptr_t;
    ContextManager<BulkProcessor, index_t> cm;
    WorkersPool poolConsoleOut(1);
    WorkersPool poolFileOut(2);

    ASYNC_API handle_t connect(std::size_t bulk) {
        auto pair = cm.create(bulk, poolConsoleOut, poolFileOut);
        auto& id = pair.first;
        return reinterpret_cast<handle_t>(id);
    }
    
    ASYNC_API void receive(handle_t handle, const char* data, std::size_t size) {
        auto wpCtx = cm.fetch(reinterpret_cast<index_t>(handle));
        auto spCtx = wpCtx.lock();
        if (spCtx) {
            spCtx->receive(std::string(data, size));
        }
    }

    ASYNC_API void disconnect(handle_t handle) {
         cm.free(reinterpret_cast<index_t>(handle));
    }
}