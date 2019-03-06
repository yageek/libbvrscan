cmake_minimum_required(VERSION 3.1)
project (bvr-scan-core VERSION 0.8.4 DESCRIPTION "BVRScan core library" LANGUAGES C)

# Core library
add_library(bvrscancore
    src/matrix/matrix.c
    src/io/io.c
    src/filter/filter.c
    src/blob/blob.c
)

target_include_directories(bvrscancore PRIVATE 
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(bvrscancore "-framework CoreGraphics -framework ImageIO -framework CoreFoundation -framework CoreServices")
# Testing
enable_testing()

add_custom_command(
        TARGET bvrscancore POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/tests/samples
                ${CMAKE_CURRENT_BINARY_DIR}/samples)

add_custom_command(
        TARGET bvrscancore POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/tests/outputs
                ${CMAKE_CURRENT_BINARY_DIR}/outputs)

## Matrix
add_executable(test_core tests/test.c tests/munit.c)
target_include_directories(bvrscancore 
PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include_private>
)
target_link_libraries(test_core bvrscancore)
add_test(test_core test_core)

