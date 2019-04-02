# Search the required dependencies
# Add the executable
add_executable(asset_gen asset_gen/main.c)
target_link_libraries(asset_gen ${CAIRO_LIBRARY})
target_include_directories(asset_gen PUBLIC ${CAIRO_INCLUDE_DIR})

# Add train executable
add_executable(train_neural train_genann/train.c)
target_link_libraries(train_neural genann ${CAIRO_INCLUDE_DIR} bvrscan bvrscanio) 
target_link_libraries(train_neural ${CAIRO_LIBRARY})

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