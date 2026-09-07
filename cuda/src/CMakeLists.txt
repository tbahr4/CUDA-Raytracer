# Add subdirectories
#add_subdirectory(...)

# Gather files at this level
file (GLOB CUDA_SOURCE_FILES
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*.cu
)

target_sources(${CUDA_TARGET_NAME} PRIVATE ${CUDA_SOURCE_FILES})

# Propagate source files to parent
set(CUDA_SOURCE_FILES ${CUDA_SOURCE_FILES} PARENT_SCOPE)
