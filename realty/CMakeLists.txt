
# pthread required
# set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
# set(THREADS_PREFER_PTHREAD_FLAG TRUE)
# find_package(Threads REQUIRED)

# Boost
# find_package(Boost REQUIRED COMPONENTS system program_options regex)
# set(Boost_USE_STATIC_LIBS ON)
# include_directories(${Boost_INCLUDE_DIR})


add_executable(rclst rclst.cpp realty.h)
add_executable(rclss rclss.cpp realty.h)

target_link_libraries(rclst
    dlib
    blas
    lapack
)

target_link_libraries(rclss
    dlib
    blas
    lapack
)

# set_default_opts(rclss)

# set_default_opts(rclst)

set_target_properties(rclst rclss PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

install(TARGETS rclss rclst DESTINATION bin)

