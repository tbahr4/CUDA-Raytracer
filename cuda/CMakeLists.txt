# Gather headers
file(GLOB_RECURSE CUDA_HEADER_FILES
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.cuh
)

#
# Setup Target
#

add_library(${CUDA_TARGET_NAME} STATIC
	${CUDA_HEADER_FILES}
)

target_include_directories(${CUDA_TARGET_NAME} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(CUDAToolkit REQUIRED)

target_link_libraries(${CUDA_TARGET_NAME}
	PRIVATE CUDA::cudart
)

set_target_properties(${CUDA_TARGET_NAME} PROPERTIES
	CUDA_ARCHITECTURES "${Raytracer_CUDA_ARCHITECTURES}"
	CUDA_SEPARABLE_COMPILATION ON
)


# Add subdirectories
add_subdirectory(src)


# Setup folder hierarchy
source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/${CUDA_TARGET_NAME}
	PREFIX "include/${CUDA_TARGET_NAME}"
	FILES ${CUDA_HEADER_FILES}
)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
	PREFIX "src"
	FILES ${CUDA_SOURCE_FILES}
)
