cmake_minimum_required( VERSION 3.22 )
project( UVPP LANGUAGES C CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )
add_compile_options( -Wall -Wextra -pedantic )

if( GTEST_PATH )
    message( "GTEST_PATH: ${GTEST_PATH}" )
endif( )

if( LIBUV_PATH )
    message( "LIBUV_PATH: ${LIBUV_PATH}" )
endif( )

include_directories( "${CMAKE_CURRENT_LIST_DIR}/include" )

add_subdirectory( src )
