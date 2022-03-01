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

find_package(Drogon CONFIG REQUIRED)
find_package(libbsoncxx 3.6.6 REQUIRED)
find_package(libmongocxx 3.6.6 REQUIRED)

include_directories(${LIBMONGOCXX_LIBRARIES})
include_directories(${LIBBSONCXX_LIBRARIES})
include_directories(${LIBMONGOCXX_INCLUDE_DIRS})
include_directories(${LIBBSONCXX_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES})
target_include_directories (${PROJECT_NAME} PRIVATE /usr/local/include/mongocxx/v_noabi;
        /usr/local/include/bsoncxx/v_noabi
;/usr/local/include/libmongoc-1.0
;/usr/local/include/libbson-1.0)