cmake_minimum_required(VERSION 3.16)

project(LibraryManagementSystem LANGUAGES CXX C)  # ← ADD 'C' HERE!

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------- SOURCE FILES ----------------
file(GLOB_RECURSE CPP_SOURCES src/*.cpp)

# Explicitly add SQLite on Windows
if(WIN32)
    set(SQLITE_SOURCE src/third_party/sqlite/sqlite3.c)
    
    # Tell CMake this is a C file, not C++
    set_source_files_properties(${SQLITE_SOURCE} PROPERTIES LANGUAGE C)
    
    set(ALL_SOURCES ${CPP_SOURCES} ${SQLITE_SOURCE})
    message(STATUS "Using static SQLite on Windows: ${SQLITE_SOURCE}")
else()
    set(ALL_SOURCES ${CPP_SOURCES})
endif()

add_executable(app ${ALL_SOURCES})

target_include_directories(app PUBLIC 
    src 
    src/third_party/sqlite
)

# ---------------- SQLITE CONFIG ----------------
if(WIN32)
    # On Windows: SQLite compiled from source
    # No additional libraries needed for static linking
    
elseif(UNIX)
    # On Linux: Use system SQLite
    find_package(SQLite3 REQUIRED)
    if(SQLite3_FOUND)
        target_include_directories(app PUBLIC ${SQLite3_INCLUDE_DIRS})
        target_link_libraries(app PRIVATE ${SQLite3_LIBRARIES})
        message(STATUS "Using system SQLite on Linux")
    else()
        message(FATAL_ERROR "SQLite3 not found! Install: sudo apt install libsqlite3-dev")
    endif()
endif()

# ---------------- STATUS ----------------
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")