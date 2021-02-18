add_executable(join_server
    main.cpp 
    server.cpp
    storage.cpp
)
configure_file(version.h.in version.h)
target_include_directories(join_server PRIVATE 
    "${CMAKE_CURRENT_BINARY_DIR}" 
)

find_package(Boost REQUIRED COMPONENTS system program_options)
set(Boost_USE_STATIC_LIBS ON)

include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(join_server  
    Boost::system
    Boost::program_options
    Threads::Threads
)

set_target_properties(join_server PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
append_warn_flags(join_server)

install(TARGETS join_server DESTINATION bin)