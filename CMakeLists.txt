cmake_minimum_required(VERSION 3.16)

project(LibraryManagementSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------- SQLITE CONFIG ----------------
set(SQLITE3_INCLUDE_DIR "C:/sqlite/include")
set(SQLITE3_LIBRARY "C:/sqlite/lib/sqlite3.lib")
# -----------------------------------------------

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/third_party/sqlite/sqlite3.c
)

add_executable(app ${SOURCES})

target_include_directories(app PUBLIC src src/third_party/sqlite)


target_include_directories(app PUBLIC
    src
    ${SQLITE3_INCLUDE_DIR}
)


target_link_libraries(app
    PRIVATE
        ${SQLITE3_LIBRARY}
)



message(STATUS "SQLite include dir = ${SQLITE3_INCLUDE_DIR}")
