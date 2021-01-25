cmake_minimum_required(VERSION 3.0.0)

# Add path to look for includes
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(utils)

project("async"
    VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER}
    DESCRIPTION "OTUS c++ homework #8: async"
    LANGUAGES CXX
)

# generate docs
option(ENABLE_DOXYGEN "Build documentation" OFF)


set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)


add_subdirectory(async)
list(APPEND EXTRA_LIBS async)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/async/inc")

add_subdirectory(async_demo)


#add_executable({PROJECT_NAME} main.cpp bulk.cpp )



# Package setup
install(TARGETS ${project_name} RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT ssbdex@yandex.ru)
include(CPack)


include(CTest)
enable_testing()



