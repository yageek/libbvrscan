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
target_link_libraries(train_neural genann ${Foundation}  ${AppKit}) 