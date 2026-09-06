set(TEST_TARGET_NAME CudaRaytracerTests)


# Gather headers
file(GLOB_RECURSE HEADER_FILES
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/common/include/${TEST_TARGET_NAME}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/common/include/${TEST_TARGET_NAME}/*.h
)


# Setup target
add_executable(${TEST_TARGET_NAME} ${HEADER_FILES})
target_link_libraries(${TEST_TARGET_NAME} ${TARGET_NAME})

target_include_directories(${TEST_TARGET_NAME} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/common/include
)


# Add subdirectories
add_subdirectory(core)


# Setup folder hierarchy
source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}/common/include/${TEST_TARGET_NAME}
	PREFIX "common/include/${TEST_TARGET_NAME}"
	FILES ${HEADER_FILES}
)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}/common/src
	PREFIX "common/src"
	FILES ${SOURCE_FILES}
)


# Apply build options
if (MSVC)
	if (${CudaRaytracer_ENABLE_EXTRA_WARNINGS})
		target_compile_options(${TEST_TARGET_NAME} PRIVATE /W4 /permissive-)
	endif()
	
	if (${CudaRaytracer_TREAT_WARNINGS_AS_ERRORS})
		target_compile_options(${TEST_TARGET_NAME} PRIVATE /WX)
	endif()
	
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	if (${CudaRaytracer_ENABLE_EXTRA_WARNINGS})
		target_compile_options(${TEST_TARGET_NAME} PRIVATE
			-Wall
			-Wextra
			-Wpedantic
			-Wshadow
			-Wconversion
			-Wsign-conversion
		)
	endif()
	
	if (${CudaRaytracer_TREAT_WARNINGS_AS_ERRORS})
		target_compile_options(${TEST_TARGET_NAME} PRIVATE -Werror)
	endif()

endif()
