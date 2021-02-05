add_executable(bulk_server
    main.cpp server.cpp
)
configure_file(version.h.in  version.h)
target_include_directories(bulk_server PRIVATE 
    "${CMAKE_CURRENT_BINARY_DIR}" 
)

find_package(Boost REQUIRED COMPONENTS system program_options)
set(Boost_USE_STATIC_LIBS ON)

include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(bulk_server  
async
Boost::system
Boost::program_options
Threads::Threads
)

set_default_opts(bulk_server)

install(TARGETS bulk_server DESTINATION bin)