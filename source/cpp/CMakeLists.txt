# Add the source in project root directory

# Add header file include directories
include_directories(./ ./include)
# Add block directories

add_subdirectory(emitter)
add_subdirectory(propagation)
add_subdirectory(ds)
add_subdirectory(type)
