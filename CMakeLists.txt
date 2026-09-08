cmake_minimum_required(VERSION 3.27)
project(Raytracer
	LANGUAGES CXX CUDA
)
set(TARGET_NAME Raytracer)
set(CUDA_TARGET_NAME RaytracerCuda)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# Setup configurable build options
option(Raytracer_ENABLE_EXTRA_WARNINGS "Enable more strict warnings" ON)
option(Raytracer_TREAT_WARNINGS_AS_ERRORS "Treat warnings as a compiler error" OFF)
option(Raytracer_BUILD_TESTS "Build tests" OFF)
set(Raytracer_CUDA_ARCHITECTURES "89"
	CACHE STRING "CUDA architecture type"
)

if (MSVC)
	option(Raytracer_ENABLE_MULTI_PROCESS_COMPILE "Enable multi-process compilation (/MP)" ON)
	option(Raytracer_ENABLE_JUST_MY_CODE "Enable just my code debug flag (/JMC)" ON)
endif()


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

add_executable(${TARGET_NAME} ${HEADER_FILES})

target_include_directories(${TARGET_NAME} 
	INTERFACE
		${PROJECT_SOURCE_DIR}/include
		
	PRIVATE
		${PROJECT_SOURCE_DIR}/include/${TARGET_NAME}
		${PROJECT_SOURCE_DIR}/cuda/include
		${stb_SOURCE_DIR}
)


# Add subdirectories
add_subdirectory(src)
add_subdirectory(cuda)

# Link libraries
target_link_libraries(${TARGET_NAME}
	PRIVATE
		${CUDA_TARGET_NAME}

	PUBLIC
		SDL3::SDL3
		nlohmann_json::nlohmann_json
)


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
	if (${Raytracer_ENABLE_EXTRA_WARNINGS})
		target_compile_options(${TARGET_NAME} PRIVATE /W4 /permissive-)
	endif()
	
	if (${Raytracer_TREAT_WARNINGS_AS_ERRORS})
		target_compile_options(${TARGET_NAME} PRIVATE /WX)
	endif()
	
	if (${Raytracer_ENABLE_MULTI_PROCESS_COMPILE})
		target_compile_options(${TARGET_NAME} PRIVATE /MP)
	endif()
	
	if (${Raytracer_ENABLE_JUST_MY_CODE})
		target_compile_options(${TARGET_NAME} PRIVATE $<$<CONFIG:Debug>:/JMC>)
	endif()
	
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	if (${Raytracer_ENABLE_EXTRA_WARNINGS})
		target_compile_options(${TARGET_NAME} PRIVATE
			-Wall
			-Wextra
			-Wpedantic
			-Wshadow
			-Wconversion
			-Wsign-conversion
		)
	endif()
	
	if (${Raytracer_TREAT_WARNINGS_AS_ERRORS})
		target_compile_options(${TARGET_NAME} PRIVATE -Werror)
	endif()

endif()


if (${Raytracer_BUILD_TESTS})
	add_subdirectory(test)
endif()
