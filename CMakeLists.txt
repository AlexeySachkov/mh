cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME "MH")
set(APP_TARGET_NAME "${PROJECT_NAME}_app")
set(GENERATOR_TARGET_NAME "${PROJECT_NAME}_generator")
set(MH_LIB_TARGET_NAME "${PROJECT_NAME}_lib")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
add_definitions("-Wall")

include_directories(include/)

set(APP_SOURCE_FILES src/main.cpp)
set(GENERATOR_SOURCE_FILES src/generator.cpp)
set(MH_LIB_SOURCE_FILES lib/mh.cpp)

add_library(${MH_LIB_TARGET_NAME} STATIC ${MH_LIB_SOURCE_FILES})

add_executable(${APP_TARGET_NAME} ${APP_SOURCE_FILES})
add_executable(${GENERATOR_TARGET_NAME} ${GENERATOR_SOURCE_FILES})

add_dependencies(${APP_TARGET_NAME} ${MH_LIB_TARGET_NAME})
target_link_libraries(${APP_TARGET_NAME} ${MH_LIB_TARGET_NAME})

add_dependencies(${GENERATOR_TARGET_NAME} ${MH_LIB_TARGET_NAME})
target_link_libraries(${GENERATOR_TARGET_NAME} ${MH_LIB_TARGET_NAME})

install(TARGETS ${APP_TARGET_NAME} ${GENERATOR_TARGET_NAME} ${MH_LIB_TARGET_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
