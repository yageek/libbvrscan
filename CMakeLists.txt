cmake_minimum_required(VERSION 3.1)
project (bvrscan VERSION 0.8.4 DESCRIPTION "BVRScan core library" LANGUAGES C)

# Option to build external elements
option(UNIT_TEST "Build the unit test." OFF)
option(TRAINING "Build the training set elements (macOS only to generate assets)" OFF)

# CMake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(Cairo REQUIRED)

# Third party
add_subdirectory(thirdParty)

# Core library
add_library(bvrscan
    src/matrix/matrix.c
    src/filter/filter.c
    src/blob/blob.c
    src/neural/neural.c
)

target_include_directories(bvrscan 
PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include_private>
)

target_link_libraries(bvrscan ${CAIRO_LIBRARY} genann)
target_include_directories(bvrscan PRIVATE ${CAIRO_INCLUDE_DIR} genann)

# Image IO library for training and computer test
add_library(bvrscanio
        imageio/src/io.c        
)

target_include_directories(bvrscanio 
PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imageio/include>
PRIVATE ${CAIRO_INCLUDE_DIR}
)
target_link_libraries(bvrscanio bvrscan ${CAIRO_INCLUDE_DIR})

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
        target_link_libraries(test_core bvrscan bvrscanio)
        add_test(test_core test_core)

endif(UNIT_TEST)

if(TRAINING)
        add_subdirectory(assets)
endif(TRAINING)