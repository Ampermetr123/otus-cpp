set (This bayan)
set (ThisLib bayan_lib)

# pthread required
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# Boost
find_package(Boost REQUIRED COMPONENTS system program_options regex)
set(Boost_USE_STATIC_LIBS ON)
include_directories(${Boost_INCLUDE_DIR})

set (Sourses
    file_list.cpp
    wildcard_impl.cpp
    file_dup.cpp
)

set (Headers
    file_list.h
    file_dup.h
    wildcard.h
)

add_executable(${This} main.cpp )
add_library(${ThisLib} ${Sourses} ${Headers})

# Setup debug output
target_compile_definitions(${This} PRIVATE VERBOSE_LEVEL=0 )
target_compile_definitions(${ThisLib} PRIVATE VERBOSE_LEVEL=0 )

configure_file(version.h.in version.h)
target_include_directories(${This} PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${This}
    ${ThisLib}  
    Boost::system
    Boost::program_options
    Boost::regex
)

target_link_libraries(${ThisLib} 
    Boost::system
    Boost::program_options
    Boost::regex
)

set_target_properties(${This} ${ThisLib} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

set_default_opts(${This})
set_default_opts(${ThisLib})

install(TARGETS ${This} DESTINATION bin)

