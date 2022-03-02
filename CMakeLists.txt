cmake_minimum_required(VERSION 3.16)
project(RestAPIApplication)

set(CMAKE_CXX_STANDARD 17)

add_executable(RestAPIApplication main.cpp)

set (PROJECT_SOURCES
        main.cpp
        mongodb_handler.h
        books_genre.h
        status.h
        http_server.h
        )

set(DEPENDENCIES
        Drogon
        mongocxx
        bsoncxx
        )

foreach (DEPENDENCE IN LISTS DEPENDENCIES)
    find_package(${DEPENDENCE} REQUIRED)
    if (${DEPENDENCE}_FOUND)
        message("-- " ${DEPENDENCE} " is FOUND")
    endif ()
endforeach ()

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CURL_INCLUDE_DIR}
        ${PROTOBUF_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${LIBMONGOCXX_INCLUDE_DIR}
        ${LIBBSONCXX_INCLUDE_DIR}
        /usr/local/include/mongocxx/v_noabi
        /usr/local/include/bsoncxx/v_noabi
        /usr/local/include/libmongoc-1.0
        /usr/local/include/libbson-1.0
)

target_link_libraries(${PROJECT_NAME}
        Drogon::Drogon
        mongo::bsoncxx_shared
        mongo::mongocxx_shared
        )