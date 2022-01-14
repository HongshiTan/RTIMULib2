cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(eillipose_fit)

find_package (Eigen3 3.3 REQUIRED NO_MOUDLE)

include_directories(${EIGEN3_INCLUDE_DIR})

add_executable (ellipsoid_fit_standalone ellipsoid_fit.cpp)
target_link_libraries(ellipsoid_fit_standalone Eigen3::Eigen)

add_library(RTEllipsoidFit SHARED ellipsoid_fit.cpp)
target_link_libraries(RTEllipsoidFit Eigen3::Eigen)
set_target_properties(RTEllipsoidFit PROPERTIES PUBLIC_HEADER ellipsoid_fit.h)
install(TARGETS RTEllipsoidFit
    LIBRARY
    PUBLIC_HEADER)
