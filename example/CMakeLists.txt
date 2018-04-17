cmake_minimum_required(VERSION 3.7)

# Basic usage.
add_executable(basic basic/basic.cpp)
target_link_libraries(basic logg)

# Compiler optimizations.
add_executable(optimization optimization/optimization.cpp)
target_link_libraries(optimization logg)

# Compound log messgae.
add_executable(compound compound/compound.cpp)
target_link_libraries(compound logg)

# Custom log levels.
add_executable(custom custom/custom.cpp)
target_link_libraries(custom logg)

# Source location macros.
add_executable(location location/location.cpp)
target_link_libraries(location logg)
