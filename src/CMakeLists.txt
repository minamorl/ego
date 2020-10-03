set(HEADER_LIST "${MyProject_SOURCE_DIR}/include/lib.h")

# Make an automatic library - will be static or dynamic based on user setting
add_library(mylib lib.cpp ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(mylib PUBLIC ../include)

# This depends on (header only) boost
target_link_libraries(mylib PRIVATE Boost::boost)