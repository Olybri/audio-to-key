cmake_minimum_required(VERSION 2.6)

project(Audio-to-Key)

set(EXECUTABLE_OUTPUT_PATH bin)

include_directories(C:/Users/loris/Tools/sfml/include)
link_directories(C:/Users/loris/Tools/sfml/lib C:/Users/loris/Tools/sfml/extlibs/libs-mingw/x86)

set(CMAKE_CXX_FLAGS "-s -O3 -static -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wfloat-equal -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef")

file(GLOB_RECURSE source_files src/*)
add_executable(atk ${source_files})

target_link_libraries(atk sfml-audio-s sfml-system-s openal32 flac vorbisenc vorbisfile vorbis ogg winmm)

add_definitions(-DSFML_STATIC)
