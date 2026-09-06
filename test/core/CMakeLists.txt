# Add subdirectories
#add_subdirectory(...)

# Gather files at this level
file (GLOB_RECURSE SOURCE_FILES
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

target_sources(${TEST_TARGET_NAME} PRIVATE ${SOURCE_FILES})

# Propagate source files to parent
set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
