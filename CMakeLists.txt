cmake_minimum_required(VERSION 3.4.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

option(WITH_TESTS FALSE CACHE)

project(bulk_server VERSION ${PROJECT_VERSION})

add_subdirectory(src)

if(${WITH_TESTS})
    add_subdirectory(test)
    enable_testing()
    add_test(NAME all_test COMMAND bulk_server_test)
endif()