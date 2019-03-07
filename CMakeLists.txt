cmake_minimum_required(VERSION 3.1)
project (bvrscan VERSION 0.8.4 DESCRIPTION "BVRScan core library" LANGUAGES C)

# Core library
add_library(bvrscan
    src/matrix/matrix.c
    src/io/io.c
    src/filter/filter.c
    src/blob/blob.c
)

target_include_directories(bvrscan PRIVATE 
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(bvrscan "-framework CoreGraphics -framework ImageIO -framework CoreFoundation -framework CoreServices")

# Testing Core
enable_testing()

add_custom_command(
        TARGET bvrscan POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/tests/samples
                ${CMAKE_CURRENT_BINARY_DIR}/samples)

add_custom_command(
        TARGET bvrscan POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/tests/outputs
                ${CMAKE_CURRENT_BINARY_DIR}/outputs)

## Matrix
add_executable(test_core tests/test.c tests/munit.c)
target_include_directories(bvrscan 
PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include_private>
)
target_link_libraries(test_core bvrscan)
add_test(test_core test_core)

add_subdirectory(assets)