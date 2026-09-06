cmake_minimum_required(VERSION 3.27)
project(CudaRaytracer)
set(TARGET_NAME CudaRaytracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# Setup optional build flags
option(CudaRaytracer_ENABLE_EXTRA_WARNINGS "Enable more strict warnings" ON)
option(CudaRaytracer_TREAT_WARNINGS_AS_ERRORS "Treat warnings as a compiler error" OFF)
option(CudaRaytracer_BUILD_TESTS "Build tests" OFF)


# Gather headers
file(GLOB_RECURSE HEADER_FILES
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/include/${TARGET_NAME}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/${TARGET_NAME}/*.h
)


#
# Dependencies 
#

include (FetchContent)

# CPM
set(CPM_DOWNLOAD_VERSION 0.43.1)
set(CPM_DOWNLOAD_LOCATION
    "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake"
)

if (NOT EXISTS "${CPM_DOWNLOAD_LOCATION}")
    file(
        DOWNLOAD
        "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake"
        "${CPM_DOWNLOAD_LOCATION}"
        TLS_VERIFY ON
    )
endif()

include("${CPM_DOWNLOAD_LOCATION}")


# SDL3
CPMAddPackage(
    NAME SDL3
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG release-3.4.16
    OPTIONS
        "SDL_SHARED OFF"
        "SDL_STATIC ON"
        "SDL_TEST_LIBRARY OFF"
        "SDL_TESTS OFF"
        "SDL_EXAMPLES OFF"
)


# JSON
CPMAddPackage(
    NAME nlohmann_json
    GITHUB_REPOSITORY nlohmann/json
    GIT_TAG v3.12.0
)


# stb_image
CPMAddPackage(
    NAME stb
    GITHUB_REPOSITORY nothings/stb
    GIT_TAG master
)


#
# Setup Target
#

add_library(${TARGET_NAME} ${HEADER_FILES})

target_link_libraries(${TARGET_NAME}
	PUBLIC
		SDL3::SDL3
		nlohmann_json::nlohmann_json
)

target_include_directories(${TARGET_NAME} 
	INTERFACE ${PROJECT_SOURCE_DIR}/include
	PRIVATE
		${PROJECT_SOURCE_DIR}/include/${TARGET_NAME}
		${stb_SOURCE_DIR}
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


#
# Build Options
#

if (MSVC)
	if (${CudaRaytracer_ENABLE_EXTRA_WARNINGS})
		target_compile_options(${TARGET_NAME} PRIVATE /W4 /permissive-)
	endif()
	
	if (${CudaRaytracer_TREAT_WARNINGS_AS_ERRORS})
		target_compile_options(${TARGET_NAME} PRIVATE /WX)
	endif()
	
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	if (${CudaRaytracer_ENABLE_EXTRA_WARNINGS})
		target_compile_options(${TARGET_NAME} PRIVATE
			-Wall
			-Wextra
			-Wpedantic
			-Wshadow
			-Wconversion
			-Wsign-conversion
		)
	endif()
	
	if (${CudaRaytracer_TREAT_WARNINGS_AS_ERRORS})
		target_compile_options(${TARGET_NAME} PRIVATE -Werror)
	endif()

endif()


if (${CudaRaytracer_BUILD_TESTS})
	add_subdirectory(test)
endif()