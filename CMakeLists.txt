cmake_minimum_required( VERSION 3.18 )

enable_language(CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure your project name
project( MyProject )

find_package( Boost 1.74.0 REQUIRED )
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory( src )
add_subdirectory( apps )