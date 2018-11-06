file(GLOB_RECURSE SOURCES "*.c")
add_executable(newscript ${SOURCES})
target_include_directories(newscript PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(newscript libnewscript)
