cmake_minimum_required( VERSION 3.18 )

enable_language(CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure your project name
project( MyProject )

# find_package( Boost REQUIRED )

add_subdirectory( src )
add_subdirectory( apps )