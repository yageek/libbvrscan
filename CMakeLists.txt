cmake_minimum_required(VERSION 3.1)
project (bvrscan VERSION 0.8.4 DESCRIPTION "BVRScan core library" LANGUAGES C)

# Option to build external elements

option(UNIT_TEST "Build the unit test." OFF)
option(TRAINING "Build the training set elements (macOS only to generate assets)" OFF)

# Core library
add_library(bvrscan
    src/matrix/matrix.c
    src/io/io.c
    src/filter/filter.c
    src/blob/blob.c
    src/neural/neural.c src/neural/neural_v1.c

)

target_include_directories(bvrscan 
PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include_private>
)

# OSX framework
find_library(CORE_GRAPHICS CoreGraphics)
find_library(IMAGE_IO ImageIO)
find_library(CORE_FOUNDATION CoreFoundation)
find_library(CORE_SERVICES CoreServices)

set(MACOS_EXTRA_LIBS ${CORE_GRAPHICS} ${IMAGE_IO} ${CORE_FOUNDATION} ${CORE_SERVICES})
target_link_libraries(bvrscan ${MACOS_EXTRA_LIBS})

if(UNIT_TEST)
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

endif(UNIT_TEST)

if(TRAINING)
        add_subdirectory(assets)
endif(TRAINING)