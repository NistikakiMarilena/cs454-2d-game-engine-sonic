cmake_minimum_required(VERSION 3.20)
project(application LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(${PROJECT_NAME} 
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        ASSETS="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        engine
)

include(FetchContent)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(engine PUBLIC nlohmann_json::nlohmann_json)

target_link_libraries(application PUBLIC nlohmann_json::nlohmann_json)

source_group(
    TREE 
        ${CMAKE_CURRENT_SOURCE_DIR} 
    PREFIX 
        "application" 
    FILES 
        ${SOURCES}
)