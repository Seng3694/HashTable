cmake_minimum_required(VERSION 2.4)

project(HashTable C)

set(HEADERS
	include/hash_table.h
	include/prime.h
)

set(SOURCES
	src/hash_table.c
	src/prime.c
)

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

if(BUILD_TEST)
	add_executable(${PROJECT_NAME}_test src/main.c)
	target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
endif()
