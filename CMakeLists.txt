cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)

# Add path to look for includes
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(utils)


project("join_server"
    VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER}
    DESCRIPTION "OTUS c++ homework #10: join_server"
    LANGUAGES CXX
)

# pthread required
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# gtest
set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
add_subdirectory("googletest" "googletest")
if (MSVC)
    set(gtest_force_shared_crt ON cache BOOL "" FORCE)
endif()
include(CTest)
enable_testing()


add_subdirectory(join_server)
add_subdirectory(join_server/test)

# generate docs options
option(BUILD_DOCUMENTATION "Build documentation" ON)
# Documentation setup
if(BUILD_DOCUMENTATION)
    find_package(Doxygen
                REQUIRED dot
                OPTIONAL_COMPONENTS mscgen dia)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/docs")
        # install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION ${CMAKE_SOURCE_DIR}/docs)
        set(DOXYGEN_PROJECT_LOGO "${CMAKE_SOURCE_DIR}/logo3.png")
        set(DOXYGEN_GENERATE_TREEVIEW YES)
        set(DOXYGEN_HIDE_UNDOC_RELATIONS NO)
        set(DOXYGEN_HAVE_DOT YES)
        set(DOXYGEN_DOT_NUM_THREADS 4)
        set(DOXYGEN_UML_LOOK YES)
        set(DOXYGEN_UML_LIMIT_NUM_FIELDS 50)
        set(DOXYGEN_TEMPLATE_RELATIONS YES)
        set(DOXYGEN_DOT_IMAGE_FORMAT svg)
        set(DOXYGEN_INTERACTIVE_SVG YES)
        set(DOXYGEN_DOT_GRAPH_MAX_NODES 100)
        set(DOXYGEN_DOT_TRANSPARENT YES)
        set(DOXYGEN_DOT_MULTI_TARGETS YES)
        set(USE_MDFILE_AS_MAINPAGE YES)
        doxygen_add_docs(doc
          ${CMAKE_SOURCE_DIR}/join_server
          COMMENT "Generating doxygen documentation for ${PROJECT_NAME}"
        )
    else()
    message(WARNING "doxygen and graphviz need to be installed to generate the doxygen documentation") 
    endif(DOXYGEN_FOUND)
endif(BUILD_DOCUMENTATION)

# Package setup
install(TARGETS join_server RUNTIME)
set(CPACK_GENERATOR DEB)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT ssbdex@yandex.ru)
include(CPack)






