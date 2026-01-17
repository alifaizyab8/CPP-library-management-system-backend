cmake_minimum_required(VERSION 3.16)

project(LibraryManagementSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------- SOURCE FILES ----------------
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/third_party/sqlite/sqlite3.c   # Only used if you bundle SQLite on Windows
)

add_executable(app ${SOURCES})

target_include_directories(app PUBLIC src)

# ---------------- SQLITE CONFIG ----------------
if (WIN32)
    # On Windows, use your manually downloaded SQLite files
    set(SQLITE3_INCLUDE_DIR "C:/sqlite/include")
    set(SQLITE3_LIBRARY "C:/sqlite/lib/sqlite3.lib")

    target_include_directories(app PUBLIC ${SQLITE3_INCLUDE_DIR})
    target_link_libraries(app PRIVATE ${SQLITE3_LIBRARY})

elseif(UNIX)
    # On Linux / Ubuntu, use system-installed SQLite3
    find_package(SQLite3 REQUIRED)
    if (SQLite3_FOUND)
        target_include_directories(app PUBLIC ${SQLite3_INCLUDE_DIRS})
        target_link_libraries(app PRIVATE ${SQLite3_LIBRARIES})
    else()
        message(FATAL_ERROR "SQLite3 not found! Please install libsqlite3-dev")
    endif()
endif()

# ---------------- STATUS ----------------
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
