cmake_minimum_required(VERSION 3.27)
project(CudaRaytracer)
set(TARGET_NAME CudaRaytracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# Setup optional build flags
option(${TARGET_NAME}_ENABLE_EXTRA_WARNINGS "Enable more strict warnings" ON)
option(${TARGET_NAME}_TREAT_WARNINGS_AS_ERRORS "Treat warnings as a compiler error" OFF)
option(${TARGET_NAME}_BUILD_TESTS "Build tests" OFF)


# Gather headers
file(GLOB_RECURSE HEADER_FILES
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/include/${TARGET_NAME}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/${TARGET_NAME}/*.h
)

# Setup target
add_library(${TARGET_NAME} ${HEADER_FILES})

target_include_directories(${TARGET_NAME} 
	INTERFACE ${PROJECT_SOURCE_DIR}/include
	PRIVATE ${PROJECT_SOURCE_DIR}/include/${TARGET_NAME}
)


# Add subdirectories
add_subdirectory(src)


# Setup folder hierarchy
source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/${TARGET_NAME}
	PREFIX "include/${TARGET_NAME}"
	FILES ${HEADER_FILES}
)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
	PREFIX "src"
	FILES ${SOURCE_FILES}
)


# Apply build options
if (MSVC)
	if (${CPPUTIL_ENABLE_EXTRA_WARNINGS})
		target_compile_options(${TARGET_NAME} PRIVATE /W4 /permissive-)
	endif()
	
	if (${CPP_UTIL_TREAT_WARNINGS_AS_ERRORS})
		target_compile_options(${TARGET_NAME} PRIVATE /WX)
	endif()
	
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	if (${CPPUTIL_ENABLE_EXTRA_WARNINGS})
		target_compile_options(${TARGET_NAME} PRIVATE
			-Wall
			-Wextra
			-Wpedantic
			-Wshadow
			-Wconversion
			-Wsign-conversion
		)
	endif()
	
	if (${CPP_UTIL_TREAT_WARNINGS_AS_ERRORS})
		target_compile_options(${TARGET_NAME} PRIVATE -Werror)
	endif()

endif()


if (${CPPUTIL_BUILD_TESTS})
	add_subdirectory(test)
endif()