find_package(Threads REQUIRED)
find_package(Boost REQUIRED)

add_library(bulk_server STATIC AsyncCommandWriter.cpp
                               CommandProcessingContext.cpp
                               CommandProcessor.cpp
                               ConsoleCommandWriter.cpp
                               DynamicCommandProcessor.cpp
                               FileCommandWriter.cpp
                               RequestParser.cpp
                               Server.cpp
                               StaticCommandProcessor.cpp
                               TcpConnection.cpp
                               Util.cpp)
add_executable(bulk_server_cli Main.cpp)

#rename the executable to "bulk_server"
set_target_properties(bulk_server_cli PROPERTIES OUTPUT_NAME bulk_server)

target_include_directories(bulk_server PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(bulk_server ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(bulk_server_cli PRIVATE bulk_server)

#BEGIN release
install(TARGETS bulk_server_cli)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
#END
