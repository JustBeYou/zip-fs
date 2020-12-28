cmake_minimum_required(VERSION 3.17.0)
project(zip-fs VERSION 1.0.0 LANGUAGES C)

# Package finder - *nix only
include(FindPkgConfig)

# Custom flags
set(C_FLAGS "-Wall -D_GNU_SOURCE")
set(C_FLAGS_DEBUG "${C_FLAGS} -g -DDEBUG -fsanitize=address")

# Cmake flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${C_FLAGS_DEBUG}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Targets
file(GLOB operations_src "operations/*.c")
add_executable(zip-fs main.c options.c zip_utils.c manager.c ${operations_src})

# FUSE
pkg_search_module(FUSE REQUIRED fuse3>=3.0.0)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FUSE_CFLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${FUSE_CFLAGS}")
include_directories(${FUSE_INCLUDE_DIRS})
target_link_libraries(zip-fs ${FUSE_LIBRARIES})

# libzip
pkg_search_module(ZIP REQUIRED libzip>=1.7.0)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ZIP_CFLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${ZIP_CFLAGS}")
include_directories(${ZIP_INCLUDE_DIRS})
target_link_libraries(zip-fs ${ZIP_LIBRARIES})
