file(GLOB_RECURSE SOURCES "*.c")
add_library(libnewscript STATIC ${SOURCES})
target_include_directories(libnewscript PUBLIC "${SCMAKE_SOURCE_DIR}/include" PRIVATE "${CMAKE_SOURCE_DIR}/src")
set_target_properties(libnewscript PROPERTIES OUTPUT_NAME newscript)
