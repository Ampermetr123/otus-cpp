cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)

# Add path to look for includes
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(utils)

set (ThisProject bayan)

project(${ThisProject}
    VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER}
    DESCRIPTION "OTUS c++ homework #7: bayan"
    LANGUAGES CXX
)

include(docs)

add_subdirectory(bayan)
add_subdirectory(test)

# Package setup
install(TARGETS ${ThisProject} RUNTIME)
set(CPACK_GENERATOR DEB)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT ssbdex@yandex.ru)
include(CPack)






