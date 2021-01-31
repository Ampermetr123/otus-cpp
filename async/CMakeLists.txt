add_library(async 
    SHARED 
    src/async.cpp
    src/bulk.cpp
    src/bulk_proc.cpp
    src/workers_pool.cpp
)

set_default_opts(async)

target_include_directories( async 
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/header"
    INTERFACE 
    "${CMAKE_CURRENT_SOURCE_DIR}/header"
)

target_link_libraries( async ${CMAKE_THREAD_LIBS_INIT} )

install(TARGETS async DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/header/async.h DESTINATION include)

