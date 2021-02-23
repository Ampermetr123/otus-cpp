set (This join_server)
set (ThisLib join_server_lib)

set (Sources
    server.cpp
    storage.cpp
    session.cpp
    iterable_result.cpp
)

set (Headers
    iterable_result.h 
    optlog.h
    session.h
    storage.h
    server.h
    iterable_result.h
)

add_executable(${This} main.cpp )

configure_file(version.h.in version.h)
configure_file(test/test_intersection.txt.in test_intersection.txt)
configure_file(test/test_difference.txt.in test_difference.txt)
target_include_directories(${This} PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}
)
add_library(${ThisLib} ${Sources} ${Headers})

find_package(Boost REQUIRED COMPONENTS system program_options)
set(Boost_USE_STATIC_LIBS ON)

include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(${This}
    ${ThisLib}  
    Boost::system
    Boost::program_options
    Threads::Threads
)

target_link_libraries(${ThisLib} 
    Boost::system
    Boost::program_options
    Threads::Threads
)

set_target_properties(${This} ${ThisLib} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
append_warn_flags(${This})
append_warn_flags(${ThisLib})

install(TARGETS ${This} DESTINATION bin)

