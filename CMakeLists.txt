cmake_minimum_required(VERSION 3.7)

project(Audio-to-Key)

set(EXECUTABLE_OUTPUT_PATH bin)

if(NOT SFML_PATH)
  message(SEND_ERROR "SFML_PATH variable not set!")
endif()

include_directories(${SFML_PATH}/include)
link_directories(${SFML_PATH}/lib ${SFML_PATH}/extlibs/libs-mingw/x86)

set(CMAKE_CXX_FLAGS "-s -O3 -static")

file(GLOB_RECURSE source_files src/*)
add_executable(atk ${source_files})

target_link_libraries(atk stdc++fs sfml-audio-s sfml-system-s openal32 winmm)

add_definitions(-DSFML_STATIC)
