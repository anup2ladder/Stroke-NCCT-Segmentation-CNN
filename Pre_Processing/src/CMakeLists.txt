cmake_minimum_required(VERSION 2.8)

project(ct-brain-extraction)

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})


set( project_sources
    itkSliceBySliceImageFilter.txx
	imageManipulation.cpp
    main.cpp
)

add_executable(ct-brain-extraction ${project_sources})

target_link_libraries(ct-brain-extraction ${ITK_LIBRARIES})
