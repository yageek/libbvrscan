# Genann
add_library(genann ${CMAKE_CURRENT_SOURCE_DIR}/genann/genann.c)
target_include_directories(genann
PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/genann>
)

