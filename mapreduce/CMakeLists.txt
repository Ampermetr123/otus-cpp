add_executable(mapreduce
    main.cpp 
    mapreduce.cpp
)

configure_file(version.h.in version.h)
target_include_directories(mapreduce PRIVATE 
    "${CMAKE_CURRENT_BINARY_DIR}" 
)

#test file for demo
configure_file(test.txt.in test.txt)
configure_file(test150.txt.in test150.txt)
configure_file(test3000.txt.in test3000.txt)

find_package(Boost REQUIRED COMPONENTS program_options)
set(Boost_USE_STATIC_LIBS ON)

include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(mapreduce  
Boost::program_options
Threads::Threads
)

set_default_opts(mapreduce)

install(TARGETS mapreduce DESTINATION bin)