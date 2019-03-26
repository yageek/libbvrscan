# Search the required dependencies
find_library(Foundation Foundation)
find_library(AppKit AppKit)
find_library(CoreText CoreText)
find_library(CoreGraphics CoreGraphics)

# Add the executable
add_executable(asset_gen asset_gen/main.m)
target_link_libraries(asset_gen ${Foundation} ${CoreText} ${CoreGraphics} ${AppKit})

# Add train executable
add_executable(train_neural train_genann/train.m)
target_link_libraries(train_neural genann ${Foundation} ${AppKit} bvrscan) 
add_custom_command(
            TARGET train_neural POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/generated
                    ${CMAKE_CURRENT_BINARY_DIR}/generated
            )
add_custom_command(
            TARGET train_neural POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/neural_tests_imgs
                    ${CMAKE_CURRENT_BINARY_DIR}/neural_tests_imgs
            )