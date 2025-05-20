cmake_minimum_required(VERSION 3.10)
project(PokeTODO)

set(CMAKE_CXX_STANDARD 17)

# include 디렉토리 추가
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/services
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models
    ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

# 소스 파일 수집 (src 폴더 내부)
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/services/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/models/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.cpp"
)

# main.cpp는 별도로 지정하고, SRC_FILES에서는 제외 (중복 방지)
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Tamagotchi 관련 주석 파일 제외 (경로 수정)
list(FILTER SRC_FILES EXCLUDE REGEX ".*src/models/Tamagotchi.*\\.cpp$")

add_executable(PokeTODO src/main.cpp ${SRC_FILES}) 