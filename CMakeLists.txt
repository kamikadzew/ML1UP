cmake_minimum_required(VERSION 2.8)

project(ML1UP CXX)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# magic for various platform support

if(MINGW)
    # Guess MINGDIR from the value of CMAKE_CXX_COMPILER if it's not set.
    if("$ENV{MINGDIR}" STREQUAL "")
        string(REGEX REPLACE "/bin/[^/]*$" "" MINGDIR "${CMAKE_CXX_COMPILER}")
        message(STATUS "Guessed MinGW directory: ${MINGDIR}")
    else("$ENV{MINGDIR}" STREQUAL "")
        file(TO_CMAKE_PATH "$ENV{MINGDIR}" MINGDIR)
        message(STATUS "Using MINGDIR: ${MINGDIR}")
    endif("$ENV{MINGDIR}" STREQUAL "")

    # Search in MINGDIR for headers and libraries.
    set(CMAKE_PREFIX_PATH "${MINGDIR}")

    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mwindows")

endif(MINGW)

# end of magic

SET(SRC_LIST
  main.cpp
  lists.cpp
)

find_package(Allegro5 REQUIRED)
find_package(Allegro5Font REQUIRED)
find_package(Allegro5TTF REQUIRED)
find_package(Allegro5Primitives REQUIRED)
find_package(Allegro5NativeDialogs REQUIRED)
if(APPLE)
  find_package(Allegro5Main)
endif(APPLE)

add_executable(ML1UP WIN32 ${SRC_LIST})

include_directories(${ALLEGRO5_INCLUDE_DIR} ${ALLEGRO5_FONT_INCLUDE_DIR} ${ALLEGRO5_TTF_INCLUDE_DIR} ${ALLEGRO5_PRIMITIVES_INCLUDE_DIR} ${ALLEGRO5_NATIVE_DIALOGS_INCLUDE_DIR})

target_link_libraries(ML1UP ${ALLEGRO5_LIBRARIES} ${ALLEGRO5_FONT_LIBRARIES} ${ALLEGRO5_TTF_LIBRARIES} ${ALLEGRO5_PRIMITIVES_LIBRARIES} ${ALLEGRO5_NATIVE_DIALOGS_LIBRARIES} ${ALLEGRO5_MAIN_LIBRARIES} m)
