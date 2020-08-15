cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(eillipose_fit)

find_package( PkgConfig )
pkg_check_modules( EIGEN3 REQUIRED eigen3 )
include_directories( ${EIGEN3_INCLUDE_DIRS} )

add_executable (ellipsoid_fit ellipsoid_fit.cpp)
#target_link_libraries (main ${PCL_LIBRARIES})